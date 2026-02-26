# GCC 3.2 PDP-10 Cleanup TODO Before GCC 4.7 Port

Date: 2026-06-19

Scope: clean the existing `gcc-3.2-pdp10` tree so the PDP-10 port becomes a clear, testable reference implementation before moving the backend to GCC 4.7.4.

Primary rule: do not port and clean at the same time. First make GCC 3.2 cleaner while preserving behavior. Then use the cleaned tree as the source for the GCC 4.7 migration.

Secondary rule: do not try to move literally every PDP-10-related line into `gcc/config/pdp10`. Some compiler-wide abstractions are genuinely needed for PDP-10's 9-bit C bytes, 36-bit hardware words, byte pointers, packed fields, and unusual floating formats. The goal is to move PDP-10 policy into `gcc/config/pdp10` and leave only small, documented, target-generic hook sites in the GCC core.

Runtime rule: long validations must be restartable and chunked. Avoid single uninterrupted full sweeps. Use checkpoints and focused rows after each cleanup patch. Reserve full sweeps for phase boundaries.

---

## 0. Current inspection summary

The PDP-10 backend in GCC 3.2 is not isolated. It includes a large normal backend plus many generic GCC changes.

Important PDP-10 backend files:

```text
gcc/config/pdp10/pdp10.c          main backend implementation
gcc/config/pdp10/pdp10.h          target macros, ABI, layout, options
gcc/config/pdp10/pdp10.md         machine description
gcc/config/pdp10/pdp10-protos.h   backend prototypes
gcc/config/pdp10/libgcc1.s        runtime helper assembly
gcc/config/pdp10/gas.h            gas assembler fragment
gcc/config/pdp10/macro.h          MACRO assembler fragment
gcc/config/pdp10/midas.h          MIDAS assembler fragment
gcc/config/pdp10/t-none           bare target fragment
gcc/config/pdp10/t-tops20         TOPS-20 target fragment
gcc/config/pdp10/tops20.h         TOPS-20 target header
gcc/config/pdp10/netbsd.h         NetBSD target header
gcc/config/pdp10/xm-*.h           host/build fragments
gcc/config/pdp10/README           backend notes
```

High-priority generic GCC files containing PDP-10 hooks or PDP-10-adjacent behavior:

```text
gcc/expr.c
gcc/expmed.c
gcc/varasm.c
gcc/function.c
gcc/c-typeck.c
gcc/c-common.c
gcc/builtins.c
gcc/calls.c
gcc/emit-rtl.c
gcc/final.c
gcc/combine.c
gcc/cse.c
gcc/real.c
gcc/real.h
gcc/libgcc2.c
gcc/libgcc2.h
gcc/defaults.h
gcc/doc/tm.texi
```

Crude marker count from inspection, useful only as a triage guide:

```text
64  gcc/expr.c
45  gcc/real.c
21  gcc/varasm.c
18  gcc/expmed.c
18  gcc/doc/tm.texi
15  gcc/c-typeck.c
15  gcc/c-common.c
10  gcc/builtins.c
8   gcc/function.c
5   gcc/dwarf2asm.c
5   gcc/combine.c
4   gcc/defaults.h
4   gcc/calls.c
4   gcc/emit-rtl.c
3   gcc/real.h
3   gcc/c-common.h
2   gcc/toplev.c
2   gcc/optabs.c
2   gcc/cse.c
1   gcc/libgcc2.c
1   gcc/rtl.c
1   gcc/simplify-rtx.c
```

Known PDP-10-specific abstractions/macros seen during inspection:

```text
REAL_BITS_PER_UNIT
REAL_UNITS_PER_WORD
EXTRACT_FIELD_AS_BITFIELD_P
BITS_PER_BITFIELD_UNIT
DATA_PADDING
LOCAL_SIZE
STACK_SLOT_SUBWORD_BITPOS
MD_ARRAY_OFFSET
MD_EXPAND_BINOP
MD_CONVERT_POINTER
MD_SIMPLIFY_UNSPEC
MD_EXPAND_ADDR_EXP
MD_COMPARE_AND_JUMP
TARGET_FLOAT_FORMAT
```

Core target model to preserve:

```text
C byte/unit size:       9 bits
Hardware word size:    36 bits
C units per word:      4
Real memory word unit: 36 bits
PDP-6/KA10 baseline:   required
Native self-build:     should remain practical on 256KW class machines
```

---

## 1. Desired final shape of the cleaned GCC 3.2 tree

The cleaned tree should have this structure conceptually:

