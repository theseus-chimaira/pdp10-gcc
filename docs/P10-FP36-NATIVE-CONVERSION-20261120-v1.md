# GCC PDP-10 native PDP-6 36-bit floating conversions (v1)

Date: 2026-11-20. This is a completed *focused* correction of
GCC's 36-bit signed and unsigned conversion operators, **not**
a claim that upstream `conversion.c` passes. Other CPU families
and all GNU 71-bit conversion helper requirements remain open.

## Fixed owning compiler defects

The prior libgcc1 signed/unsigned integer-to-SF helpers used `FSC`
on unnormalized integer storage. The result's exponent changed,
but its 27-bit fraction remained in the wrong bit position.
The old unsigned-to-SF path also discarded nine low bits before
rounding. In the real compiler this was observed as erroneous
values for ordinary integers, halfway rounding, and upper-range
36-bit unsigned inputs.

The current `gcc/config/pdp10/libgcc1.s` constructs normalized
SF values, uses exact guard-bit rounding with ties-to-even,
and handles the highest possible signed/unsigned inputs including
the fraction carry. It also fixes signed SF-to-int at `-2^35`,
which previously returned zero instead of the representable
signed minimum. New genuine `__fixunssfsi` and `__floatunsidf`
helpers handle unsigned FP-to-36-bit and unsigned 36-bit-to-
D-format conversions, respectively. The latter constructs a
full 27+35 fraction without losing precision in an SF intermediate.

`gcc/config/pdp10/pdp10.md` now lowers PDP-6/KA unsigned
SI-to-DF using `__floatunsidf` rather than SF then DF extension.
`t-none` registers the genuine new per-function DOBJ helpers
in LIB1ASMFUNCS and excludes generic libgcc2 duplicates.

The installed GCC `cc1` SHA256 is
`67ebf49eb2d7eb896b7f95e0aab9bab3f94c148ce914297f4ff1cf1c61cedfc7`.
Installed corrected `libgcc.a` SHA256 is
`afd520fbbff5db32466d887e49136f09ee2c4517b63eded83325cd721505f960`.
The previous `libgcc.a` SHA256 was
`9bead1fd78365d1fa96f868c24e3234d035673e16db01301946f553aa4b097d2`.
The archive was reconstructed from real original DOBJ members;
unchanged members are preserved, and old compiler/library binaries
are backed up in versioned TMPDIR files. No hosted libc was selected.

## Native validation

GCC PDP-6 O0/O1/O2/Os all PASS five independent C89 conversion
oracles (20 guest results), including 36-bit signed/unsigned
int-to-SF/DF, SF/DF-to-int, signed `-2^35`, unsigned `2^36-1`,
and signed/unsigned half-ULP ties-to-even. Four earlier GCC
signed-conversion results actually failed before this repair;
those raw reports remain preserved.

The unchanged GNU upstream `conversion.c` still has a missing
real 71-bit `__fixunssfdi` runtime entry for GCC: all four
native PDP-6 profiles link-fail there. This is a real
outstanding full-source prerequisite, not a compiler test PASS.
The earlier missing `__fixunssfsi` prerequisite is now fixed.

The source-pinned evidence, including 48 native results shared
with KCC (30 corrected guest PASS, 18 genuine old/original FAIL),
230 SHA256-pinned raw artifacts, and 13 fail-closed controls is
in `pdp10-testkit/docs/PDP10-P1-CONVERSION-36BIT-REPAIR-20261120-v1.md`.

P1 source construction remains 55 covered / 12 blocked;
the large compiler/CPU/optimization sweep remains deferred.
