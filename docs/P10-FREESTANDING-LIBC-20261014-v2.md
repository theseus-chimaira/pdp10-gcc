# PDP-10 freestanding libc and raw-byte extensions (v2)

The GCC `pdp10-dec-none` toolchain now stages `libp10c-v2.a` separately
from `libgcc.a`. This OS-independent, PDP-6-compatible library includes
9-bit C-byte memory/string routines. The assembly modules and resulting
DARC archive match the corresponding KCC distribution byte-for-byte.

The archive lives at
`$PREFIX/lib/gcc-lib/pdp10-dec-none/3.2/freestanding/libp10c-v2.a`.
It is not implicitly linked into every freestanding program: specify the
archive explicitly when needed. Compiler arithmetic helpers remain in
`libgcc.a`.

`$PREFIX/pdp10-dec-none/include/string.h` declares the C operations using
GCC's own target `size_t`. The separate `p10bytes-v2.h` declares
`p10bcopy_v2`, `p10bfill_v2`, and `p10bcmp_v2`. Their operands are raw
36-bit PDP-10 byte-pointer descriptors, not C `char *` pointers. The first
three arguments use AC1-AC3. Whole-word paths handle matching, aligned
6/9/18/36-bit layouts; byte instructions handle all other sizes.

The archive is assembled using installed PDP-10 DAS and DARC tools. Matching
archive hashes were verified against KCC. In pdp10-testkit, both compilers
passed the three C regressions under PDP-6. The native assembly regression
also passed for 6/7/8/9/18/36-bit pointers. These are not full CPU-profile
or hosted-libc acceptance tests.

## BLT fast paths (2026-10-14, v3)

Aligned transfers of at least four complete words use PDP-6 BLT for
standard C 9-bit memcpy/memset and raw-width 6/9/18/36-bit copy/fill.
Unaligned and partial-word work retains byte-pointer operations. Backward
memmove overlap remains directional, rather than using unsafe forward BLT.
Eight PDP-6 C execution cells (KCC and GCC) and the native width and
BLT copy/fill regressions passed. Runtime performance and the threshold
are not yet calibrated; these are correctness results, not performance claims.

## Search routines (2026-10-19, v1)

`strstr`, `strpbrk`, and `strspn` have been added as separate PDP-6-compatible
assembler archive members, with GCC target declarations in `string.h`. There
is no stack or writable library data. The same C89 regression passes under
KCC (optimized and unoptimized) and GCC `-O0/-O1/-O2/-Os` with the installed libraries;
each new archive member resolves independently under the production linker.
The new KCC and GCC archives were verified byte-identical. Their combined
static implementation is 55 instruction words (28+13+14), versus 94 for three analogous
KCC-compiled C functions. A measured KCC `-n` SIMH PDP-6 comparison
of 16 searches per workload showed 71,243 vs 198,011 executed steps for
`strstr`, 54,315 vs 132,123 for `strpbrk`, and 21,771 vs 42,939 for
`strspn` (assembler vs C reference, including shared harness). These are
instruction counts, not AAP logical ticks or PDP-6 cycle timing.
Against GCC `-O2` C reference, the corresponding assembler/C step counts
are 59,626/65,770, 42,538/42,618 and 12,954/20,714. The middle case is
essentially tied on instruction steps, so no general throughput claim is
made from this input alone.

## GCC initialized data fix (2026-10-19)

The PDP-10 backend previously buffered an incomplete 36-bit word for a 9-bit
initialized object and sometimes flushed that word only *after* switching to
`.text`, corrupting the object's last characters. Its GAS-mode
`ASM_FINISH_DECLARE_OBJECT` now flushes pending bytes at the end of each
object, while its correct section is still selected. A dedicated PDP-6
execution regression passes at `-O0`, `-O1`, `-O2`, and `-Os` on the rebuilt,
installed GCC compiler. The separate `ADDRESSOF` optimizer ICE has also
been repaired in `gcc/function.c` (see below).

## PDP-10 stack-subword ADDRESSOF fix (2026-10-19)

The optimizer could previously abort in `put_addressof_into_stack` while
handling an `ADDRESSOF` whose underlying 9-bit local was already represented
by `SUBREG(ZERO_EXTRACT(MEM))`. The generic path expected a raw `REG`
and incorrectly attempted another register-to-stack conversion. The
compiler now reuses the PDP-10 byte-address constructor shared with the
existing variable-fixup path to form an address of the existing stack slot.
It does not hide the ICE or suppress the original regression.

The unchanged reproducible source compiled at all four GCC optimization
levels, and the complete freestanding string regression passed on PDP-6
at `-O0/-O1/-O2/-Os` with the updated installed compiler. The same
reproducer compiled with `-O1` for PDP-6, KA10, KI10, KS10, and KL10.
Execution on the latter four CPUs has not been verified by this patch.
