# Todo

This is the single todo list for this PDP-10 GCC fork. It consolidates the
project todo list and PDP-10-specific unfinished work described in source
and documentation.

Generic GCC 3.2 comments for unrelated targets are not repeated here. They are
upstream historical maintenance notes rather than work specific to this fork.

## GCC 4.7 migration

- [ ] Prepare the PDP-10 work for migration from GCC 3.2 to GCC 4.7.
  - Inventory every PDP-10-specific front-end, middle-end, backend, libgcc,
    build-system, and testsuite change that must survive the move.
  - Map GCC 3.2 target macros, RTL hooks, machine-description patterns, reload
    assumptions, and build machinery to their GCC 4.7 equivalents.
  - Separate changes that should be ported directly from changes that should
    be reimplemented using newer GCC interfaces.
  - Keep the GCC 3.2 compiler usable until the GCC 4.7 port reaches equivalent
    target and regression-test coverage.

## Optimizer and code-size work

- [ ] Benchmark the shared PDP-6 `libp10c-v2.a` assembly routines against
  byte-by-byte implementations across CPU and optimization profiles.
  Verify archive extraction with production link scripts and expand
  width/alignment cases. The staged archive and PDP-6 compiler tests pass.

- [ ] Investigate optimizer regressions where `-Os` produces larger code than
  `-O2` or even `-O1`.
  - Build focused regression cases for every confirmed inversion.
  - Identify which GCC passes or PDP-10 cost decisions cause the growth.
  - Fix the underlying target or optimizer issue rather than papering over it
    with per-test exceptions.
  - Keep `-Os` conservative about transformations already known to increase
    PDP-10 code size, including unnecessary save/restore traffic, forced
    memory operands, block reordering, and loop branch-count conversion.
- [ ] Build a benchmark corpus that can be compiled by both GCC for PDP-10 and
  KCC.
- [ ] Compare GCC `-Os` output against KCC output.
  - Acceptance: GCC `-Os` output is smaller than KCC output for each
    benchmark, or each exception is understood and documented.
- [ ] Compare GCC `-O2` output against KCC output.
  - Acceptance: GCC `-O2` output runs faster than KCC output for each
    benchmark, or each exception is understood and documented.
- [ ] Record the exact compiler revisions, flags, assembler, simulator or
  hardware target, and measurement method used for each comparison.

## Cost model

- [ ] Define the instruction-size and runtime-cost metrics used by the PDP-10
  backend.
- [ ] Compare costs reported or implied by KCC, GCC, PDP-10 documentation,
  SIMH, and other PDP-10 emulators.
- [ ] Document disagreements between sources and choose the cost model GCC
  should optimize for.
- [ ] Add or update tests that catch regressions in the chosen cost model.

## PDP-10 backend

- [ ] Implement working PDP-6 floating-point code generation for
  `-mpdp6-float`.
- [ ] Implement working KA10/PDP-10 floating-point code generation for
  `-mka10-float`.
- [ ] Add enough floating-point runtime coverage to implement correct
  double-to-single rounding for positive and negative fraction edge cases.
- [ ] Revisit the generic libgcc floating conversions currently excluded from
  the freestanding PDP-10 runtime and provide target implementations where
  they are still required.
- [ ] Implement or deliberately retire PDP-10 string-instruction support.
- [ ] Implement multi-section function support for the huge code model.
- [ ] Add an explicit target proof/helper for folding the hand-written
  label-value call sequence into `PUSHJ`, then enable the currently disabled
  peephole only when equivalence is guaranteed.
- [ ] Add an extended-addressing implementation for switch table labels and
  entries instead of relying on the current unextended-addressing form.
- [ ] Decide whether the freestanding target needs exception-unwind support
  and, if so, add the required libgcc unwinder objects.

## Wide integer modes and ABI

- [ ] Keep explicit regression coverage for the 71-bit and raw 72-bit
  `long long` modes, especially at ABI boundaries.
- [ ] Verify that DAIMOS and libgcc continue to use the normalized 71-bit ABI
  and that mixed 71-bit/72-bit calls require explicit adapters.
- [ ] Keep public headers using `int71_t`/`uint71_t` and
  `raw72_t`/`uraw72_t` where the representation matters.

## Target variants

- [x] Add orthogonal ITS target selection for KA10, KL10, and KS10.
- [x] Define `__PDP10_ITS__` and `TARGET_ITS` for ITS-targeted source.
- [x] Provide direct raw-pair `CIRC` access through `<pdp10-its.h>` without
  introducing a GCC builtin.
- [x] Add focused GCC target regression coverage for ITS `CIRC`.

- [ ] Validate BLT speed and the four-whole-word threshold on PDP-6 using
  actual instruction timings. Extend the raw-byte regressions to later
  processors and additional pointer alignments and overlap conditions.