```text
gcc/config/pdp10/
    pdp10.c             all PDP-10 policy and special expansion logic
    pdp10.h             macro definitions, hook bindings, layout constants
    pdp10.md            RTL patterns
    pdp10-protos.h      prototypes
    libgcc1.s           PDP-10 helper routines
    gas.h               assembler flavor fragment
    macro.h             assembler flavor fragment
    midas.h             assembler flavor fragment
    t-*                 target make fragments

GCC generic files:
    expr.c              tiny generic hook call sites only
    expmed.c            tiny generic hook call sites only
    varasm.c            tiny generic hook call sites only
    function.c          tiny generic hook call sites only
    c-typeck.c          tiny generic hook call sites only
    c-common.c          tiny generic hook call sites only
    builtins.c          tiny generic hook call sites only
    real.c/real.h       minimal float-format registration only
    defaults.h          default definitions for PDP-10-needed abstractions
    doc/tm.texi         documentation for intentional generic abstractions
```

Acceptable generic code:

```c
#ifndef REAL_BITS_PER_UNIT
#define REAL_BITS_PER_UNIT BITS_PER_UNIT
#endif
```

```c
#ifdef MD_EXPAND_ADDR_EXP
  rtx tmp = MD_EXPAND_ADDR_EXP (...);
  if (tmp)
    return tmp;
#endif
```

Bad final shape:

```c
#ifdef PDP10
  /* 80 lines of PDP-10 byte-pointer policy directly inside expr.c. */
#endif
```

The cleaned GCC 3.2 tree should be usable as a documented local fork, not necessarily as an upstreamable patch.

---

## 2. Phase A: freeze baseline and make validation reproducible

Goal: create an exact behavioral baseline before refactoring.

### A1. Preserve baseline source

- [ ] Create read-only or archived baseline from uploaded `gcc-3.2-pdp10.tar.gz`.
- [ ] Record source hash of the archive.
- [ ] Record extraction path.
- [ ] Record host compiler, host OS, and build command.
- [ ] Confirm whether `build.sh` is the canonical build entry point.
- [ ] Record whether the build currently succeeds with host `gcc`.
- [ ] Record whether the resulting PDP-10 GCC can compile at least one trivial C file.

Suggested artifact:

```text
docs/pdp10-cleanup/BASELINE.md
```

### A2. Capture baseline backend inventory

- [ ] Count lines in `gcc/config/pdp10/*`.
- [ ] Count `define_insn`, `define_expand`, `define_peephole`, `define_attr` in `pdp10.md`.
- [ ] List all target triplets currently handled in `gcc/config.gcc`.
- [ ] List all assembler flavor fragments.
- [ ] List all `#define` feature switches in `pdp10.h`.
- [ ] List all externally visible backend functions in `pdp10-protos.h`.

Suggested artifact:

```text
docs/pdp10-cleanup/BACKEND_INVENTORY.md
```

### A3. Capture generic edit inventory

For every generic GCC file touched by PDP-10 logic, record:

```text
file
function or section
marker or macro name
purpose
can move into pdp10.c? yes/no
needs generic hook site? yes/no
needs compiler-wide abstraction? yes/no
covered by test? yes/no
status
```

Suggested artifact:

```text
docs/pdp10-cleanup/GENERIC_EDIT_INVENTORY.tsv
```

Initial files to classify:

```text
gcc/expr.c
gcc/expmed.c
gcc/varasm.c
gcc/function.c
gcc/c-typeck.c
gcc/c-common.c
gcc/builtins.c
gcc/calls.c
gcc/emit-rtl.c
gcc/final.c
gcc/combine.c
gcc/cse.c
gcc/real.c
gcc/real.h
gcc/libgcc2.c
gcc/libgcc2.h
gcc/defaults.h
gcc/doc/tm.texi
```

### A4. Establish focused smoke tests

Do not begin source cleanup until these run in small chunks:

- [ ] trivial integer add/return
- [ ] global int load/store
- [ ] local int load/store
- [ ] char global load/store
- [ ] char local load/store
- [ ] char array indexing
- [ ] short global load/store
- [ ] short local load/store
- [ ] struct with char fields
- [ ] struct with short fields
- [ ] packed struct field access
- [ ] pointer arithmetic on char pointer
- [ ] pointer arithmetic on int pointer
- [ ] string literal initialization
- [ ] memcpy-like struct copy
- [ ] function call with char argument
- [ ] function call with short argument
- [ ] function return char
- [ ] function return short
- [ ] simple switch

