#!/bin/sh
# Build the optional full finite D-format formatter, independently of v2.
set -eu
src=$1
archive=$2
assembly=$3
: "${PDP10_PREFIX:?PDP10_PREFIX must be set}"
tmpbase=${TMPDIR:-$HOME/tmp}
mkdir -p "$tmpbase"
work=$(mktemp -d "$tmpbase/gcc-p10stdio-build-v3.XXXXXXXX")
trap 'rm -rf "$work"' EXIT HUP INT TERM
"$PDP10_PREFIX/bin/pdp10-dec-none-gcc" \
    -std=c99 -ffreestanding -fno-builtin -fno-common \
    -march=base -mtune=166 -munextended -mno-string \
    -Os -S "$src/p10stdio-format-v3.c" -o "$work/p10stdio-format-v3.s"
"$PDP10_PREFIX/bin/das" -F -C -O "$work/p10stdio-format-v3.dobj" "$work/p10stdio-format-v3.s"
"$PDP10_PREFIX/bin/darc" -o "$archive" "$work/p10stdio-format-v3.dobj"
cp "$work/p10stdio-format-v3.s" "$assembly"
