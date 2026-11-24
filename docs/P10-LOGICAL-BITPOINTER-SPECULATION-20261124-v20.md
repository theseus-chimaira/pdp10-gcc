# PDP-10 logical-bit-pointer speculation repair v20

Date: 2026-11-24

## Root cause: opaque untyped ADJBP on the wrong branch

The full `pdp10-testkit` byte-pointer runtime oracle contains
`short16 *p = q - 9`. The backend accepts both native S=16 pointers
and exact packed S=1 pointers for that same C type. Its dynamic-pointer
expansion tests the runtime S field, then branches to one of two
representations. The S=1 branch previously emitted an opaque raw
`UNSPEC_ADJBP` with a scaled offset of -144 bits. On non-KL10 targets
this matched an assembler pattern containing its own reverse-IBP
search loop, all hidden inside one RTL SET.

At GCC O1/O2/Os, the compiler could speculate that apparently pure
SET ahead of the S=1/S=16 branch. On a native S=16 pointer, the
reverse search cannot find its original pointer after subtracting a
word, and the simulator repeatedly executed the loop at PDP-6 octal
PC 005127..005132 until the 100-million-instruction step limit.
One observed native pointer at the loop had S=16, P=4; treating it as
the S=1 logical pointer was incorrect, not merely inefficient.

## Correct expansion

On machines without native ADJBP, the S=1 branch now emits typed
one-bit pointer arithmetic through existing RTL expansion machinery:

* A constant offset that fits the host int uses `maybe_expand_ibp`
  with byte size 1. Full 36-bit word steps adjust only the pointer's
  right half and preserve P/S, with at most 35 explicit IBP steps.
* Dynamic offsets, or constants too large for the helper's host-int
  argument, use `expand_adjust_byte_pointer_size (1, ...)`. Its real
  RTL branches and loops are visible to GCC's optimizer rather than
  being hidden in an opaque assembly instruction.
* A zero offset explicitly copies the original pointer to the chosen
  destination. Native KS10/KL10 ADJBP retains its hardware path.

The existing v18 fix already guarantees non-overlapping output and
input registers before branching. No additional scratch register or
runtime-library helper was introduced for constant full-word offsets.

Implementation: `gcc/config/pdp10/pdp10.c`, function
`pdp10_expand_addbp3`. The raw untyped ADJBP fallback remains in the
machine description for genuinely untyped callers; the typed S=1
branch no longer depends on it on pre-KL10 targets.

## Verified result and boundary

After installing rebuilt GCC into `$HOME/cross`, the official isolated
testkit runner executed the full 36-cell
`runtime-byte-pointer-diff.c` GCC matrix:

| Status | Before v20 | After v20 |
| --- | ---: | ---: |
| PASS | 9 | 33 |
| run_fail | 11 | 3 |
| timeout | 16 | 0 |

The three remaining `gcc-ks10-o1/o2/os` runtime failures are unrelated
loads: signed 9-bit element read at octal 0245 and 18-bit element read
at octal 0344. They remain failures, not accepted results.

The new C89 source
`pdp10-short16-reverse-adjust-v20.c` exercises constant -9 pointer
subtraction, 16-bit signed/unsigned loads and dynamic forward/reverse
arithmetic. It passed all 36 registered GCC profile cells.

The six earlier v17-v19 division and pointer regressions were also
rerun from scratch against the v20 compiler: **216/216 PASS**.
These tests establish the bounded v20 repair; they do not establish
a complete compiler matrix pass.
