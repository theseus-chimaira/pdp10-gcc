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