Use the existing `pdp10-c-testkit` where possible. If a test does not exist, add a small C89 test.

### A5. Create a checkpoint runner

- [ ] Add or adapt a runner that accepts explicit row list.
- [ ] Add chunking by compiler profile and machine.
- [ ] Ensure each row writes a result file atomically.
- [ ] Ensure interrupted runs can resume without deleting completed rows.
- [ ] Ensure focused runs can execute only changed-risk rows after each patch.

Required target machines for cleanup checks:

```text
pdp6
ka10
```

Optional later:

```text
ki10
pdp10 family/base profiles
```

---

## 3. Phase B: classify and document generic abstractions

Goal: decide what may remain in the GCC core and what must move to `gcc/config/pdp10`.

### B1. Define categories

Each generic edit must be assigned one category:

```text
backend-policy
    PDP-10-specific logic that should move to pdp10.c.

generic-hook-site
    Small target-call site in generic GCC. Logic lives in target backend.

compiler-abstraction
    Genuine compiler-wide abstraction with normal default value.
    Example: REAL_BITS_PER_UNIT.

float-format
    Floating-format support. GCC 4.7 migration will probably use modes.def.

runtime-libgcc
    Runtime helper behavior or libgcc support.

debug-output
    DBX/DWARF/TOPS20 debug-format support.

build-system
    configure, config.gcc, make fragment, host/target triplet handling.

dead-or-suspect
    Old code that may no longer be needed. Remove only after focused tests.
```

### B2. Document compiler-wide abstractions

For each abstraction, write:

```text
name
normal default
PDP-10 value
why generic GCC needs it
which files use it
which tests cover it
whether GCC 4.7 also needs it
```

Initial abstraction list:

```text
REAL_BITS_PER_UNIT
REAL_UNITS_PER_WORD
BITS_PER_BITFIELD_UNIT
EXTRACT_FIELD_AS_BITFIELD_P
DATA_PADDING
LOCAL_SIZE
STACK_SLOT_SUBWORD_BITPOS
```

Important note: `REAL_BITS_PER_UNIT` and `REAL_UNITS_PER_WORD` should not be treated as cosmetic. They express the split between 9-bit C units and 36-bit memory words.

### B3. Document target hook sites

For every `MD_*` hook, write a contract:

```text
hook name
called from which generic function
input invariants
output meaning
returns null/zero when target does not handle it?
side effects allowed?
PDP-10 implementation function
minimal test
```

Initial hook list:

```text
MD_ARRAY_OFFSET
MD_EXPAND_BINOP
MD_CONVERT_POINTER
MD_SIMPLIFY_UNSPEC
MD_EXPAND_ADDR_EXP
MD_COMPARE_AND_JUMP
```

### B4. Add documentation location

- [ ] Create `gcc/config/pdp10/PORTING-NOTES.md` or `README.pdp10-cleanup`.
- [ ] Keep short generic-abstraction comments near definitions in `defaults.h` or relevant generic file.
- [ ] Avoid large prose blocks in generic GCC C files.

---

## 4. Phase C: move backend-policy code into `pdp10.c`

Goal: remove large PDP-10 blocks from generic files without changing behavior.

For each task below:

1. identify the existing generic code block,
2. extract logic into a `pdp10_*` function,
3. add prototype to `pdp10-protos.h`,
4. bind it through a small macro in `pdp10.h`,
5. leave only a small generic hook site,
6. run focused tests before continuing.

### C1. `expr.c` cleanup

Likely responsibility:

- address expansion
- array offset special handling
- pointer conversion handling
- component reference handling
- memory reference construction
- byte-pointer arithmetic support
- subword load/store expansion

Tasks:

- [ ] Identify every PDP-10-related block in `expr.c`.
- [ ] Split blocks by purpose.
- [ ] Move address expression policy into `pdp10_expand_addr_exp` or equivalent.
- [ ] Move array offset policy into `pdp10_array_offset` or equivalent.
- [ ] Move pointer conversion policy into `pdp10_convert_pointer` or equivalent.
- [ ] Move target-specific binary operation policy into `pdp10_expand_binop` or equivalent.
- [ ] Keep only hook calls in `expr.c`.
- [ ] Add comments explaining why generic expansion asks the target first/last.

Focused tests:

- [ ] char pointer increment
- [ ] char pointer plus constant
- [ ] char pointer plus variable
- [ ] int pointer plus constant
- [ ] struct member address
- [ ] array member address
- [ ] packed field address if allowed
- [ ] local char load/store
- [ ] global char load/store
- [ ] nested char array indexing

