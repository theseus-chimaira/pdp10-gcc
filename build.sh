#!/bin/sh
set -eu

# Minimal PDP-10 GCC 3.2 build helper.
#
# Usage:
#
#   PREFIX=/usr/local ./build.sh
#   PREFIX=/usr/local ./build.sh --regen
#
# Default mode does not regenerate autotools output.
# Use --regen after source edits that require rebuilding generated configure files.
#
# Out-of-tree build:
#
#   cd /path/to/gcc-3.2-pdp10
#   rm -rf ../build-gcc32-pdp10
#   mkdir ../build-gcc32-pdp10
#   cd ../build-gcc32-pdp10
#   PREFIX=/usr/local ../gcc-3.2-pdp10/build.sh
#
# With regeneration:
#
#   cd /path/to/gcc-3.2-pdp10
#   PREFIX=/usr/local ./build.sh --regen

usage()
{
	cat <<'EOF'
usage:
  ./build.sh
  ./build.sh --regen
  ./build.sh --help

environment:
  PREFIX    install prefix, default /usr/local

The target is fixed to pdp10-dec-none.
The assembler is expected at:

  $PREFIX/bin/pdp10-dec-none-as
EOF
}

require_command()
{
	if ! command -v "$1" >/dev/null 2>&1; then
		echo "error: required command not found: $1" >&2
		exit 1
	fi
}

autoconf_input()
{
	if [ -f "$SRC/$1/configure.ac" ]; then
		echo configure.ac
		return 0
	fi

	if [ -f "$SRC/$1/configure.in" ]; then
		echo configure.in
		return 0
	fi

	return 1
}

regen_autoconf_dir()
{
	infile=`autoconf_input "$1"` || {
		echo "==> skip autoconf: $1"
		return 0
	}

	require_command autoconf

	echo "==> autoconf: $1"
	(
		cd "$SRC/$1"
		autoconf --output=configure "$infile"
		chmod a+x configure
	)
}

regen()
{
	echo "==> clean: autom4te.cache"
	find "$SRC" -type d -name autom4te.cache -prune -exec rm -rf {} +

	regen_autoconf_dir gcc
	regen_autoconf_dir libiberty
}

DO_REGEN=no

case "${1:-}" in
	"")
		;;
	--regen)
		DO_REGEN=yes
		shift
		;;
	--help|-h)
		usage
		exit 0
		;;
	*)
		echo "error: unknown option: $1" >&2
		usage >&2
		exit 1
		;;
esac

if [ $# -ne 0 ]; then
	echo "error: too many arguments" >&2
	usage >&2
	exit 1
fi

SRC="$(cd "$(dirname "$0")" && pwd)"
BUILD="$(pwd)"

TARGET=pdp10-dec-none
PREFIX="${PREFIX:-/usr/local}"
AS="$PREFIX/bin/$TARGET-as"
CC=cc

CFLAGS="-std=gnu89 -g3 -O0 -w \
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

echo "==> source: $SRC"
echo "==> build:  $BUILD"
echo "==> prefix: $PREFIX"
echo "==> target: $TARGET"
echo "==> as:     $AS"

if [ "$DO_REGEN" = yes ]; then
	regen
fi

if [ ! -x "$AS" ]; then
	echo "error: target assembler is not executable: $AS" >&2
	echo "       install the PDP-10 binutils under PREFIX first" >&2
	exit 1
fi

mkdir -p "$PREFIX"

echo "==> configure"
CC="$CC" CFLAGS="$CFLAGS" "$SRC/configure" \
	--target="$TARGET" \
	--prefix="$PREFIX" \
	--enable-languages=c \
	--with-as="$AS" \
	--disable-nls \
	--without-headers \
	--disable-shared \
	--disable-multilib

echo "==> make all-gcc"
make all-gcc \
	CC="$CC" \
	CC_FOR_BUILD="$CC" \
	CFLAGS="$CFLAGS" \
	HOST_CFLAGS="$CFLAGS" \
	BOOT_CFLAGS="$CFLAGS" \
	WARN_CFLAGS=

echo "==> make install"
make install

echo "==> installed cc1"
ls -l "$PREFIX/lib/gcc-lib/$TARGET/3.2/cc1"
