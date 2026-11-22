#!/bin/sh
set -eu
src=$1
archive=$2
assembly=$3
: "${PDP10_PREFIX:?PDP10_PREFIX must be set}"
export TMPDIR="${TMPDIR:-$HOME/tmp}"
mkdir -p "$TMPDIR"
work=$(mktemp -d "$TMPDIR/gcc-p10heap-v1.XXXXXXXX")
trap 'rm -rf "$work"' EXIT HUP INT TERM
"$PDP10_PREFIX/bin/pdp10-dec-none-gcc" -std=c89 -ffreestanding \
    -fno-builtin -fno-common -march=base -mtune=166 -munextended \
    -mno-string -Os -S "$src/p10heap-arena-v1.c" -o "$work/p10heap-arena-v1.s"
"$PDP10_PREFIX/bin/das" -F -C -O "$work/p10heap-arena-v1.dobj" \
    "$work/p10heap-arena-v1.s"
"$PDP10_PREFIX/bin/darc" -o "$archive" "$work/p10heap-arena-v1.dobj"
cp "$work/p10heap-arena-v1.s" "$assembly"
