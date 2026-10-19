#!/bin/sh
# Build the freestanding PDP-6 C library with the target assembler/archiver.
# This is deliberately independent of KCC, DAIMOS, and GCC's hosted libc.
set -eu

src=$1
output=$2
if [ -z "${PDP10_PREFIX:-}" ]; then
    echo 'PDP10_PREFIX is required' >&2
    exit 2
fi

tmpbase=${TMPDIR:-$HOME/tmp}
mkdir -p "$tmpbase"
work=$(mktemp -d "$tmpbase/gcc-p10c-v2.XXXXXXXX")
trap 'rm -rf "$work"' EXIT HUP INT TERM

set --
for name in kcc-libc-memcpy-v1 kcc-libc-memmove-v1 \
            kcc-libc-memset-v1 kcc-libc-memcmp-v2 \
            kcc-libc-strlen-v1 kcc-libc-strcmp-v1 \
            kcc-libc-strncmp-v1 \
            kcc-libc-strstr-v1 kcc-libc-strpbrk-v1 kcc-libc-strspn-v1 \
            p10bytes-v2 kcc-libc-string-v1; do
    object="$work/$name.dobj"
    "$PDP10_PREFIX/bin/das" -F -C -O "$object" "$src/$name.s"
    set -- "$@" "$object"
done
"$PDP10_PREFIX/bin/darc" -o "$output" "$@"
