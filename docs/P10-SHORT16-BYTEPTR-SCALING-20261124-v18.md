# PDP-10 short16 byte-pointer scaling and optimized load repair v18

Date: 2026-11-24

## Diagnosed defects

The `runtime-byte-pointer-diff.c` failure at octal ID 0103 initially
looked like a signed six-bit load defect. It was an oracle/ABI mismatch:
GCC's built-in `char6` is unsigned SIXBIT, while KCC's `char6` is signed.
The testkit now uses the signed `int6` type for its common signed
six-bit check and separately tests the GCC built-in's unsigned values.
The signed expectation was preserved, not removed.

After that correction, the test reached the real short16 dynamic
pointer bug at ID 0310. On PDP-10, short16 elements occupy two
nine-bit C storage units but one architectural 16-bit byte position.
The generic front end had already multiplied the index by two into an
RTL register; the backend then treated the scaled result as an element
count. `p + 4` therefore advanced eight elements. The `pdp10_expand_binop`
hook now recognizes an exact multiply-by-two in the original pointer
expression tree and converts the already-evaluated RTL to element units.
It deliberately does not reevaluate the index, which may have side
effects. An intact RTL multiply/shift or a constant remains handled by
the existing `pdp10_expand_addbp3` normalization.

The early constant symbolic-pointer fold in
`expand_add_or_sub_pointer` bypassed that normalizer: `&array[3]`
could become `POINT 16,array+3,15` rather than the proper
`POINT 16,array+1,31`. That fold now normalizes the same even
two-unit short16 offsets before applying ADJBP to the pointer word.

At optimized levels, a constant `POINT 16` dereference could be
lowered into an `HImode` MEM and emitted as `HLRZ` (18 bits) followed
by a 16-bit signed extension. The resulting value discarded bit 15:
`-32767` became `4`. `pdp10_output_movhi` now recognizes a constant
native 16-bit byte pointer and issues `LDB` of exactly 16 bits instead.
It does not alter 18-bit halfword loads.

Finally, the native S=16 branch in `pdp10_expand_addbp3` called its
helper with a result register overlapping the original pointer. The
helper can select a different destination for ADJBP, but its return
value was ignored. Optimized KS10 reverse increments then returned
the unchanged pointer. The dispatcher now chooses a non-overlapping
destination before splitting the logical S=1 and native S=16 branches,
so both paths write the same result.

## Verification boundary

The new independent C89 testkit regression
`pdp10-short16-pointer-scaling-v18.c` covers dynamic add/subtract,
constant address folding, signed short16 loads, and the common
result-register contract. The standalone stage test passed all 16
CPU/optimization combinations (PDP-6, KA10, KI10, KS10 times
O0/O1/O2/Os). The registered, isolated full GCC profile run then
passed **36/36** under strict exit policy.

The broader `runtime-byte-pointer-diff.c` source passed **9/9 GCC O0**
profiles in the isolated checkpoint directory, but its complete
36-profile rerun still has **11 run_fail and 16 timeout** results.
Those are separate optimized byte-pointer problems and are not claimed
fixed here. No expectation was weakened to accept them.

Testkit evidence is in
`history/PDP10-SHORT16-BYTEPTR-REPAIR-20261124-v18.md` of the
`pdp10-testkit` repository. Independent checkpoint directories were
used because another test runner was concurrently writing to
`full-sweep-current`.
