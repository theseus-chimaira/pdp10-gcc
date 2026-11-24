# PDP-10 nested-function trampoline stack repair v21

Date: 2026-11-24

## Diagnosed failure

The GCC matrix source `tests/todo/generic/20000822-1.c` creates a
GNU nested-function trampoline on the stack. In the original sweep,
33 of its 44 GCC cells ended in `timeout`; 18 hit the SIMH instruction
limit and 15 hit the host wall-clock limit. The guest commonly ended
in the illegal-instruction loop at PC 0, not in a long computation.

At KI10 O2, the old compiler allocated four stack words and placed
the two-word executable trampoline at `1(17)` and `2(17)`, above the
allocated frame. The next `PUSHJ 17,f0` overwrote its first instruction
with the saved return address. A scratch assembly experiment moving
the trampoline inside the allocated frame passed the same guest
execution, establishing the stack-address error before source changes.

## Architectural cause and correction

GCC's `round_trampoline_addr` expresses `TRAMPOLINE_ALIGNMENT` as a
multiple of `BITS_PER_UNIT`. The original target left its alignment
at `FUNCTION_BOUNDARY` (36 bits), so the generic code rounded a
PDP-10 *word address* upward to a multiple of four *word addresses*.
The template requires only an aligned machine word, not a four-word
boundary, and this rounding can move the executable trampoline
outside its reserved slot.

`gcc/config/pdp10/pdp10.h` now defines
`TRAMPOLINE_ALIGNMENT BITS_PER_UNIT`, suppressing the incorrect
four-word address rounding. Stack slots remain aligned to 36-bit
machine words by the normal `STACKVAR_BOUNDARY` and `STACK_BOUNDARY`.

The physical copy still requires *word* alignment. A separate,
default-compatible `TRAMPOLINE_COPY_ALIGNMENT` value was introduced
at the `assemble_trampoline_template` memory-copy site in
`gcc/function.c`. The PDP-10 target defines it as `BITS_PER_WORD`.
This preserves the compact two-word `BLT` initialization rather than
generating an eight-byte `LDB`/`DPB` copy loop. The KI10 O2 trampoline
test remains correct, and the resulting assembly is nine lines shorter
than the first correct but bytewise-copying implementation.

No executable-stack permission change, special simulator behavior,
new runtime helper or extra register is required.

## Validation

The existing registered `20000822-1.c` test exercises nested static
chains and indirect calls, which must return 2. The installed
compiler passes **44/44** GCC cells in the versioned isolated testkit
run `workdir/pdp10-timeout-triage-v21-trampoline`, compared with 33
timeouts and 11 passes in the previous checkpoint. Its template-copy
variant was also checked manually with KI10 O0/O1/O2/Os.

The final installed compiler was checked with the associated testkit
source and other pointer regressions: **218/218** GCC/KCC cells passed
under strict exit policy, with no old successful checkpoints skipped. The
remaining timeout clusters are independent: 36 wide-integer GCC,
27 packed-aggregate GCC, and 22 KCC optimizer cases.
