#!/bin/sh
# Package the shared, independently selectable KCC/GCC ABI members.
# Sources are exact copies of their versioned KCC runtime counterparts.
# Do not change the established libp10c-v2.a member list.
set -eu
src=$1
dest=$2
: "${PDP10_PREFIX:?PDP10_PREFIX must be set}"
tmpbase=${TMPDIR:-$HOME/tmp}
mkdir -p "$tmpbase" "$dest"
work=$(mktemp -d "$tmpbase/gcc-p10shared-v1.XXXXXXXX")
trap 'rm -rf "$work"' EXIT HUP INT TERM

build_member()
{
    "$PDP10_PREFIX/bin/das" -F -C -O "$work/$1.dobj" "$src/$1.s"
    "$PDP10_PREFIX/bin/darc" -o "$dest/$2.a" "$work/$1.dobj"
}

build_member kcc-libc-ctype-c-locale-v1 libp10ctype-c-locale-v1
build_member kcc-libc-fp36-pdp6-v1 libp10fp36-pdp6-v1
build_member kcc-libc-fp36-signed-df-pdp6-v1 libp10fp36-signed-df-pdp6-v1
