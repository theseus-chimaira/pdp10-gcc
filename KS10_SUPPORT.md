# KS10 Compiler Support Policy

This document defines what `-march=ks10` means for the DAIMOS PDP-10 C
compiler backend.  It is a compiler contract, not a complete KS10 hardware
manual.  The scope is ordinary C code generation, C runtime helpers, and
future libc tuning.  It deliberately excludes monitor-only device I/O except
where inline assembler or kernel code must know that it is not portable.

## Summary

For DAIMOS, `-march=ks10` is an 18-bit local-address PDP-10 target with
KS/KL-class arithmetic improvements.  It is not a KL10 Model B extended-section
target.

The compiler may treat KS10 as:

```text
KS10 = base PDP-10 C instruction set
       + KI10-style doubleword moves and double-format floating point
       + KS/KL double-integer arithmetic
       + optional string/decimal EXTEND support for libc only
       - KL10 Model B extended sections
       - XBLT
       - G-float by default
```

The default DAIMOS userland target should remain the conservative PDP-6/KA10
baseline.  `-march=ks10` is a stronger target profile for KS10 machines and for
emulators that model the KS10 instruction set closely enough.

## Current backend facts

At the time this document was written, the GCC PDP-10 backend has the following
relevant implementation facts:

- `gcc/config/pdp10/pdp10.c` maps `-march=ks10` to `CPU_KS10` and disables
  `CPU_EXTENDED` in the CPU table.
- `gcc/config/pdp10/pdp10.h` defines `TARGET_KL10up` as true for KS10.
- `gcc/config/pdp10/pdp10.h` defines `TARGET_KI10up` through `TARGET_KL10up`,
  so KS10 also satisfies KI10-up predicates.
- `TARGET_STRING` is currently hard-disabled as `0`, even though the command
  line has `-mstring` and `-mno-string` switches.
- The backend defaults ordinary `long long` to the 71-bit PDP-10 doubleword
  integer model unless `-mlong-long-72bit` is selected.
- XBLT selection in `pdp10.c` requires `TARGET_EXTENDED`; therefore plain
  `-march=ks10` must not select XBLT.
- The `DDIV` machine-description pattern is currently guarded by `0 &&
  TARGET_71BIT`, so it is not emitted today.

These facts should not be treated as accidents.  They match the policy below,
except that future patches may turn selected KS10 features into tested codegen.

## Addressing model

Plain `-march=ks10` means local 18-bit PDP-10 addressing.

Allowed:

- Normal 18-bit effective addresses.
- Normal section-zero style C pointers.
- Normal `MOVEI`, index, indirection, byte pointers, and local labels.

Not allowed by plain `-march=ks10`:

- `XMOVEI` as the normal address-generation instruction.
- KL10 Model B nonzero sections.
- Multi-section functions or data as a normal C ABI.
- `XBLT`.

If a future command-line combination permits `-march=ks10 -mextended`, DAIMOS
should reject it or diagnose it.  KS10 support must not silently become a fake
KL10B target.

## Baseline C instructions

The following instruction families are safe for ordinary C code generation on
KS10 and are also part of the shared PDP-10-style baseline unless a smaller
PDP-6/KA10 profile says otherwise:

| C need | Instruction families | `-march=ks10` policy |
| --- | --- | --- |
| Word moves | `MOVE`, `MOVEM`, `MOVS`, `EXCH`, `SET*` | Allowed. |
| Integer arithmetic | `ADD`, `SUB`, `IMUL`, `IDIV`, `MUL`, `DIV` | Allowed. |
| Logic | `AND`, `IOR`, `XOR`, `EQV`, `ANDC*`, `ORC*` | Allowed. |
| Branches and skips | `JUMP*`, `SKIP*`, `CAI*`, `CAM*`, `AOJ*`, `SOJ*` | Allowed. |
| Halfwords | `HRR*`, `HLR*`, `HRL*`, `HLL*` | Allowed. |
| Calls and stack | `PUSHJ`, `POPJ`, `PUSH`, `POP` | Allowed. |
| Byte access | `LDB`, `DPB`, `ILDB`, `IDPB`, `IBP` | Allowed. |
| Word block copy | `BLT` | Allowed; prefer over XBLT. |

The compiler must not rely on KA10/PDP-6 pushdown carry quirks.  Stack-pointer
code must remain valid on KI/KL/KS-style pushdown behavior.

## Doubleword moves

