# PDP-10 wide integer modes v1

Ordinary `long long` defaults to the normalized PDP-10 71-bit doubleword
model.  `-mlong-long-71bit` selects that ABI explicitly.

`-mlong-long-72bit` selects the legacy flat raw 72-bit model.  In that mode
all 72 bits of the two-word DImode object participate in arithmetic.  The
preprocessor defines `__LONG_LONG_72BIT__`; otherwise it defines
`__LONG_LONG_71BIT__`.

The two modes have identical two-word storage size but different value
representations and arithmetic semantics.  Objects and calls must not cross a
mode boundary without an explicitly named adapter.  DAIMOS and libgcc remain
built with `-mlong-long-71bit`.

Portable public headers should use `int71_t`/`uint71_t` for the DAIMOS ABI and
`raw72_t`/`uraw72_t` for flat 72-bit values.  A program deliberately compiled
entirely with `-mlong-long-72bit` may additionally typedef `long long` as
`int72_t`, but that spelling is mode-dependent and is not a mixed-ABI type.
