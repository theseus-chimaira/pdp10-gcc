#!/bin/sh
# Build shared GCC-native PDP-10 formatter and optional DAIMOS console.
# The former never references an OS syscall; the latter requires one.
set -eu
src=$1
buffer_archive=$2
daimos_archive=$3
assembly_directory=$4
: "${PDP10_PREFIX:?PDP10_PREFIX must be set}"

tmpbase=${TMPDIR:-$HOME/tmp}
mkdir -p "$tmpbase"
work=$(mktemp -d "$tmpbase/gcc-p10stdio-build-v2.XXXXXXXX")
trap 'rm -rf "$work"' EXIT HUP INT TERM
for name in p10stdio-format-v2 p10stdio-daimos-console-v1; do
    "$PDP10_PREFIX/bin/pdp10-dec-none-gcc" \
        -std=c89 -ffreestanding -fno-builtin -fno-common \
        -march=base -mtune=166 -munextended -mno-string \
        -Os -S "$src/$name.c" -o "$work/$name.s"
    "$PDP10_PREFIX/bin/das" -F -C -O \
        "$work/$name.dobj" "$work/$name.s"
done
"$PDP10_PREFIX/bin/darc" -o "$buffer_archive" \
    "$work/p10stdio-format-v2.dobj"
"$PDP10_PREFIX/bin/darc" -o "$daimos_archive" \
    "$work/p10stdio-daimos-console-v1.dobj"
cp "$work/p10stdio-format-v2.s" \
   "$work/p10stdio-daimos-console-v1.s" "$assembly_directory/"
