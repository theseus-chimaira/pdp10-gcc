# PDP-10 unsigned SI division and remainder repair v17

Date: 2026-11-24

## Defect

GCC's PDP-6/KA10/KI10/KS10 unsigned `SImode` division patterns used
`SETZ` on the high accumulator and copied the entire 36-bit numerator
into the low accumulator before executing `DIV`.

PDP-10 `DIV` accepts a 70-bit dividend: its second accumulator has
only 35 payload bits. Numerator bit 35 was discarded. In the original
testkit, all 108 cells for three unsigned division/remainder reducers
reported a wrong answer for high-bit unsigned operands. The isolated
`udivsi3` reducer first failed at octal test ID `0201`, namely
`0400000000000 / 2`.

## Correct backend lowering

For a positive divisor, produce the AC pair in this order:

```
MOVE low,numerator
MOVE high,numerator
LSH high,-43
TLZ low,0400000
DIV high,divisor
```

`-43` is an octal shift count corresponding to a right shift of 35.
Thus `high` becomes numerator bit 35, and the low accumulator holds
the remaining 35 bits. This uses the same two result ACs as before,
without an additional temporary register. It adds two instructions
to the positive DIV sequence, and remains a constant-size operation.

In `pdp10_udivmodsi4_general`, the negative-divisor comparison branch
continues to avoid signed `DIV`; its forward skip offsets must increase
by two words to account for the longer positive branch. No 71-bit
integer or additional runtime helper is introduced.

## Validation

The unmodified pre-fix reducer produced the wrong answer on native
PDP-6. A temporary replacement of the generated assembly then passed
the exact reducer after correcting the necessary branch offsets.

The source change is in `gcc/config/pdp10/pdp10.md`, in both the
`pdp10_udivmodsi4_positive` and `pdp10_udivmodsi4_general` patterns.
After rebuilding and installing GCC in `$HOME/cross`, the official
`pdp10-testkit` runner retested every previously failed cell for:

| Source | Rerun result |
| --- | ---: |
| `runtime-reduce-udivsi3-libgcc.c` | 36/36 PASS |
| `runtime-reduce-umodsi3-libgcc.c` | 36/36 PASS |
| `runtime-pattern-divmod-si-unsigned.c` | 36/36 PASS |

The new, independent constant-divisor regression
`tests/semantic/regression/pdp10-unsigned-divmod-constant-v17.c`
passes 36/36 registered GCC profiles. It checks `DIVI`/constant
remainder with high-bit and small unsigned numerators without
constant-folding the numerator.

The two testkit runner/inventory validation scripts passed. The
installed `cc1` SHA256 after this change is
`71149fffe4bf33198e9a33290c39faecc63cf8f3228f68f3e319438d5e9f8f40`.

The full compiler matrix is not claimed. Other recorded runtime,
compilation, link and timeout failures remain separate.
