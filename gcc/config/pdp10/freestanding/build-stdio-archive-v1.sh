#!/bin/sh
# Build the OS-independent PDP-10 in-memory stdio formatter.
# GCC's actual va_list ABI is required; KCC's sprintf entry is not used.
set -eu

src=$1
output=$2
assembly_output=${3:-}
if [ -z "${PDP10_PREFIX:-}" ]; then
    echo 'PDP10_PREFIX is required' >&2
    exit 2
fi

tmpbase=${TMPDIR:-$HOME/tmp}
mkdir -p "$tmpbase"
work=$(mktemp -d "$tmpbase/gcc-p10stdio-build-v1.XXXXXXXX")
trap 'rm -rf "$work"' EXIT HUP INT TERM

"$PDP10_PREFIX/bin/pdp10-dec-none-gcc" \
    -std=c89 -ffreestanding -fno-builtin -fno-common \
    -march=base -mtune=166 -munextended -mno-string \
    -Os -S "$src/p10stdio-format-v1.c" -o "$work/p10stdio-format-v1.s"
"$PDP10_PREFIX/bin/das" -F -C -O \
    "$work/p10stdio-format-v1.dobj" "$work/p10stdio-format-v1.s"
"$PDP10_PREFIX/bin/darc" -o "$output" "$work/p10stdio-format-v1.dobj"
if [ -n "$assembly_output" ]; then
    cp "$work/p10stdio-format-v1.s" "$assembly_output"
fi
