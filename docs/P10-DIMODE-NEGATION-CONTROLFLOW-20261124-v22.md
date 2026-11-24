# PDP-10 doubleword negation literal-control-flow repair v22

Date: 2026-11-24

## Source of 36 GCC runtime timeouts

The upstream wide-integer regression `950612-1.c` tests absolute
values through signed and unsigned 36-bit and extended `long long`
functions. Its previous GCC matrix had 36 timeouts, while the KCC
cells passed. Dividing the test into individual function invocations
showed the 36-bit functions `f1` and `f2` pass but extended-integer
`f3` and `f4` repeatedly reach PC 0.

Both `*negdi2` and `*negdi2_71_no_dmovn` formerly used an operand
sequence of the form `JUMPE low,[AOJA high,.+2]`. A bracket literal
is a data word in the assembler's literal pool. `JUMPE` transfers
control to that word, and the relative target inside the literal
is evaluated at its pool address, not at the original negation
sequence. A zero low word can therefore jump into unrelated or
uninitialized memory. Raising the simulator limit to 200 million
instructions does not cure this control-flow defect.

## Correct implementation

Use a local conditional-skip idiom, with no out-of-line literal:

```
setca high,
skipn low,low
addi high,1
movn low,low
```

`SKIPN` skips the increment when the low word is nonzero. When
it is zero, the high word is complemented and incremented, while
negating low zero leaves it unchanged. The separate-destination
alternative similarly negates the low word and conditionally
increments the complemented high word. The 71-bit pattern also
preserves its existing low-word sign-bit normalization.

The longer visible sequences have correct GCC instruction `length`
attributes; they remove a literal-pool dependency instead of adding
an additional scratch register or arithmetic runtime helper.
The patch is in `gcc/config/pdp10/pdp10.md`.

## Validation

Scratch replacement of the offending emitted instructions showed
the old zero-low-word loop converted into a normal guest HALT.
After correcting the conditional skip to `SKIPN`, the `f3` signed
wide-absolute-value diagnostic passed on KI10. A staged rebuilt
compiler passed the original full upstream source at O2 on PDP-6,
KA10, KI10 and KS10. This included both `f3` and `f4`.

The final installed compiler was verified through a fresh isolated
strict-exit-policy testkit matrix of five source groups, including
the original upstream wide-int source, the v21 trampoline/defined
upstream cases, and the earlier v19/v20 byte-pointer regressions:
**284/284 PASS** (204 GCC, 80 KCC), with no old PASS skipped.
All **36** formerly timed-out GCC wide-int cells now pass. The
remaining historical timeout groups total 49: 27 packed-aggregate
GCC and 22 KCC optimizer cases.
Only actual PASS results qualify as repaired timeouts; raising
the timeout alone is never taken as an oracle success.
