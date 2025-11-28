#!/bin/sh
set -eu

# Build helper for the PDP-10 GCC 3.2 tree.
#
# Preferred out-of-tree build:
#
#   cd /path/to/gcc-3.2-pdp10
#   ./build.sh --regen-only
#   rm -rf ../build-gcc32-pdp10
#   mkdir -p ../build-gcc32-pdp10
#   cd ../build-gcc32-pdp10
#   ../gcc-3.2-pdp10/build.sh --no-regen
#
# You may also run this script directly from the source tree, but the build
# products will then be mixed into the checkout.  The out-of-tree form above
# keeps generated configure/build output separate from source edits.
#
# Regeneration uses the currently installed Autoconf/Automake.  This tree is
# intended to regenerate with current tools such as Autoconf 2.72 and Automake
# 1.17.  The default regeneration scope is the C-only PDP-10 compiler path:
# gcc and libiberty.  The top-level configure script is old Cygnus
# configure, not an Autoconf input, so it is intentionally kept as source.
#
# Pruned components are intentionally not regenerated here: Ada, Java/GCJ,
# Boehm GC, libffi, fastjar, and bundled zlib.  C, Objective-C, C++,
# Fortran/libf2c, libobjc, libstdc++-v3, and libiberty remain.  Use
# --regen-all to also regenerate the remaining secondary target libraries.
#
# Generated files are intentionally not source files.  After applying the
# cleanup patch, remove tracked generated files with:
#
#   git rm --cached -- $(cat generated-files-core.lst)
#   cat .gitignore.generated >> .gitignore
#   git clean -fdX

regen_autoconf_dir () {
	if [ -f "$SRC/$1/configure.in" ]; then
		echo "==> autoconf: $1"
		(cd "$SRC/$1" && autoconf)
	fi
}

regen_automake_dir () {
	if [ -f "$SRC/$1/Makefile.am" ]; then
		echo "==> aclocal: $1"
		(cd "$SRC/$1" && aclocal)
		if grep 'AC_CONFIG_HEADER\|AC_CONFIG_HEADERS\|AM_CONFIG_HEADER' "$SRC/$1/configure.in" >/dev/null 2>&1; then
			echo "==> autoheader: $1"
			(cd "$SRC/$1" && autoheader)
		fi
		echo "==> automake: $1"
		(cd "$SRC/$1" && automake --add-missing --copy --foreign)
	fi
}

regen_core_autotools () {
	find "$SRC" -type d -name autom4te.cache -prune -exec rm -rf {} +
	regen_autoconf_dir gcc
	regen_autoconf_dir libiberty
}

regen_all_autotools () {
	find "$SRC" -type d -name autom4te.cache -prune -exec rm -rf {} +

	for d in \
		libstdc++-v3
	do
		regen_automake_dir "$d"
	done

	for d in \
		gcc \
		libf2c \
		libf2c/libF77 \
		libf2c/libI77 \
		libf2c/libU77 \
		libiberty \
		libobjc \
		libstdc++-v3
	do
		regen_autoconf_dir "$d"
	done
}

REGEN=yes
REGEN_SCOPE=core
if [ "${1:-}" = "--regen-only" ]; then
	REGEN=only
	shift
elif [ "${1:-}" = "--regen-all" ]; then
	REGEN=only
	REGEN_SCOPE=all
	shift
elif [ "${1:-}" = "--no-regen" ]; then
	REGEN=no
	shift
fi

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

if [ "$REGEN" != no ]; then
	if [ "$REGEN_SCOPE" = all ]; then
		regen_all_autotools
	else
		regen_core_autotools
	fi
	if [ "$REGEN" = only ]; then
		exit 0
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