### C2. `expmed.c` cleanup

Likely responsibility:

- bitfield extraction
- bitfield insertion
- subword extraction
- packed fields
- byte-in-word positioning

Tasks:

- [ ] Identify PDP-10 special cases for extraction/insertion.
- [ ] Decide which are generic abstractions and which are backend policy.
- [ ] Keep `BITS_PER_BITFIELD_UNIT` defaulted generically if still needed.
- [ ] Keep `EXTRACT_FIELD_AS_BITFIELD_P` defaulted generically if still needed.
- [ ] Move PDP-10 policy into target functions where possible.
- [ ] Document bit numbering expectations for PDP-10.

Focused tests:

- [ ] unsigned bitfield load
- [ ] signed bitfield load
- [ ] bitfield store
- [ ] char field in struct
- [ ] short field in struct
- [ ] packed char field
- [ ] packed short field
- [ ] adjacent packed fields
- [ ] volatile packed field if supported

### C3. `varasm.c` cleanup

Likely responsibility:

- global initializer emission
- string literal packing
- constructor emission
- constants in 36-bit words
- section/data padding

Tasks:

- [ ] Identify all PDP-10-specific data emission logic.
- [ ] Move target-specific packing policy into `pdp10_asm_*` or `pdp10_data_*` helpers.
- [ ] Keep `DATA_PADDING` as small documented hook if needed.
- [ ] Ensure string literal emission remains byte-correct.
- [ ] Ensure initialized char arrays and packed structs remain byte-correct.
- [ ] Check assembler flavor differences: gas, MACRO, MIDAS.

Focused tests:

- [ ] global `char s[] = "abc"`
- [ ] global `char s[5] = "abc"`
- [ ] global char array with non-printable values
- [ ] global short array
- [ ] initialized struct with char fields
- [ ] initialized packed struct
- [ ] mixed int/char global initializer
- [ ] const string pointer

### C4. `function.c` cleanup

Likely responsibility:

- stack slot sizes
- subword local layout
- local variable padding
- frame layout
- temporary allocation

Tasks:

- [ ] Identify use of `LOCAL_SIZE` and `STACK_SLOT_SUBWORD_BITPOS`.
- [ ] Ensure defaults are generic and harmless.
- [ ] Move PDP-10 stack slot calculations into backend functions where practical.
- [ ] Document PDP-10 local layout for char/short objects.
- [ ] Verify no persistent gaps are introduced for non-paged PDP-6/KA10 machines.

Focused tests:

- [ ] local char variables adjacent
- [ ] local short variables adjacent
- [ ] local char array
- [ ] local struct with char fields
- [ ] local packed struct
- [ ] function call preserves local subword values
- [ ] recursion with subword locals

### C5. `c-typeck.c` and `c-common.c` cleanup

Likely responsibility:

- type conversions
- pointer conversions
- char/short promotions
- warnings or semantic checks affected by PDP-10 byte pointers

Tasks:

- [ ] Identify all PDP-10-related type checking changes.
- [ ] Separate true C semantics from code-generation workaround.
- [ ] Move codegen policy out of C frontend where possible.
- [ ] Keep only required target semantic hooks.
- [ ] Ensure host-independent C89 behavior remains unchanged for normal targets.

Focused tests:

- [ ] char pointer conversion
- [ ] void pointer conversion
- [ ] struct pointer conversion
- [ ] integer-to-pointer cast
- [ ] pointer-to-integer cast
- [ ] comparison of different pointer types
- [ ] function prototype with char/short args

### C6. `builtins.c` cleanup

Likely responsibility:

- memcpy/memset/memcmp/string operation lowering
- builtin expansion involving byte units

Tasks:

- [ ] Identify PDP-10-specific builtin handling.
- [ ] Move target-specific builtin expansion into `pdp10_expand_builtin` helpers where possible.
- [ ] Ensure normal targets remain unaffected.
- [ ] Decide which builtins should be disabled or left as libc calls for DAIMON bare target.

Focused tests:

- [ ] struct assignment lowered correctly
- [ ] small memcpy equivalent
- [ ] char array copy
- [ ] zero initialization
- [ ] memset-like clearing of char array
- [ ] compare generated assembly for helper calls versus inline expansion

### C7. `calls.c` cleanup

Likely responsibility:

- function call ABI corner cases
- argument passing of subword objects
- return value handling

Tasks:

