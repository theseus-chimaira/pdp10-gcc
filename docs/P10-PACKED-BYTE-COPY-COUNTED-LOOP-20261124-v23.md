# PDP-10 packed by-value copy loop termination repair v23

Date: 2026-11-24

## Root cause

The testkit `gnu-packed-byvalue.c` previously recorded 27 GCC
simulator timeouts and 17 immediate runtime failures among 44 GCC
profiles. A reduced C89 test retaining only a three-byte packed
struct passed the first field check, then hung in the copy of a
by-value call argument at O1/O2/Os.

The backend's `expand_byte_copy` built the loop tail as two separate
RTL instructions: `count = count - 1`, then a conditional backward
jump if `count != 0`. The initial GCC RTL dump for the failing
argument copy showed the count decrement moved into the preheader
ahead of a libcall sequence, while the backward edge tested the
unchanged count. The resulting LDB/DPB/IBP loop was unbounded.

## Correction

Use the existing `decrement_and_branch_until_zero` machine pattern
for the byte-copy loop tail. The parallel RTL updates the count and
branches on the **updated** value as an indivisible operation; the
PDP-10 backend emits a single `SOJN` instruction. This is both
smaller and faster than a separate subtract and branch, uses no
additional scratch register and preserves the counted-copy contract
even in a compiler libcall/argument-copy context.

Source: `gcc/config/pdp10/pdp10.c`, `expand_byte_copy`.

## Verification and remaining bug

A reduced source previously timed out at O1/O2/Os on PDP-6; after
rebuilding GCC it terminates at each optimization level. The full
`gnu-packed-byvalue.c` source was then rerun with the installed
compiler against the official 44-cell GCC matrix:

| Status | Before v23 | After v23 |
| --- | ---: | ---: |
| PASS | 0 | 0 |
| run_fail | 17 | 44 |
| timeout | 27 | 0 |

The failed assertions are **not** counted as passing. They expose
an independent packed-argument ABI/field extraction bug: `sum3`
can treat a packed word as a scalar instead of extracting its
three nine-bit fields. The v23 fix is limited to loop termination.

The permanent C89 testkit source
`pdp10-packed-argument-copy-loop-v23.c` independently calls functions
with 3, 6, 11 and 20-byte packed by-value arguments and checks that
each call returns. It deliberately does not check their field values
while the ABI issue remains open. It passes **36/36** GCC cells under
strict exit policy, with generated `SOJN` loops checked on PDP-6.

The five source groups from the preceding v19-v22 repairs were
rerun independently against installed v23 GCC: **284/284 GCC/KCC
PASS**, strict exit policy, with no previous successes skipped.