`-march=ks10` may use KI10-up doubleword move instructions when profitable:

```asm
dmove
dmovem
dmovn
dmovnm
```

Use cases:

- Moving `DImode` values.
- Moving double-format floating-point values.
- Optimizing doubleword temporaries.
- Selected `long long` helper sequences.

These instructions are not part of the PDP-6/KA10 baseline.  They must remain
behind `TARGET_KI10up` or a stricter predicate.

## Double-integer arithmetic

`-march=ks10` may use KS/KL double-integer arithmetic for the default 71-bit
`long long` model when the operation is tested:

```asm
dadd
dsub
dmul
```

Policy:

| Operation | Codegen status for `-march=ks10` | Notes |
| --- | --- | --- |
| `DADD` | Allowed. | Backend already has `TARGET_71BIT && TARGET_KL10up` patterns. |
| `DSUB` | Allowed. | Backend already has `TARGET_71BIT && TARGET_KL10up` patterns. |
| `DMUL` | Allowed after focused tests. | Backend has `TARGET_71BIT && TARGET_KL10up` patterns. |
| `DDIV` | Disabled until validated. | Backend pattern is currently disabled. |

`DADD`, `DSUB`, `DMUL`, and `DDIV` are not KI10 baseline instructions.  Do not
use them for `-march=ki10`.  A monitor could emulate missing instructions via a
UUO path, but the compiler must not assume such emulation for ordinary generated
code.

The raw 72-bit `long long` mode is a separate compatibility mode selected by
`-mlong-long-72bit`.  KS10 double-integer codegen should be designed and tested
against the default 71-bit model first.

## Floating point

KS10 may use KI10-style double-format floating point when hard-float codegen is
enabled:

```asm
dfad
dfsb
dfmp
dfdv
dmovn
```

Policy:

- Single-format floating point may use the normal PDP-10 floating-point
  operations when the selected target profile permits hard-float.
- Double-format floating point may use `DFAD`, `DFSB`, `DFMP`, and `DFDV` under
  `TARGET_KI10up && !TARGET_GFLOAT`.
- G-float is not part of plain `-march=ks10`.
- PDP-6 and KA10 floating-point behavior is not the KS10 model and must remain
  separate.

DAIMOS V1 should not make floating-point support part of the shared baseline.
Keep FP behavior profile-specific and tested separately.

## EXTEND string and decimal support

`EXTEND` is useful for ordinary C programs only through library and builtin bulk
operations.  It is not needed for scalar C expression selection.

Do not use EXTEND for:

- Integer arithmetic.
- Pointer arithmetic.
- Scalar comparisons.
- Ordinary `char` load and store.
- `long long` arithmetic.
- Address generation.

Potential future use cases:

| C or libc operation | EXTEND use case | Policy |
| --- | --- | --- |
| `memcpy`, `memmove`, `bcopy` | Byte-string moves. | Future libc/builtin optimization only. |
| `memcmp`, `strcmp`, `strncmp` | String comparisons. | Future libc/builtin optimization only. |
| `strcpy`, `strncpy` | String moves with termination rules. | Future libc optimization only. |
| `memset`, `bzero` | Fill loops. | Benchmark before use. |
| `atoi`, `strtol`, `printf`, `scanf` | Decimal conversion helpers. | Future libc helper only. |
| Character translation | Translate/edit helpers. | Future specialized libc helper only. |

The current backend has `TARGET_STRING` hard-disabled.  This is intentional for
DAIMOS bring-up.  If EXTEND string support is added later, use a separate switch
or internal libc build profile such as:

```text
-mextend-string
-mextend-decimal
```

Do not make `-mstring` automatically mean that every available EXTEND operation
is safe.  In particular, filter out extended-address operations such as XBLT.

## XBLT policy

`XBLT` is Extended Block Transfer.  It is the extended-addressing relative of
`BLT`.

For DAIMOS compiler purposes:

```text
BLT  = allowed word block copy in local 18-bit address space
XBLT = not allowed for plain -march=ks10
```

Reasons:

- DAIMOS KS10 support is local-address, not KL10B multi-section support.
- Plain KS10 code must not require extended sections.
- Current backend XBLT selection already requires `TARGET_EXTENDED`.
- Ordinary C aggregate copy works with `BLT` or simple loops.

If a future KL10B or XKL target supports extended sections, that target should
own XBLT.  Do not share that policy with KS10.

