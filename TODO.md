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

- [ ] Post-1.0: implement the full PDP-10 character-classification
  library (<ctype.h>, including isprint and case conversions) with a
  deliberate UTF-9 strategy. Do not publish a 7-bit ASCII-only
  classifier as UTF-9-aware. Define C-locale behavior separately
  from Unicode classification, handle 9-bit code units and EOF
  correctly, and specify decoding of multicode-unit UTF-9 characters
  before adding Unicode character properties. Keep this work out of
  the 1.0 scope; no test-only isprint substitute.
- [x] Install a GCC-native PDP-10 freestanding stdio header and real
  independently linkable memory formatter (libp10stdio-v1.a);
  provide sprintf, vsprintf, snprintf and vsnprintf using GCC's
  own va_list ABI. Build at PDP-6-safe -Os, install only
  supported function declarations, and preserve separate library
  linkage. The hosted KCC sprintf ABI is not reused.
  See docs/P10-STDIO-FREESTANDING-20261120-v1.md.
- [ ] Extend GCC stdio to real FILE streams, fprintf, fopen,
  input scanning and remaining hosted interfaces only with an
  actual OS/device backend, never declarations or dummy stubs.
- [x] Add a separately linkable DAIMOS console adapter for GCC
  printf, vprintf, putchar and puts backed by dsys_writechar;
  keep the v2 shared buffer formatter freestanding. Generic
  buffer-formatting regression passes 16 native guest cells in
  pdp10-testkit; the DAIMOS console adapter passes 8 bare-machine
  ABI/fail-stop checks owned by daimos-testkit. The actual DAIMOS
  booted guest acceptance is still unverified (critical root-I/O
  halt before the test application). See
  docs/P10-STDIO-DAIMOS-20261120-v2.md.
- [ ] Accept the real DAIMOS monitor WRITECHAR console test after a
  successful OS boot and guest transcript; ABI recording alone is
  insufficient. FILE streams, fprintf and fopen remain pending.
- [ ] Add correctly rounded floating-point printf conversions to the
  GCC freestanding formatter when required target arithmetic and
  test oracles become available.

- [x] Fix non-extended PDP-10 packed BLKmode inline byte-copy from
  ordinary word addresses: form valid 9-bit byte pointers without
  changing incoming P,S-encoded pointers. Constant symbols and direct
  frame-relative word addresses avoid runtime checks to save code.
  The staged PDP-6 GCC O0 testkit regression fails with the previous
  installed backend and passes with the patched backend. See testkit
  test-gcc-packed-block-bytepointer-v1.sh; operational compiler
  installation and broader profiles remain pending.
- [x] Fix packed 18-bit static/dynamic halfword readback at PDP-6
  O0/O1/O2/Os in staged GCC. Normalizing a packed COMPONENT_REF to its
  containing 36-bit word preserves its selector; explicitly restoring
  physical word alignment prevents GCC's optimized byte-fragment
  decomposition from using incorrect C-unit displacements. The exact
  original upstream execute/20010518-2.c and independent halfword
  regression now pass all four native PDP-6 profiles. The earlier O2/Os
  failures disappeared without changing any original assertion.
- [ ] Install the verified staged GCC backend and check additional CPUs
  and wider regressions before closing the overall packed-field work.
  KCC's separate packed-aggregate failure is not resolved by this fix.

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
