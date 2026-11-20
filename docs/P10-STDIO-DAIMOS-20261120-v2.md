# GCC PDP-10 stdio v2: shared formatter and optional DAIMOS console

The v1 formatter remains installed and independent. This version adds
an output callback to the shared formatting engine, without changing
the GCC stdarg ABI, its C89 buffer-formatting APIs, or the freestanding
target's default I/O requirements.

## Installed artifacts

    $PDP10_PREFIX/lib/gcc-lib/pdp10-dec-none/3.2/freestanding/libp10stdio-v2.a
    $PDP10_PREFIX/lib/gcc-lib/pdp10-dec-none/3.2/freestanding/p10stdio-format-v2.s
    $PDP10_PREFIX/lib/gcc-lib/pdp10-dec-none/3.2/freestanding/libp10stdio-daimos-v1.a
    $PDP10_PREFIX/lib/gcc-lib/pdp10-dec-none/3.2/freestanding/p10stdio-daimos-console-v1.s
    $PDP10_PREFIX/pdp10-dec-none/include/p10stdio-daimos-v1.h

The independent libp10stdio-v2.a continues to implement sprintf,
vsprintf, snprintf and vsnprintf. No OS symbol is required to link it.
The internal callback entry, __p10stdio_vformat_v2, can stream
output without allocating a buffer. It stops immediately when
an output callback fails. Format parsing and numeric conversion
remain in one shared implementation.

The separate libp10stdio-daimos-v1.a implements printf, vprintf,
putchar and puts. It uses the real DAIMOS dsys_writechar interface,
whose descriptor 1 selects stdout. This archive is optional and
must only be linked with the DAIMOS syscall veneer. The portable
stdio.h does not claim to implement FILE, file streams, scanf,
fopen or fprintf, and float formatting is still unsupported.

Both archives are built by build-stdio-archive-v2.sh and staged
by build.sh. The compiler's ordinary bare-metal test runner selects
only p10stdio-format-v2.s for its memory-format functions; it
does not silently pull DAIMOS WRITECHAR into freestanding programs.

## Test ownership and observed evidence

Generic C standard-library tests belong in pdp10-testkit:

    misc/compat/test-gcc-p10stdio-v2.sh

The registered v2 suite passes the real linker archive-extraction
check and 16 native guest programs across PDP-6, KA10, KI10 and
KS10 at GCC O0 and O2. It validates real memory formatting, and
the unchanged historical 920726-1.c program.

DAIMOS-specific console integration tests belong in daimos-testkit:

    tests/validation/daimos-gcc-stdio-console-abi-v1.sh
    tests/validation/daimos-gcc-stdio-console-guest-v1.sh

The ABI test passes eight bare-machine CPU/mode combinations using
a recording dsys_writechar sink. This proves GCC variadic calling,
return behavior, error propagation and archive extraction, NOT
the actual monitor syscall. The full guest test is opt-in and
NOT ACCEPTED: its initial DAIMOS boot stopped in critical root I/O
before the application executed. No kernel or simulator workaround
was introduced. Full monitor console acceptance remains open.

This boundary is deliberate: standard-library correctness belongs
to the compiler testkit, while real DAIMOS syscall/guest integration
belongs to the OS testkit.
