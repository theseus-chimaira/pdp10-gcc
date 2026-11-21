# GNU 71-bit integer / SF / D-format conversion repair (2026-11-21, v1)

## Verified compiler correction

The original GCC GNU test `conversion.c` contains signed and unsigned
71-bit `long long` conversions beyond KCC's source-language modes.
All four native PDP-6 GCC profiles (O0/O1/O2/Os) historically failed
linking on missing `__fixunssfdi`; after adding that helper they
exposed missing `__fixsfdi`. Subsequent native failures revealed
broken generic DImode-to-SF/DF conversion near 2^35 and a negative
fractional SF-to-DImode cast producing a noncanonical negative zero.
The previous generic double-to-DImode arithmetic also caused native
execution to time out after passing the earlier assertions.

These were real GCC libgcc compiler-runtime bugs, not deficiencies
in the historical test source. The owning GCC library source
`gcc/config/pdp10/libgcc1.s` now provides six independent DARC/DOBJ
entry points, selected with `gcc/config/pdp10/t-none`:

* `__fixunssfdi`: positive PDP-10 SF to unsigned 36+35-bit integer;
* `__fixsfdi`: signed SF to signed 36+35-bit integer, truncating
  negative fractions to canonical integer zero;
* `__fixunsdfdi`: unsigned D-format double to 36+35-bit integer,
  without floating division or hosted helpers;
* `__fixdfdi`: signed D-format to 36+35-bit integer, using real
  absolute magnitude and full-width two's-complement sign restoration;
* `__floatdidf`: signed 71-bit integer to canonical 27+35-bit D-format,
  with exact guard/sticky rounding and sign carry;
* `__floatdisf`: signed 71-bit integer directly to canonical 27-bit
  SF, rounding nearest/ties-to-even from *all 71 bits*, rather than
  double-rounding through D-format.

Input and output follow the GCC DImode ABI: AC1 is a 36-bit high word,
AC2 carries 35 low payload bits and a copied high-word sign bit.
For the SF/DF routines the input or result is AC1 (SF) or AC1:AC2
(DF). All helpers use the PDP-6 baseline instruction set and bounded
stack saves; there are no permanent tables, heap allocations or OS
dependencies. The testkit links real independently extracted libgcc
members, not substitutes from an OS or fixture.

## Native PDP-6 acceptance

The unchanged historical source
`tests/semantic/upstream/pdp10/pdp10/conversion.c`, SHA256
`2736912e40e1dabd84f7f193ddaaf5ddf9bf00e76edb0cca0611b3f49e5c1beb`,
now **PASSes all six native PDP-6 KCC/GCC compiler profiles**:
KCC noopt/opt and GCC O0/O1/O2/Os. A separate original-source
assertion-line diagnostic also PASSes all six. The GNU-only original
71-bit integer-to-floating and floating-to-integer portions each
PASS the four GCC profiles independently. These are real simulator
executions with successful HALT and `__test_exit=0`.

A separate C89 test invokes the genuine `__floatdisf` and
`__floatdidf` entry points directly with explicit 36+35-bit
arguments, without using the host C `long long` extension. It PASSes
GCC O0/O1/O2/Os and checks zero, one, 2^35 boundaries, near-signed
71-bit extrema, negative one and signed minimum. It also checks the
critical large SF rounding boundary near 2^69: an exact halfway
case rounds to even, while a one-bit sticky increment rounds upward;
both cases preserve the correct D-format value.

The eight previous C89 36-bit signed/unsigned conversion sources
continue to PASS all six native PDP-6 profiles (48 guest PASS).
The old missing-compiler-helper errors and subsequent `run_fail`/
timeout results remain historical SHA256-pinned evidence; they must
not be silently relabeled or discarded.

The installed GCC compiler `cc1` has not changed, SHA256
`67ebf49eb2d7eb896b7f95e0aab9bab3f94c148ce914297f4ff1cf1c61cedfc7`.
The original installed libgcc archive was preserved at SHA256
`afd520fbbff5db32466d887e49136f09ee2c4517b63eded83325cd721505f960`.
The rebuilt real compiler-owned libgcc archive has SHA256
`f9b1720d59df7b0e8868e27a0aa036a222472a820d7080a3e008f61b0b4bed6c`.
Only the six 71-bit conversion DOBJ members were replaced or added;
all other original installed libgcc archive members were preserved.

## Acceptance boundary

This closes P1 original source `conversion.c`, Batch 4 slot 4,
for the verified native PDP-6 six-profile construction. It does not
authorize a full compiler/CPU/optimization sweep or claim the other
PDP-10 CPU models have been tested. P1 becomes 56 covered / 11
blocked; frozen 59-source batches become 48 covered / 11 blocked
after the owning testkit ledger is authenticated and committed.

The reproducible source/profile transcript and all original/ABI
guest artifacts belong in `pdp10-testkit` under a new, versioned
GCC 71-bit conversion evidence record. Source-specific 71-bit
accuracy, the PDP-6 runtime ABI, and compiler/OS-library separation
remain explicit in that record.
