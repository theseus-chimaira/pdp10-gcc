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
BUILD="$(pwd)"
TARGET="${PDP10_GCC_TARGET:-pdp10-dec-none}"
PREFIX="${PDP10_GCC_PREFIX:-/usr/local}"
AS="${PDP10_GCC_AS:-$PREFIX/bin/$TARGET-as}"
CONFIGURE_HOST=""
if [ -n "${PDP10_GCC_BUILD_ALIAS:-}" ] || [ -n "${PDP10_GCC_HOST_ALIAS:-}" ]; then
	CONFIGURE_HOST="--build=${PDP10_GCC_BUILD_ALIAS:-x86_64-unknown-linux-gnu} --host=${PDP10_GCC_HOST_ALIAS:-${PDP10_GCC_BUILD_ALIAS:-x86_64-unknown-linux-gnu}}"
fi

if [ -d /system ] || [ "$(uname -o 2>/dev/null || true)" = Android ]; then
	if [ -z "${PDP10_GCC_PREFIX+x}" ]; then
		PREFIX="$HOME/cross"
	fi
	AS="${PDP10_GCC_AS:-$PREFIX/bin/$TARGET-as}"
	if [ -z "$CONFIGURE_HOST" ]; then
		CONFIGURE_HOST="--build=${PDP10_GCC_BUILD_ALIAS:-x86_64-unknown-linux-gnu} --host=${PDP10_GCC_HOST_ALIAS:-x86_64-unknown-linux-gnu}"
	fi
fi

INSTALL="${PDP10_GCC_INSTALL:-}"
if [ -z "$INSTALL" ]; then
	if [ -w "$PREFIX" ]; then
		INSTALL="make install"
	elif command -v sudo >/dev/null 2>&1; then
		INSTALL="sudo -n make install"
	elif command -v doas >/dev/null 2>&1; then
		INSTALL="doas make install"
	else
		INSTALL="make install"
	fi
fi

mkdir -p "$PREFIX"

CC=gcc CFLAGS="$HOSTCFLAGS" "$SRC/configure" $CONFIGURE_HOST \
	--target="$TARGET" \
	--prefix="$PREFIX" \
	--enable-languages=c \
	--with-as="$AS" \
	--disable-nls \
	--without-headers \
	--disable-shared \
	--disable-multilib

make all-gcc \
	CFLAGS="$HOSTCFLAGS" \
	HOST_CFLAGS="$HOSTCFLAGS" \
	BOOT_CFLAGS="$HOSTCFLAGS" \
	WARN_CFLAGS=
$INSTALL
ls -l "$PREFIX/lib/gcc-lib/$TARGET/3.2/cc1"
