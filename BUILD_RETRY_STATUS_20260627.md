# GCC build retry status, 2026-06-27

Requested retry budget: 10 GCC build attempts.

Result:

- `pdp10-tools` rebuilt successfully.
- `pdp10-tools` `make check` passed.
- KCC rebuilt successfully.
- GCC regenerated core autotools files successfully.
- GCC built successfully through:
  - `cc1`
  - `xgcc`
  - `cpp`
  - `collect2`
  - `libgcc.a`
- GCC installed successfully into the local test prefix after the expected prefix directories were created.

Observed non-backend build stops during retry:

1. Missing target archive tools in the local prefix.

   `pdp10-dec-none-ar` and `pdp10-dec-none-ranlib` were built by `pdp10-tools`, but were not copied into the local prefix before the first GCC continuation.  GCC therefore stopped while creating `libgcc.a`.

   Classification: setup/install staging issue, not a GCC backend bug.

2. Re-running the full `build.sh --no-regen` over an already partially built tree re-entered top-level configure/libiberty setup and produced transient `config.status`/`testsuite/Makefile` failures.

   Directly continuing `make -C gcc all.cross` after recreating `gcc/Makefile` avoided this.

   Classification: old top-level configure/reconfigure fragility in an interrupted build tree, not a PDP-10 backend bug.

3. GCC install initially failed because the install prefix was missing nested directories such as `share/info`, `share/man/man1`, `share/man/man7`, and `pdp10-dec-none/lib`.

   Creating the directories allowed install to complete.

   Classification: install-prefix staging issue, not a compiler bug.

No new compiler ICE or code-generation failure appeared during this retry run.  The real remaining bug from the earlier test classification is still the `subdi3` ICE seen by the testkit, not a GCC self-build failure.
