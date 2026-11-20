# PDP-10 GCC C-locale isprint/isspace target header (v1)

The PDP-10 GCC freestanding header `gcc/ginclude/ctype.h` now declares
real `int isprint(int)` and `int isspace(int)` entry points, installed
with the target's `USER_H` list in `gcc/config/pdp10/t-pdp10`.

The compiled functions are maintained in the shared KCC compiler
runtime, not in a test fixture or DAIMOS hosted libc. Both compiler
freestanding installations use the same PDP-6-compatible assembly
source and independently linkable `libp10ctype-c-locale-v1.a`:

    libp10ctype-c-locale-v1.a
    SHA256 790b064ecef6857d98c9a30b326c638af839b70931b19fc603fbe9b10aee99a3

Both functions operate on the entire 9-bit unsigned-character domain
(0000..0777) and EOF (-1). In the C locale, `isprint` accepts octal
040..0176; `isspace` accepts 011..015 and 040. All other 9-bit
characters are unclassified by these functions. This does **not**
implement UTF-9 or remaining `<ctype.h>` character properties.

Against the installed compiler C runtime, unchanged historic test
sources `991112-1.c` and `980707-1.c` both PASS on all six native
PDP-6 compiler profiles. A separate exhaustive test validates all
512 nine-bit values and EOF through real function pointers and also
PASSes all six profiles. The old missing-header and undefined-symbol
link failures remain archived in PDP10-Testkit.

Source: `kcc/runtime/libc/kcc-libc-ctype-c-locale-v1.s`.
The original `libp10c-v2.a` was not modified, preserving its
SHA256 provenance and the historical compiler regression records.

This is a focused PDP-6 and C-locale validation. The other CPU
profiles, general hosted libc and complete UTF-9 semantics remain
outside the verified claim.