- [ ] Identify PDP-10 call ABI edits.
- [ ] Move target-specific decisions into backend ABI helpers.
- [ ] Keep only tiny call sites if generic support is unavoidable.
- [ ] Document char/short argument extension and placement.
- [ ] Document struct return behavior.

Focused tests:

- [ ] call with char arg
- [ ] call with short arg
- [ ] call with mixed int/char/short args
- [ ] call with pointer args
- [ ] call with struct arg if supported
- [ ] return char
- [ ] return short
- [ ] return small struct
- [ ] nested calls with subword args

### C8. `emit-rtl.c`, `combine.c`, `cse.c`, `simplify-rtx.c`, `optabs.c` cleanup

Likely responsibility:

- special RTL simplification
- target-specific unspec simplification
- combine/cse safety with PDP-10 byte pointers
- special optab behavior

Tasks:

- [ ] Identify all PDP-10-related optimizer/RTL changes.
- [ ] Classify each as required or suspect.
- [ ] Move simplification policy into `MD_SIMPLIFY_UNSPEC` or similar where possible.
- [ ] Add comments for any optimizer barrier needed for byte pointers.
- [ ] Avoid optimizer hacks without tests.

Focused tests:

- [ ] run affected rows at `-O0`
- [ ] run affected rows at `-O1`
- [ ] run affected rows at `-O2`
- [ ] run affected rows at `-Os`
- [ ] include byte pointer postincrement rows
- [ ] include packed field side-effect rows

### C9. `final.c`, debug output, and assembly output cleanup

Likely responsibility:

- final instruction output
- debug labels
- assembler compatibility
- TOPS-20 debug output

Tasks:

- [ ] Identify PDP-10 final-output changes.
- [ ] Move backend-specific output decisions into `pdp10.c` where possible.
- [ ] Separate DAIMON/bare target from TOPS-20/ITS/NetBSD legacy output.
- [ ] Decide whether TOPS-20 debug support remains in cleanup scope.
- [ ] Do not let debug-output cleanup block core C compiler cleanup.

Focused tests:

- [ ] compile simple file to assembly with debug disabled
- [ ] compile simple file to assembly with `-g` if supported
- [ ] verify assembler accepts output
- [ ] verify labels and global symbols are stable

---

## 5. Phase D: rationalize floating point support

Goal: isolate PDP-10 floating-point logic so GCC 4.7 migration is tractable.

### D1. Inventory current FP behavior

- [ ] Locate all `TARGET_FLOAT_FORMAT` uses.
- [ ] Locate PDP-10-specific changes in `real.c` and `real.h`.
- [ ] Identify supported PDP-10 FP formats.
- [ ] Identify whether PDP-6/KA10 target profile should support hardware FP, software FP, or no FP at first.
- [ ] Identify libgcc FP helper dependencies.

### D2. Decide cleanup target for GCC 3.2

Options:

```text
Option 1: preserve existing FP exactly, only document it.
Option 2: isolate FP better but do not redesign it.
Option 3: disable/stub FP for bare DAIMON target and leave legacy OS targets alone.
```

Recommendation: choose Option 2 for GCC 3.2 cleanup. Do not redesign FP until GCC 4.7, where `pdp10-modes.def` or equivalent will be needed.

### D3. Add FP tests separately

Keep FP tests separate from integer/byte-pointer tests.

Focused tests:

- [ ] compile float constant
- [ ] compile double constant
- [ ] float add if supported
- [ ] double add if supported
- [ ] float compare if supported
- [ ] cast int to float if supported
- [ ] cast float to int if supported
- [ ] function call with float arg if supported
- [ ] return float if supported

Important: do not let FP failures block char/short/byte-pointer cleanup unless the touched code directly affects FP.

---

## 6. Phase E: target option and target profile cleanup

Goal: simplify the old GCC 3.2 target surface before GCC 4.7.

### E1. Inventory existing target triplets

- [ ] List all PDP-10 target triplets in `gcc/config.gcc`.
- [ ] Identify which are actually needed.
- [ ] Identify which depend on old host/OS assumptions.
- [ ] Identify which have working assembler/runtime support.

Likely current legacy targets:

```text
pdp10-*-none
pdp10-*-tops20
pdp10-*-its
pdp10-*-netbsd
pdp10-*-linux
```

Recommended DAIMON cleanup focus:

```text
pdp10-*-none
```

Optional future target:

```text
pdp10-*-daimos
```

### E2. Keep legacy targets from contaminating the cleanup

