# PDP-10 packed by-value argument field extraction repair v26

Date: 2026-11-24

## Root cause

The original testkit `gnu-packed-byvalue.c` passed its packed
return-value and local field checks but failed when a 3, 6, 11 or
20-byte packed structure was passed by value and its fields were
read inside the callee. After the v23 copy-loop fix, all 44 GCC
profiles terminated but failed the original runtime oracle.

GCC 3.2 promoted a formal parameter's nine-bit `unsigned char`
fields into full SImode MEMs with the *same* containing-word
address. Their metadata still said `p.a0+0 S1 A9`, `p.a1+0 S1 A9`
and `p.a2+0 S1 A9`, but the RTL MEM expressions themselves had
the same address and mode. Under O0, MOVE/ADD read the entire
36-bit word three times. Under optimization, CSE additionally
folded distinct field reads into one load before final-output
corrections could see the field identity.

## Correct representation before optimization

`pdp10_packed_parm_byte_position` recognizes only a nine-bit
integer COMPONENT_REF of a direct by-value PARM_DECL whose
SImode memory access carries S1/A9 and is frame-based. The MEM's
address is already the containing native 36-bit word; the field's
bit position within that word comes from FIELD_DECL, not from a
guessed byte-pointer selector or reinterpreted stack offset.
Cross-word fields, pointer dereferences and ordinary scalar
SImode words are not rewritten.

The `movsi` and `addsi3` expanders now replace this ambiguous
promoted load *before CSE* by a true nine-bit `ZERO_EXTRACT` or
`SIGN_EXTRACT` of a whole containing-word MEM. The latter is made
without the narrow field's memory attributes, retains volatility
and conservatively aliases memory. The bit position is now part
of the RTL operation, so two different fields cannot be merged
merely because they occupy one physical word.

Combine can still fold a last field access back into a direct
SImode MEM operand of MOVE or ADD. Final-output code recovers
the field position in that case and emits an exact nine-bit LDB;
for signed fields it additionally sign-extends the extracted
nine-bit value before using it. Matching machine-description
attributes report the actual instruction counts (one LDB for an
unsigned MOVE, three operations for signed MOVE, two or four
operations for an unsigned or signed ADD, respectively).

No runtime helper, target-global buffer or permanent guest
allocation is introduced.

## Verification

The smaller diagnostic source passed all **16/16** PDP-6, KA10,
KI10 and KS10 O0/O1/O2/Os configurations with the staged compiler.
The unmodified original `gnu-packed-byvalue.c` passed the same
16 staged combinations.

The independent permanent C89/GNU-packed testkit regression
`pdp10-packed-byvalue-field-extract-v26.c` checks direct 3, 6,
11 and 20-byte by-value arguments and negative signed 9-bit
fields. It fails under the previous installed compiler in both
O0 and O2, and passes after repair. An intermediate backend
variant passed the original test but exposed 18 negative-field
O1/Os failures; the final sign-extension fix resolved them.

With the final rebuilt compiler installed, the official isolated
GCC matrix passed **44/44** cells of the original test and
**36/36** cells of the new regression: **80/80 PASS** under
strict exit policy, with no previously passing cells skipped.
The earlier 44 runtime failures are fixed, but this is not a
claim that every GCC packed-aggregate ABI or full-sweep cell is
now passing.

Eight adjacent registered GCC source groups were also rerun from
scratch under strict exit policy: v17 unsigned division, v18
short16 pointer scaling, v19 indexed byte pointers, v20 reverse
short16 adjustment, v23 packed argument-copy termination, and
three historical wide-integer/control-flow sources. All
**312/312 PASS**. Together with the two v26 source groups this
is **392/392** fresh passing GCC cells. The adjacent checkpoint
is `workdir/pdp10-packed-byvalue-v26-adjacent` in the testkit.

Installed GCC cc1 SHA256:
`752a3c6a33e112dddcb83822c59a33222f8b8017163229d30550a9a4d055fbea`.
