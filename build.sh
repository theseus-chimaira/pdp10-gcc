#!/bin/sh
set -eu

HOSTCFLAGS="-std=gnu89 -g3 -O0 -w \
-fno-strict-aliasing \
-Wno-implicit-int \
-Wno-implicit-function-declaration \
-Wno-int-conversion \
-Wno-incompatible-pointer-types \
-Wno-error \
-Wno-pedantic \
-Wno-traditional \
-Wno-enum-compare \
-Wno-enum-conversion \
-Wno-unused-variable \
-Wno-unused-function \
-Wno-format"

SRC="$(cd "$(dirname "$0")" && pwd)"
BUILD="$SRC/build"
TARGET="pdp10-dec-none"
PREFIX="/usr/local"

mkdir -p "$BUILD" "$PREFIX"
cd "$BUILD"
CC=gcc CFLAGS="$HOSTCFLAGS" "$SRC/configure" \
	--target="$TARGET" \
	--prefix="$PREFIX" \
	--enable-languages=c \
	--with-as="/usr/local/bin/pdp10-dec-none-as" \
	--disable-nls \
	--without-headers \
	--disable-shared \
	--disable-multilib

make all-gcc \
	CFLAGS="$HOSTCFLAGS" \
	HOST_CFLAGS="$HOSTCFLAGS" \
	BOOT_CFLAGS="$HOSTCFLAGS" \
	WARN_CFLAGS=
doas make install
exit 0
ls -l "$PREFIX/lib/gcc-lib/$TARGET/3.2/cc1"
