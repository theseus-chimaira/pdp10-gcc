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
SUDO="${SUDO:-sudo -n}"
DOAS="${DOAS:-doas}"
CONFIGURE_HOST=""
have_command () {
	set -- $1
	[ $# -gt 0 ] && command -v "$1" >/dev/null 2>&1
}
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

if [ ! -d "$PREFIX" ]; then
	if mkdir -p "$PREFIX" 2>/dev/null; then
		:
	elif have_command "$SUDO"; then
		$SUDO mkdir -p "$PREFIX"
	elif have_command "$DOAS"; then
		$DOAS mkdir -p "$PREFIX"
	else
		mkdir -p "$PREFIX"
	fi
fi

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
if [ -w "$PREFIX" ]; then
	make install
elif have_command "$SUDO"; then
	$SUDO make install
elif have_command "$DOAS"; then
	$DOAS make install
else
	make install
fi
ls -l "$PREFIX/lib/gcc-lib/$TARGET/3.2/cc1"