- [ ] Avoid deleting legacy target fragments in the first cleanup pass.
- [ ] Mark unsupported/unvalidated legacy targets in documentation.
- [ ] Keep DAIMON/bare build path simple.
- [ ] Prefer one assembler flavor for validation first, probably GAS if that is what the simulator/testkit path uses.

### E3. Inventory target flags

- [ ] List `TARGET_SWITCHES` and `TARGET_OPTIONS` in `pdp10.h`.
- [ ] Classify each as required, legacy, debug-only, or unknown.
- [ ] Record default CPU profile.
- [ ] Record PDP-6 and KA10 option differences.
- [ ] Record optimizer-sensitive options.

This phase prepares for GCC 4.7 `pdp10.opt` conversion later.

---

## 7. Phase F: libgcc and runtime helper cleanup

Goal: make runtime helper dependencies explicit.

### F1. Inventory helper routines

- [ ] List all labels/functions in `gcc/config/pdp10/libgcc1.s`.
- [ ] Match each helper to compiler-generated calls.
- [ ] Identify integer helpers.
- [ ] Identify shift helpers.
- [ ] Identify multiply/divide helpers.
- [ ] Identify comparison helpers.
- [ ] Identify FP helpers.
- [ ] Identify abort/trap dependencies.

Suggested artifact:

```text
docs/pdp10-cleanup/LIBGCC_HELPERS.tsv
```

Columns:

```text
helper_name
source_file
called_by_backend
called_by_generic_gcc
integer/fp/memory/control
required_for_daimon_v1
test_name
status
```

### F2. Add helper-call tests

- [ ] force 36-bit multiply helper if applicable
- [ ] force 36-bit divide helper if applicable
- [ ] force wide shift helper if applicable
- [ ] force comparison helper if applicable
- [ ] force struct copy helper if applicable
- [ ] force FP helper only in FP test group

### F3. Prepare for GCC 4.7 libgcc split

GCC 4.7 has a different libgcc layout than GCC 3.2. For now, record what needs to move later:

```text
GCC 3.2: gcc/config/pdp10/libgcc1.s
GCC 4.7: likely libgcc/config/pdp10 plus config.host entries
```

Do not perform this migration during GCC 3.2 cleanup.

---

## 8. Phase G: tests that must exist before GCC 4.7 work begins

Goal: make the cleaned GCC 3.2 tree a behavioral oracle.

Every cleanup should preserve or improve these tests on PDP-6 and KA10.

### G1. Byte and char tests

- [ ] char global load/store
- [ ] char local load/store
- [ ] char array sequential access
- [ ] char array indexed by variable
- [ ] char pointer postincrement
- [ ] char pointer preincrement
- [ ] char pointer add constant
- [ ] char pointer add variable
- [ ] char pointer subtraction if supported
- [ ] char function argument
- [ ] char function return
- [ ] signed char compare
- [ ] unsigned char compare
- [ ] char in struct
- [ ] char in packed struct
- [ ] string literal load
- [ ] string literal initialization

### G2. Short/subword tests

- [ ] short global load/store
- [ ] short local load/store
- [ ] short array indexing
- [ ] short function argument
- [ ] short function return
- [ ] signed short compare
- [ ] unsigned short compare
- [ ] short in struct
- [ ] short in packed struct
- [ ] mixed char/short struct
- [ ] mixed char/short/int struct

### G3. Packed struct and bitfield tests

- [ ] unsigned bitfield load
- [ ] signed bitfield load
- [ ] bitfield store
- [ ] adjacent bitfields
- [ ] bitfield with char neighbor
- [ ] bitfield with short neighbor
- [ ] packed struct assignment
- [ ] packed struct passed to function if supported
- [ ] packed struct returned from function if supported
- [ ] side effects in packed field index expression

### G4. ABI/call tests

- [ ] mixed int/char args
- [ ] mixed int/short args
- [ ] mixed pointer/char/short args
- [ ] many arguments crossing register/stack boundary
- [ ] nested calls with subword args
- [ ] recursive function with subword locals
- [ ] function pointer call
- [ ] small return values
- [ ] struct return path
- [ ] varargs if supported

### G5. Control-flow tests

- [ ] if/else on char compare
- [ ] if/else on short compare
- [ ] switch on int
- [ ] switch on char
- [ ] switch on short
- [ ] nested switch
- [ ] switch with packed-field side effect
- [ ] loop over char pointer
- [ ] loop over short pointer

### G6. Data layout tests

