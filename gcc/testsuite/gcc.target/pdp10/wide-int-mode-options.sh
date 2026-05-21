#!/bin/sh
set -eu
cc=${PDP10_GCC:-pdp10-dec-none-gcc}
src=${1:-gcc/testsuite/gcc.target/pdp10/wide-int-mode-macros.c}
tmp=${TMPDIR:-/tmp}/pdp10-wide-mode-$$
trap 'rm -rf "$tmp"' EXIT HUP INT TERM
mkdir -p "$tmp"
"$cc" -E -DTEST_71 -mlong-long-71bit "$src" > "$tmp/71.i"
"$cc" -E -DTEST_72 -mlong-long-72bit "$src" > "$tmp/72.i"
"$cc" -S -O1 -mlong-long-71bit -o "$tmp/71.s" "$src"
"$cc" -S -O1 -mlong-long-72bit -o "$tmp/72.s" "$src"
test -s "$tmp/71.s"
test -s "$tmp/72.s"
echo "PDP-10 71/72-bit long long modes compile"
