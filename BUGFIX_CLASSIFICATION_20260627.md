# GCC/KCC/PDP10 Tools Failure Classification - 2026-06-27

This note records the current build and test failure classes before fixing
individual compiler bugs.

## Local builds

- pdp10-tools: builds successfully.
- pdp10-tools tests: pass via `make check`.
- kcc-linux: builds successfully with the host C compiler.
- gcc-3.2-pdp10: core autotools regeneration succeeds. The cross GCC build
  was restarted five times. It reached normal `gcc/` object and generator
  compilation, but did not finish within the allowed execution windows.

One transient GCC configure failure was seen before regeneration:

- libiberty `config.status` failed to create `Makefile` with a stale/incomplete
  build directory.
- After `./build.sh --regen-only` and a fresh build directory, this did not
  recur.

During GCC configuration, these missing target-side binutils names were also
reported:

- `pdp10-dec-none-nm`
- `pdp10-dec-none-ld`
- `pdp10-dec-none-objdump`

The build still proceeded into host compiler object generation, so these are
classified as toolchain-completeness warnings for now, not the current stop.

## Full-sweep result classes

Input summary:

- selected cells: 22782
- pass: 15416
- compile_fail: 4
- policy_fail: 6
- run_fail: 4278
- all failures are for machine `ks10`

### Class A: test harness / emulator path failure

Count: 4278 run failures.

Observed message in representative cell logs:

```text
missing hardcoded SIMH for ks10: /data/data/com.termux/files/home/cross/bin/pdp10-ks
```

This affects both GCC and KCC semantic cells. The assembly generation step
completed; execution failed before the simulator ran. These are not compiler
semantic failures yet.

Likely next action: install or symlink the KS10 SIMH binary at the path expected
by `p10bare`, or teach `p10bare`/the testkit to discover the installed SIMH
binary instead of using the hardcoded path.

### Class B: KS10 policy emits ADJBP in base policy test

Count: 6 policy failures.

Failing test:

- `tests/codegen/base/base_policy_no_adjbp_jffo.c`

Failing profiles:

- `kcc-ks10-opt`
- `kcc-ks10-noopt`
- `gcc-ks10-o0`
- `gcc-ks10-o1`
- `gcc-ks10-o2`
- `gcc-ks10-os`

Observed forbidden mnemonic:

```text
forbidden_mnemonics=ADJBP
```

Both compilers emit `ADJBP` for byte-pointer stepping in KS10 profiles. This is
either a real compiler policy issue, or the policy test is too broad for KS10
profiles if KS10 is allowed to use `ADJBP`.

Likely next action: decide whether the baseline no-ADJBP policy should apply to
KS10 profiles. If it should, fix GCC and KCC byte-pointer address generation. If
not, adjust the test matrix/policy expectations.

### Class C: GCC DImode subtract ICE for const -1 on KS10

Count: 4 compile failures.

Failing test:

- `tests/codegen/pattern/subdi3.c`

Failing profiles:

- `gcc-ks10-o0`
- `gcc-ks10-o1`
- `gcc-ks10-o2`
- `gcc-ks10-os`

Representative compiler error:

```text
unrecognizable insn:
(insn 14 13 16 (set (reg:DI 24)
        (minus:DI (reg:DI 25)
            (const_double -1 [0xffffffffffffffff] 255 [0xff]))) -1 (nil)
    (expr_list:REG_DEAD (reg:DI 25)
        (nil)))
Internal compiler error in extract_insn, at recog.c:2134
```

This is a real GCC backend bug. The KS10 `subdi3` pattern does not recognize a
DImode subtract with a `const_double -1` operand.

Likely next action: fix the PDP-10 backend `subdi3` predicate/expander/splitter
so subtracting DImode constant -1 is handled, probably by canonicalizing it to
addition of +1 or by accepting this constant form in the relevant pattern.

## Suggested fix order

1. Fix Class A first, because it hides real runtime failures behind a harness
   failure.
2. Fix Class C next, because it is a clear GCC backend ICE with a small failing
   surface.
3. Resolve Class B after deciding the KS10 policy rule for `ADJBP`.
