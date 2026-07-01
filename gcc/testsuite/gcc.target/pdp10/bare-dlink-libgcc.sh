#!/bin/sh
# Verify that pdp10-dec-none GCC drives dlink and supplies bare libgcc itself.
set -eu
cc=${PDP10_GCC:-pdp10-dec-none-gcc}
src=${1:-gcc/testsuite/gcc.target/pdp10/bare-dlink-libgcc-1.c}

case "$cc" in
*/*) cc_dir=`dirname "$cc"` ;;
*)   cc_path=`command -v "$cc"` || { echo "compiler not found: $cc" >&2; exit 1; }
     cc_dir=`dirname "$cc_path"` ;;
esac
PATH=$cc_dir:$PATH
export PATH

tmpbase=${TMPDIR:-.}
tmp=$tmpbase/pdp10-bare-dlink-$$
trap 'rm -rf "$tmp"' EXIT HUP INT TERM
mkdir -p "$tmp"

"$cc" -O2 -march=166 -S "$src" -o "$tmp/probe.s"
grep '%BADL9' "$tmp/probe.s" >/dev/null

"$cc" -O2 -march=166 "$src" -o "$tmp/probe.dxr"
test -s "$tmp/probe.dxr"

echo "PDP-10 bare dlink/libgcc resolution passed"
