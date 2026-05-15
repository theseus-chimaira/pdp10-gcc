#!/bin/sh
# GCC-only regression: debug options must not change the canonical C ABI.
set -eu

GCC=${PDP10_GCC:-pdp10-dec-none-gcc}
TMPBASE=${TMPDIR:-/tmp}/pdp10-gcc-debug-abi.$$
trap 'rm -rf "$TMPBASE"' EXIT HUP INT TERM
mkdir -p "$TMPBASE"

cat > "$TMPBASE/test.c" <<'EOS'
int f(int a, int b, int c, int d, int e)
{
    return a + b + c + d + e;
}
EOS

"$GCC" -S -g -v -o "$TMPBASE/test.s" "$TMPBASE/test.c" \
    >"$TMPBASE/driver.log" 2>&1
if grep -- '-mregparm=0' "$TMPBASE/driver.log" >/dev/null 2>&1; then
    echo "error: -g changed the PDP-10 C ABI to -mregparm=0" >&2
    exit 1
fi

# With the canonical four-register ABI, the fifth argument is stack-passed.
# At -O0 the backend stores AC1-AC4 in the frame; seeing AC4 here also guards
# against the old debug translation that disabled register arguments entirely.
if ! grep 'movem 4,' "$TMPBASE/test.s" >/dev/null 2>&1; then
    echo "error: -g output does not use AC4 for the fourth argument" >&2
    exit 1
fi

for opt in -mregparm=0 -mreturn-regs=3 -mcall-clobbered=7
do
    if "$GCC" -S "$opt" "$TMPBASE/test.c" -o "$TMPBASE/bad.s" \
        >/dev/null 2>&1
    then
        echo "error: noncanonical ABI option unexpectedly accepted: $opt" >&2
        exit 1
    fi
done

"$GCC" -S -mregparm=4 -mreturn-regs=4 -mcall-clobbered=8 \
    "$TMPBASE/test.c" -o "$TMPBASE/canonical.s"

echo "ok: debug options preserve canonical PDP-10 C ABI"
