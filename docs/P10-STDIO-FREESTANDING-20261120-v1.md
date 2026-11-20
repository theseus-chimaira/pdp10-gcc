# PDP-10 GCC freestanding stdio buffer formatter (v1)

Date: 2026-11-20. The pdp10-dec-none target now installs a real
stdio.h at the following target include path and a separately
selectable native archive:

    $PDP10_PREFIX/pdp10-dec-none/include/stdio.h
    $PDP10_PREFIX/lib/gcc-lib/pdp10-dec-none/3.2/freestanding/libp10stdio-v1.a

The archive contains an actual GCC-built formatter, rather than
declarations alone or an adapter around KCC's incompatible variadic
entry. The source is gcc/config/pdp10/freestanding/p10stdio-format-v1.c;
the build and installation step is integrated into build.sh. The
standalone build-stdio-archive-v1.sh uses installed GCC/DAS/DARC and
the PDP-6-safe baseline at -Os.

## Implemented interface

- sprintf and vsprintf (unbounded caller-supplied destination).
- snprintf and vsnprintf (bounded destination; return the full
  count of characters that would have been written, excluding NUL).
- C89 stdarg.h forwarding uses the actual GCC built-in va_list;
  this is not KCC's separately implemented ABI.
- Format conversions: d, i, u, o, x, X, c, s, p and percent;
  h and l integer length modifiers; flags -, +, space, # and 0;
  width and precision, including *.
- Proper NUL termination and positive character counts in supported
  cases. For a size-zero buffer, the destination may be NULL.
  An unsupported conversion (including floating-point) returns -1
  rather than silently emitting an incorrect result.

Memory behavior: no global writable library state, heap allocator,
temporary output allocation or OS dependency. The engine stores
reversed integer digits in a 40-character local buffer and streams
the result to its caller's memory. Signed minimum values avoid
undefined signed negation. Width and precision parsing reject
overflow rather than wrapping.

The target has 9-bit characters and 36-bit int/long. The implementation
never assumes ordinary host 8-bit bytes or 32-bit machine words.
It does not change any compiler calling convention.

## Deliberately not included

This is a freestanding buffer-formatting subset, not all of
hosted ISO stdio.h. File streams, FILE, standard streams,
printf, fprintf, putchar, puts, fopen, fclose and input scanning
need a real OS or device-I/O backend, and are neither declared as
operational nor stubbed out. Floating-point printf conversions are
also pending; the library reports them as unsupported.

## Real installation and linkage

The normal build script stages the target include and archive.
The formatter is not implicitly linked into arbitrary programs,
consistent with the existing separate libp10c-v2.a arrangement.
For production DOBJ/DARC linkage, place libp10stdio-v1.a before
any older hosted libc that happens to define sprintf, followed by
libgcc.a. Do not substitute KCC's sprintf assembly entry for
GCC-compiled callers.

pdp10-testkit also selects the installed p10stdio-format-v1.s member
for GCC source references to sprintf, vsprintf, snprintf or vsnprintf,
without selecting it for KCC or supplying fake libc symbols.

The focused registered compatibility suite is:

    misc/compat/test-gcc-p10stdio-v1.sh

It checks the real archive through production dlink and executes
two meaningful target programs at GCC O0/O2 across PDP-6, KA10,
KI10 and KS10. These include unchanged upstream 920726-1.c with
real sprintf, strlen and strcmp, and a dedicated C89 regression
covering signed/unsigned formatting, width/precision, buffers,
forwarded va_list, and target 9-bit characters. The latter also
passes the host C89 compiler with the 9-bit assertion guarded.

The separate P1 ledger can now close the hosted-formatting part of
920726-1.c when matched with its independently verified return-value
regression. The original still contains uninitialized, unused return
objects; GCC successes do not imply correctness of those returns.

## Pending work

1. Define an actual OS I/O backend for console and stream functions.
2. Add correctly rounded floating-point output if needed by genuine
   printf users and supported target hardware/runtime.
3. Verify more integer-format edge cases and CPU/codegen profiles.
4. Decide how hosted stdio is selected by GCC and DAIMOS while
   keeping the freestanding compiler usable without a monitor.
