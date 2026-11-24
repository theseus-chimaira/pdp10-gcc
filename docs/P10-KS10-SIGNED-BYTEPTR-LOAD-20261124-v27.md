# PDP-10 KS10 signed byte-pointer and halfword load repair v27

Date: 2026-11-24

## Observed failure

The broad `runtime-byte-pointer-diff.c` regression passed nine of
twelve KS10 GCC profiles, but failed at optimized cases `0245`
(O1 signed nine-bit char9 load) and `0344` (O2/Os signed eighteen-bit
short18 load). These are exact-width byte-pointer dereferences;
the P/S selector must not be replaced by a word load just because a
pointer register or the stack slot storing that pointer is aligned.

## Backend correction

`pdp10_scalar_mem_p` now excludes nested `MEM(MEM(...))` accesses,
where the inner memory operation obtains an encoded byte pointer,
and does not mistake a QI/HI register-held pointer with no source
expression for a scalar local. In `sign_extendqisi2`, the optimized
word/shift shortcut is allowed only when the actual accessed byte
is known to be a scalar or constant aligned object, rather than
merely having an apparently aligned pointer container. Dynamic
byte pointers therefore retain exact `LDB` semantics.

For `pdp10_output_movhi`, a constant `POINT 16` load uses `LDB`,
preserving its precise 16-bit field. A constant native `POINT 18`
is decoded to determine which architectural half it selects;
aligned left and right halfword cases then use their matching
`HLRZ`/`HRRZ` or signed `HLRE`/`HRRE` operation. An address alone
cannot establish the selected half.

## Verification

The pre-fix installed compiler produced runtime failures `0245`
at KS10 O1 and `0344` at KS10 O2/Os. The staged corrected compiler
passed all three reduced original cases. After installing the rebuilt
`cc1`, the complete GCC `runtime-byte-pointer-diff.c` source passed
**36/36** profiles under strict exit policy in its own isolated
`workdir/pdp10-byteptr-ks10-v27-independent-b` directory.

The permanent dedicated regression
`pdp10-ks10-byteptr-sign-half-v27.c`, covering both signed widths,
passed **36/36** GCC profiles under strict exit policy in a separate
`workdir/pdp10-byteptr-ks10-v27-independent-test` directory.

These results correct the last three historically recorded optimized
KS10 byte-pointer assertions. Other compiler tests must be checked
independently; this is not a claim of a full compiler matrix PASS.

Six adjacent GCC sources covering the original packed by-value
cases, v26 field extraction, v18/v19/v20 pointer adjustments, and
v23 packed-copy termination were also rerun from scratch:
**224/224 PASS**, strict exit policy, no green checkpoints skipped.
Across the two 36-cell primary runs and this independent adjacent
selection, v27 has **296/296 passing GCC cells**.
