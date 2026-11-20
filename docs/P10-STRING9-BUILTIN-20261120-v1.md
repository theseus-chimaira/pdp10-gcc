# PDP-10 9-bit STRING_CST builtin folding correction (2026-11-20, v1)

## Original code-generation defect

The GCC 3.2 `gcc/builtins.c` `c_strlen` helper previously passed the
raw bytes of a PDP-10 9-bit `STRING_CST` to the *host* `strlen`.
Each PDP-10 9-bit character is encoded as **two host bytes** to
preserve the full `0000..0777` character value. The first byte of an
ordinary ASCII character is therefore zero. For `" a b"`, the old
GCC optimizer inferred length zero and compiled `strcpy` as a copy
of only the first target byte, dropping `"a b"` and the terminator.

The correction checks `PDP10_STRING_CST_9BIT_P`, converts host
storage lengths into logical target character counts, and scans
paired host bytes for a *complete* target NUL. Offset arithmetic is
in target units. The generic `c_getstr` API no longer exposes the
two-host-byte encoding as if it were a normal host C string to
`strcmp`, `strchr` or other host-library folding paths. When an
inline literal copy is safe, `expand_builtin_memcpy` still uses the
existing PDP-10-specific `pdp10_builtin_string_data_init` and
`pdp10_builtin_memcpy_read_str` callbacks to read correct 9-bit
values, not host `strlen` or host raw bytes. Otherwise the compiler
uses the actual target memory and compiler-owned libc functions.

No new compiler-side persistent buffer, target runtime service,
allocator, or string-function workaround was introduced. The
corrected `gcc/builtins.c` SHA256 is
`32d2e6d1c375d8d313ecfb59fb0455f03aebc0338efeecb409d4a5c718e13513`.

## Native target verification

The existing compiler regression tests
`pdp10-strcpy-space-literal-v1.c` and
`pdp10-space-token-pointer-v1.c` originally produced six genuine
GCC PDP-6 O1/O2/Os failures (second byte not copied and wrong
first token pointer). After rebuilding/installing GCC `cc1`,
both tests PASS in GCC PDP-6 O0/O1/O2/Os. The optimized copy
now processes all five target bytes of `" a b"`, including NUL.

The new independent `pdp10-builtin-string9-constant-v1.c`
checks `strlen` including embedded NUL, offsets and 9-bit
`\400..\777` characters, constant `strcmp`, `strncmp`, `memcmp`,
and volatile readback from `strcpy` with high 9-bit characters.
The old installed GCC `cc1` was run explicitly against this test:
all four PDP-6 profiles failed at `fail_id=1`. With the
corrected `cc1`, all four profiles PASS with actual guest HALT,
`fail_id=0` and `__test_exit=0`.

Previously installed `cc1` SHA256:
`d43034a6151e57787514ff6293b6d8da846de34a6192766d701505dab1eb6cc8`.
Rebuilt/installed `cc1` SHA256:
`0c33d1e7e93dd372e8b503aabf167c3299acdd830b289199c679ca07a7529b8a`.
The previous binary was preserved as a versioned `$TMPDIR`
backup. The compiler owned `libp10c-v2.a` remains unchanged.

The native test also found two unrelated KCC PDP-6 noopt/opt
failures at `fail_id=6`, where KCC emits the `\400` character of
`"\400x"` as zero. This is an independent KCC 9-bit literal
code-generation issue; it is not a reason to change the correct
GCC oracle or claim all compiler families pass.

Full provenance, 18 source/profile outcomes, 90 SHA256-pinned
raw artifacts and 11 negative controls reside in the versioned
`pdp10-testkit/docs/PDP10-GCC-STRING9-FOLD-REPAIR-20261120-v1.md`
and its evidence manifest/archive.

## Remaining boundaries

This change repairs a demonstrated GCC target-string compiler
defect on native PDP-6. The complete builtin and string-ABI
optimization matrix, all later PDP-10 CPU variants and hosted
`ctype.h`/`isspace` functionality remain unverified. The
unchanged historical `980707-1.c` original still cannot compile
or link with the compiler-owned C library and has not produced a
guest PASS. The full compiler sweep remains deferred.