## KS10 I/O instructions

KS10 I/O is relevant to the kernel and to inline assembler, not to ordinary C
code generation.

Compiler policy:

- Do not emit KS10 I/O instructions from normal C RTL patterns.
- Permit them only in assembler, builtins explicitly designed for kernel use,
  or target-specific runtime code.
- Do not treat KA10/KI10 device I/O instruction compatibility as a C-codegen
  issue.

## What KS10 lacks for ordinary C

For ordinary generated C, KS10 does not lack a useful feature that PDP-6, KA10,
or KI10 has.  The older machines differ mostly in areas that the compiler must
avoid in a portable C baseline:

| Older-machine area | KS10 relation | Compiler decision |
| --- | --- | --- |
| KA10/PDP-6 external I/O model | Different on KS10. | Kernel/asm only, not normal C. |
| KA10/PDP-6 FP quirks | Not the KS10 FP model. | Keep FP target-specific. |
| PDP-6 floating-point differences | Not portable. | Keep out of common C backend. |
| Pushdown carry behavior | KI/KL/KS behavior differs from KA/PDP-6 details. | Never depend on the carry quirk. |

Therefore the safe rule is: make the portable baseline run everywhere, then add
KS10 optimizations only when they are guarded and tested.

## Recommended compiler predicates

Suggested target predicates and intended use:

| Predicate | True for KS10? | Intended use |
| --- | --- | --- |
| `TARGET_KS10` | Yes. | KS10-specific policy and diagnostics. |
| `TARGET_KI10up` | Yes. | `DMOVE`, double-format FP, KI-up behavior. |
| `TARGET_KL10up` | Yes. | KS/KL local-address arithmetic such as `DADD`. |
| `TARGET_EXTENDED` | No by default. | KL10B/XKL-style extended addressing only. |
| `TARGET_STRING` | No currently. | Future EXTEND string/decimal code only. |
| `TARGET_GFLOAT` | No by default. | Future G-float targets only. |
| `TARGET_71BIT` | Yes by default. | Default `long long` model. |

Do not use `TARGET_KL10up` alone for features that require extended sections.
Use `TARGET_EXTENDED` for those.  KS10 is KL-like for some local-address
instructions, not a KL10B sectioned-address machine.

## Plain `-march=ks10` must allow

```asm
dmove
dmovem
dmovn
dmovnm

dadd
dsub
dmul

; later, after tests:
; ddiv

; if hard-float is enabled:
dfad
dfsb
dfmp
dfdv
```

## Plain `-march=ks10` must not emit

```asm
xmovei
xblt
gfad
gfsb
gfmp
gfdv
```

It also must not use `EXTEND` for generic scalar C codegen.

## Test requirements before enabling more KS10 codegen

Before turning on or expanding a KS10-only instruction pattern, add focused tests
to `pdp10-c-testkit`.

Minimum test groups:

1. Doubleword move tests for register, memory, return, argument, and struct-copy
   pressure.
2. `DADD` and `DSUB` tests for positive, negative, carry, borrow, zero, and
   boundary 71-bit values.
3. `DMUL` tests for signs, zero, one, minus one, high-half pressure, and
   truncation behavior.
4. `DDIV` tests before enabling DDIV: quotient, remainder, signs, divide by one,
   divide by minus one, high-word inputs, and boundary cases.
5. Double-format FP tests only under a hard-float profile.
6. EXTEND string/decimal tests only when the libc/builtin profile is introduced.
7. Negative tests proving `-march=ks10` does not emit `XMOVEI`, `XBLT`, or
   extended-section forms.

For each compiler change, run focused PDP-10 testkit rows first.  Reserve full
matrix sweeps for phase boundaries.

## Final rule

Use this rule for DAIMOS:

```text
pdp10-base:
    Portable PDP-6/KA10/KI10/KS10 C codegen.
    No double-integer arithmetic opcodes.
    No EXTEND.
    No XBLT.
    No extended sections.

-march=ks10:
    pdp10-base
    + DMOVE family
    + DADD/DSUB/DMUL for the default 71-bit long long model
    + DDIV only after validation
    + DF floating point only under a hard-float profile
    + future EXTEND string/decimal only for libc/builtins
    - XBLT
    - XMOVEI and extended sections
    - G-float by default
```

This keeps KS10 useful without making it a fake KL10B target and without
polluting the shared DAIMOS PDP-6/KA10 baseline.
