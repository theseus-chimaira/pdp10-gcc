# TODO

## Code Generation Comparison

- [ ] Build a benchmark corpus that can be compiled by both GCC for PDP-10 and KCC.
- [ ] Compare GCC `-Os` output against KCC output.
  - Acceptance: GCC `-Os` output is smaller than KCC output for each benchmark, or each exception is documented.
- [ ] Compare GCC `-O2` output against KCC output.
  - Acceptance: GCC `-O2` output runs faster than KCC output for each benchmark, or each exception is documented.
- [ ] Record the exact compiler revisions, flags, assembler, simulator or hardware target, and measurement method used for each comparison.

## Cost Model Comparison

- [ ] Define the instruction and runtime cost metrics to compare.
- [ ] Compare costs reported or implied by KCC, GCC, PDP-10 documentation, SIMH, and other PDP-10 emulators.
- [ ] Document disagreements between sources and choose the cost model GCC should optimize for.
- [ ] Add or update tests that catch regressions in the chosen cost model.

## Target Variants

- [ ] Add orthogonal ITS target selection for KA10, KL10, and KS10.  The
  variant must preserve the existing CPU backend while enabling ITS-specific
  instruction availability such as `CIRC`.
- [ ] Add orthogonal BBN/TENEX target selection for KA10, including `JSYS`
  availability.
- [ ] Define target preprocessor macros for the ITS and BBN variants so system
  code can select the correct monitor/CPU interface without inventing separate
  base CPU families.
- [ ] Add focused target-variant tests for valid/invalid CPU combinations and
  instruction gating.