- [ ] initialized char array
- [ ] initialized short array
- [ ] initialized mixed struct
- [ ] initialized packed struct
- [ ] zero-initialized global data
- [ ] const data
- [ ] string table
- [ ] array of structs with char fields
- [ ] array of packed structs

### G7. Optimizer matrix

For each cleanup phase boundary, run focused rows under:

```text
-O0
-O1
-O2
-Os
```

For each:

```text
pdp6
ka10
```

Do not run full sweeps after every patch. Use focused rows after each patch and full sweep only after phase completion.

---

## 9. Phase H: cleanup completion criteria

The GCC 3.2 cleanup is complete when all of these are true:

- [ ] All large PDP-10 logic blocks are moved out of generic GCC files where practical.
- [ ] Remaining generic edits are documented as hook sites or compiler abstractions.
- [ ] `gcc/config/pdp10/PORTING-NOTES.md` exists.
- [ ] `docs/pdp10-cleanup/GENERIC_EDIT_INVENTORY.tsv` exists and is complete.
- [ ] `docs/pdp10-cleanup/LIBGCC_HELPERS.tsv` exists and is complete.
- [ ] Focused smoke tests pass on PDP-6 and KA10.
- [ ] Full cleaned GCC 3.2 focused matrix does not regress against baseline.
- [ ] Known failures are explicitly listed and classified.
- [ ] The tree can still build from a clean checkout.
- [ ] The build process is documented.
- [ ] A source patch/tarball of changed files only is produced.

Recommended artifact names:

```text
gcc32-pdp10-cleanup-changed-files-YYYYMMDD.tar.gz
gcc32-pdp10-cleanup-results-YYYYMMDD.tar.gz
GCC32_PDP10_CLEANUP_REPORT_YYYYMMDD.md
```

Per user preference, downloadable source update archives should contain only changed replacement files, not a full tree.

---

## 10. GCC 4.7 migration preparation checklist

Do not begin GCC 4.7 migration until Phase H is complete or consciously waived.

### I1. Backend files to copy first

```text
gcc/config/pdp10/pdp10.c
gcc/config/pdp10/pdp10.h
gcc/config/pdp10/pdp10.md
gcc/config/pdp10/pdp10-protos.h
gcc/config/pdp10/gas.h
gcc/config/pdp10/t-none
```

### I2. New GCC 4.7 files likely needed

```text
gcc/config/pdp10/pdp10.opt
gcc/config/pdp10/predicates.md
gcc/config/pdp10/constraints.md
gcc/config/pdp10/pdp10-modes.def
libgcc/config/pdp10/*
```

### I3. GCC 3.2 macros requiring GCC 4.7 conversion

Known poisoned/replaced macro families in GCC 4.7 include:

```text
TARGET_SWITCHES
TARGET_OPTIONS
TARGET_OPTION_TRANSLATE_TABLE
CPP_PREDEFINES
TARGET_VERSION
OVERRIDE_OPTIONS
FUNCTION_ARG
FUNCTION_ARG_ADVANCE
FUNCTION_ARG_PARTIAL_NREGS
RETURN_POPS_ARGS
RETURN_IN_MEMORY
SETUP_INCOMING_VARARGS
BUILD_VA_LIST_TYPE
EXPAND_BUILTIN_VA_ARG
EXPAND_BUILTIN_VA_START
PROMOTE_FUNCTION_ARGS
PROMOTE_FUNCTION_RETURN
PROMOTE_PROTOTYPES
CONDITIONAL_REGISTER_USAGE
CAN_ELIMINATE
FRAME_POINTER_REQUIRED
LEGITIMIZE_ADDRESS
ENCODE_SECTION_INFO
CONST_COSTS
RTX_COSTS
ADDRESS_COST
MACHINE_DEPENDENT_REORG
PREDICATE_CODES
MAX_INTEGER_COMPUTATION_MODE
UNIQUE_SECTION
EXTRA_SECTIONS
EXTRA_SECTION_FUNCTIONS
READONLY_DATA_SECTION
ASM_FILE_START
ASM_FILE_END
ASM_GLOBALIZE_LABEL
ASM_OUTPUT_MI_THUNK
TRAMPOLINE_TEMPLATE
INITIALIZE_TRAMPOLINE
LEGITIMATE_CONSTANT_P
```

Expected GCC 4.7 replacements:

