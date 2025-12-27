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
# 1.17.
#
# The default regeneration scope is the C-only PDP-10 compiler path:
#
#   gcc
#   libiberty
#
# Use --regen-all to regenerate the remaining kept secondary libraries:
#
#   libf2c
#   libf2c/libF77
#   libf2c/libI77
#   libf2c/libU77
#   libobjc
#   libstdc++-v3
#
# The top-level ./configure is old Cygnus configure, not a normal Autoconf
# output.  Keep ./configure and ./configure.in as source.
#
# Pruned components are intentionally not regenerated here:
#
#   Ada
#   Java/GCJ
#   Boehm GC
#   libffi
#   fastjar
#   bundled zlib
#
# Kept components:
#
#   C
#   Objective-C
#   C++
#   Fortran/libf2c
#   libobjc
#   libstdc++-v3
#   libiberty
#
# Generated files are intentionally not source files.  After applying the
# cleanup patches, remove tracked generated files with:
#
#   git rm --cached -- $(cat generated-files-core.lst)
#   cat .gitignore.generated >> .gitignore
#   git clean -fdX

usage () {
	cat <<'EOF'
usage:
  ./build.sh [--regen-only]
  ./build.sh [--regen-all]
  ./build.sh [--no-regen]
  ./build.sh [--help]

modes:
  default       regenerate core autotools files, then configure/build/install
  --regen-only regenerate core autotools files only
  --regen-all  regenerate all kept autotools files only
  --no-regen   skip autotools regeneration and build only

environment:
  PDP10_GCC_TARGET       target triplet, default pdp10-dec-none
  PDP10_GCC_PREFIX       install prefix, default /usr/local
  PDP10_GCC_AS           assembler path
  PDP10_GCC_BUILD_ALIAS  configure --build value
  PDP10_GCC_HOST_ALIAS   configure --host value
  CC                     host C compiler, default cc
  SUDO                   sudo command, default "sudo -n"
  DOAS                   doas command, default "doas"
EOF
}

have_command () {
	set -- $1
	[ $# -gt 0 ] && command -v "$1" >/dev/null 2>&1
}

require_command () {
	if ! command -v "$1" >/dev/null 2>&1; then
		echo "error: required command not found: $1" >&2
		exit 1
	fi
}

autoconf_input () {
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

regen_autoconf_dir () {
	infile=`autoconf_input "$1"` || {
		echo "==> skip autoconf: $1: no configure.ac/configure.in"
		return 0
	}

	require_command autoconf

	echo "==> autoconf: $1 ($infile)"
	(
		cd "$SRC/$1"
		autoconf --output=configure "$infile"
	)
}

regen_automake_dir () {
	if [ ! -f "$SRC/$1/Makefile.am" ]; then
		echo "==> skip automake: $1: no Makefile.am"
		return 0
	fi

	infile=`autoconf_input "$1"` || {
		echo "==> skip automake: $1: no configure.ac/configure.in"
		return 0
	}

	require_command aclocal
	require_command automake

	echo "==> aclocal: $1"
	(
		cd "$SRC/$1"
		aclocal
	)

	if grep 'AC_CONFIG_HEADER\|AC_CONFIG_HEADERS\|AM_CONFIG_HEADER' "$SRC/$1/$infile" >/dev/null 2>&1; then
		require_command autoheader
		echo "==> autoheader: $1 ($infile)"
		(
			cd "$SRC/$1"
			autoheader "$infile"
		)
	fi

	echo "==> automake: $1"
	(
		cd "$SRC/$1"
		automake --add-missing --copy --foreign
	)
}

clean_autom4te_cache () {
	echo "==> clean: autom4te.cache"
	find "$SRC" -type d -name autom4te.cache -prune -exec rm -rf {} +
}

regen_core_autotools () {
	clean_autom4te_cache

	regen_autoconf_dir gcc
	regen_autoconf_dir libiberty
}

regen_all_autotools () {
	clean_autom4te_cache

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

case "${1:-}" in
	--help|-h)
		usage
		exit 0
		;;
	--regen-only)
		REGEN=only
		REGEN_SCOPE=core
		shift
		;;
	--regen-all)
		REGEN=only
		REGEN_SCOPE=all
		shift
		;;
	--no-regen)
		REGEN=no
		shift
		;;
	"")
		:
		;;
	*)
		echo "error: unknown option: $1" >&2
		usage >&2
		exit 1
		;;
esac

HOSTCC="${CC:-cc}"
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
SUDO="${SUDO:-doas}"
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

if [ "$REGEN" != no ]; then
	echo "==> source: $SRC"
	echo "==> build:  $BUILD"
	echo "==> regen:  $REGEN_SCOPE"

	if [ "$REGEN_SCOPE" = all ]; then
		regen_all_autotools
	else
		regen_core_autotools
	fi

	if [ "$REGEN" = only ]; then
		exit 0
	fi
fi

echo "==> target: $TARGET"
echo "==> prefix: $PREFIX"
echo "==> as:     $AS"
echo "==> cc:     $HOSTCC"

if [ ! -d "$PREFIX" ]; then
	echo "==> mkdir:  $PREFIX"

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

echo "==> configure"
CC="$HOSTCC" CFLAGS="$HOSTCFLAGS" "$SRC/configure" $CONFIGURE_HOST \
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
	CC="$HOSTCC" \
	CC_FOR_BUILD="$HOSTCC" \
	CFLAGS="$HOSTCFLAGS" \
	HOST_CFLAGS="$HOSTCFLAGS" \
	BOOT_CFLAGS="$HOSTCFLAGS" \
	WARN_CFLAGS=

echo "==> install"
if [ -w "$PREFIX" ]; then
	make install
elif have_command "$SUDO"; then
	$SUDO make install
elif have_command "$DOAS"; then
	$DOAS make install
else
	make install
fi

echo "==> installed cc1"
ls -l "$PREFIX/lib/gcc-lib/$TARGET/3.2/cc1"