```text
TARGET_SWITCHES/TARGET_OPTIONS        -> pdp10.opt plus target option hooks
CPP_PREDEFINES                        -> TARGET_CPU_CPP_BUILTINS
FUNCTION_ARG family                   -> targetm.calls hooks
return value macros                   -> TARGET_FUNCTION_VALUE etc.
varargs macros                        -> GCC 4.7 varargs target hooks
PREDICATE_CODES                       -> predicates.md
old constraints                       -> constraints.md where practical
TARGET_FLOAT_FORMAT                   -> pdp10-modes.def / real_format setup
```

### I4. GCC 4.7 generic hook-site reapplication

After cleanup, the list of required generic hook sites should be known. Reapply only those to GCC 4.7.

Expected areas:

```text
expr.c
expmed.c
varasm.c
function.c
c-typeck.c
c-common.c
builtins.c
calls.c
emit-rtl.c
combine.c/cse.c/simplify-rtx.c if still needed
real.c/real.h or modes/real-format files
```

### I5. GCC 4.7 bring-up order

1. Build `cc1` for `pdp10-unknown-none` or `pdp10-dec-none`.
2. Compile trivial int function to RTL/assembly.
3. Assemble trivial output.
4. Add integer libgcc helpers.
5. Enable char/short tests.
6. Enable pointer/byte-pointer tests.
7. Enable packed struct tests.
8. Enable ABI tests.
9. Enable optimizer profiles.
10. Enable or explicitly defer FP.

Do not attempt full `pdp10-c-testkit` sweeps until the focused rows are green.

---

## 11. Recommended immediate next work items

Start here.

### Work item 1: create cleanup documentation directory

- [ ] Add `docs/pdp10-cleanup/`.
- [ ] Add `BASELINE.md`.
- [ ] Add `BACKEND_INVENTORY.md`.
- [ ] Add empty `GENERIC_EDIT_INVENTORY.tsv` with headers.
- [ ] Add empty `LIBGCC_HELPERS.tsv` with headers.

### Work item 2: generate inventory automatically

- [ ] Write `tools/pdp10-inventory.sh` or similar.
- [ ] It should list marker hits in generic files.
- [ ] It should list backend functions/macros.
- [ ] It should count MD patterns.
- [ ] It should not require modern GNU-only features if the script is meant to run on old systems; but it may be host-only tooling if documented.

### Work item 3: classify `expr.c`

- [ ] Manually inspect every PDP-10 marker in `gcc/expr.c`.
- [ ] Fill `GENERIC_EDIT_INVENTORY.tsv` for `expr.c`.
- [ ] Identify extraction candidates.
- [ ] Add focused tests for each behavior before moving code.

### Work item 4: extract one small hook as proof of method

Choose a small, low-risk extraction before touching the hardest code.

Good candidates:

```text
MD_SIMPLIFY_UNSPEC
MD_COMPARE_AND_JUMP
DATA_PADDING if currently simple
```

Avoid starting with `expr.c` byte-pointer address expansion unless tests are already in place.

### Work item 5: run focused validation

After the proof extraction:

- [ ] build compiler
- [ ] run trivial smoke tests
- [ ] run affected focused rows on PDP-6 and KA10
- [ ] compare assembly if semantic execution is not available
- [ ] record result in `docs/pdp10-cleanup/RESULTS.md`

---

## 12. Do-not-do list

- [ ] Do not begin GCC 4.7 port before GCC 3.2 generic edits are inventoried.
- [ ] Do not remove a generic PDP-10 abstraction just because it looks ugly.
- [ ] Do not move compiler-wide layout concepts blindly into `pdp10.c` if generic code needs them.
- [ ] Do not run full sweeps after every small cleanup patch.
- [ ] Do not trust compile-only success for byte/char/packed-field changes.
- [ ] Do not mix floating-point cleanup with byte-pointer cleanup.
- [ ] Do not let TOPS-20/ITS/NetBSD legacy target support complicate the DAIMON bare target path.
- [ ] Do not replace KCC as oracle. KCC remains the correctness baseline.
- [ ] Do not create source update archives containing full trees; use changed replacement files only.

---

## 13. Success definition

This project succeeds if the cleaned GCC 3.2 PDP-10 tree becomes a stable, documented reference with:

```text
1. PDP-10 policy centralized in gcc/config/pdp10.
2. Remaining generic compiler changes small and documented.
3. Focused regression tests covering every retained abstraction/hook.
4. No regression against the current working GCC 3.2 PDP-10 behavior.
5. A clear, finite list of generic hook sites to reapply to GCC 4.7.
6. A clear list of GCC 4.7 target API migrations.
7. KCC-green testkit retained as semantic oracle.
```

Only after that should the GCC 4.7 port start.
