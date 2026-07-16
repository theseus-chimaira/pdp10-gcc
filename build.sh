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
# Some archive/extraction paths can preserve the configure file contents but
# drop execute bits on generated configure scripts below the tree.  A direct
# sub-configure then fails as "Permission denied" or the build appears to
# stall while retrying old configure probes.  This is a checkout/archive-mode
# problem, not a PDP-10 backend problem.  Before a serious build, normalize
# generated script permissions in the extracted source tree, for example:
#
#   find . -name configure -type f -exec chmod a+x {} +
#
# Running a sub-configure through "sh path/to/configure" is also a safe
# diagnostic workaround, but the normal build expects executable configure
# scripts.
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
  PDP10_PREFIX           required PDP-10 tool/install root
  PDP10_GCC_TARGET       target triplet, default pdp10-dec-none
  PREFIX                 install prefix override, default $PDP10_PREFIX
  PDP10_GCC_AS           assembler override, default $PDP10_PREFIX/bin/$PDP10_GCC_TARGET-as
  CC                     host C compiler, default cc
  PDP10_GCC_BUILD_ALIAS  configure --build value
  PDP10_GCC_HOST_ALIAS   configure --host value
                         octeon-unknown-openbsd7.8/7.9 are treated as mips64 OpenBSD
  DOAS                   doas command, default "doas"
  SUDO                   sudo command, default "sudo -n"
  PDP10_GCC_INSTALL_PRIV install privilege mode: auto, always, never; default auto
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
UNAME_S="$(uname -s 2>/dev/null || echo unknown)"
UNAME_M="$(uname -m 2>/dev/null || echo unknown)"
UNAME_R="$(uname -r 2>/dev/null || echo unknown)"

if [ -z "${PDP10_PREFIX:-}" ]; then
	echo "error: PDP10_PREFIX must be set" >&2
	exit 2
fi

: "${PREFIX:=$PDP10_PREFIX}"
IS_ANDROID=no
if [ -d /system ] || [ "$(uname -o 2>/dev/null || true)" = Android ]; then
	IS_ANDROID=yes
fi

AS="${PDP10_GCC_AS:-$PDP10_PREFIX/bin/$TARGET-as}"
ASDIR=`dirname "$AS"`
# Keep freshly installed target binutils visible to the old GCC makefiles.
# They invoke tools such as pdp10-dec-none-ar by name when building libgcc.
# Also include the assembler directory, since PREFIX may be overridden while
# the bootstrap tools remain under PDP10_PREFIX.
PATH="$ASDIR:$PDP10_PREFIX/bin:$PREFIX/bin:$PATH"
export PATH
export PDP10_PREFIX
DOAS="${DOAS:-doas}"
SUDO="${SUDO:-sudo -n}"
INSTALL_PRIV="${PDP10_GCC_INSTALL_PRIV:-auto}"
CONFIGURE_HOST=""

canonical_host_alias () {
	case "$1" in
		octeon-unknown-openbsd7.8)
			echo mips64-unknown-openbsd7.8
			;;
		octeon-unknown-openbsd7.9)
			echo mips64-unknown-openbsd7.9
			;;
		octeon-unknown-openbsd*)
			echo "mips64-unknown-openbsd${1#octeon-unknown-openbsd}"
			;;
		*)
			echo "$1"
			;;
	esac
}

if [ -n "${PDP10_GCC_BUILD_ALIAS:-}" ] || [ -n "${PDP10_GCC_HOST_ALIAS:-}" ]; then
	BUILD_ALIAS=`canonical_host_alias "${PDP10_GCC_BUILD_ALIAS:-x86_64-unknown-linux-gnu}"`
	HOST_ALIAS=`canonical_host_alias "${PDP10_GCC_HOST_ALIAS:-${PDP10_GCC_BUILD_ALIAS:-x86_64-unknown-linux-gnu}}"`
	CONFIGURE_HOST="--build=$BUILD_ALIAS --host=$HOST_ALIAS"
elif [ "$UNAME_S" = OpenBSD ] && [ "$UNAME_M" = octeon ]; then
	HOST_ALIAS=`canonical_host_alias "octeon-unknown-openbsd$UNAME_R"`
	CONFIGURE_HOST="--build=$HOST_ALIAS --host=$HOST_ALIAS"
fi

if [ "$IS_ANDROID" = yes ]; then
	if [ -z "$CONFIGURE_HOST" ]; then
		BUILD_ALIAS=`canonical_host_alias "${PDP10_GCC_BUILD_ALIAS:-x86_64-unknown-linux-gnu}"`
		HOST_ALIAS=`canonical_host_alias "${PDP10_GCC_HOST_ALIAS:-x86_64-unknown-linux-gnu}"`
		CONFIGURE_HOST="--build=$BUILD_ALIAS --host=$HOST_ALIAS"
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
if [ -n "$CONFIGURE_HOST" ]; then
	echo "==> host:   $CONFIGURE_HOST"
fi

if [ ! -x "$AS" ]; then
	echo "error: target assembler is not executable: $AS" >&2
	echo "       set PDP10_PREFIX=/path/to/prefix or PDP10_GCC_AS=/path/to/$TARGET-as" >&2
	exit 1
fi

privileged_command () {
	if have_command "$DOAS"; then
		$DOAS "$@"
	elif have_command "$SUDO"; then
		$SUDO "$@"
	else
		"$@"
	fi
}

needs_install_privilege () {
	case "$INSTALL_PRIV" in
		always)
			return 0
			;;
		never)
			return 1
			;;
		auto)
			:
			;;
		*)
			echo "error: invalid PDP10_GCC_INSTALL_PRIV: $INSTALL_PRIV" >&2
			echo "       expected auto, always, or never" >&2
			exit 1
			;;
	esac

	# System prefixes often have mixed ownership: the prefix itself may be
	# writable, while subdirectories such as man/man1 are not.  Prefer a
	# privileged install for those prefixes unless explicitly disabled.
	if [ "`id -u`" = 0 ]; then
		return 1
	fi

	case "$PREFIX" in
		/usr/local|/usr/local/*|/usr/cross|/usr/cross/*)
			return 0
			;;
	esac

	[ ! -w "$PREFIX" ]
}

if [ ! -d "$PREFIX" ]; then
	echo "==> mkdir:  $PREFIX"

	if mkdir -p "$PREFIX" 2>/dev/null; then
		:
	else
		privileged_command mkdir -p "$PREFIX"
	fi
fi

if [ -f Makefile ] && [ -f config.status ] && \
   [ -f gcc/Makefile ] && [ -f libiberty/Makefile ]; then
	echo "==> configure: existing complete build tree; skipping top-level configure"
else
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
fi

make_gcc_args () {
	make \
		CC="$HOSTCC" \
		CC_FOR_BUILD="$HOSTCC" \
		CFLAGS="$HOSTCFLAGS" \
		HOST_CFLAGS="$HOSTCFLAGS" \
		BOOT_CFLAGS="$HOSTCFLAGS" \
		WARN_CFLAGS= "$@"
}

make_libiberty_args () {
	make \
		CC="$HOSTCC" \
		CFLAGS="$HOSTCFLAGS" "$@"
}

top_make_has_target () {
	[ -f Makefile ] && grep "^$1:" Makefile >/dev/null 2>&1
}

echo "==> make all-gcc"
if top_make_has_target all-gcc; then
	make_gcc_args all-gcc
else
	echo "==> top-level all-gcc target missing; using direct gcc/libiberty fallback"
	make_libiberty_args -C libiberty all
	make_gcc_args -C gcc all.cross
fi

echo "==> install"
if needs_install_privilege; then
	INSTALL_WITH_PRIV=yes
else
	INSTALL_WITH_PRIV=no
fi

if [ "$INSTALL_WITH_PRIV" = yes ]; then
	echo "==> install privilege: doas/sudo"
	privileged_command mkdir -p \
		"$PREFIX/bin" \
		"$PREFIX/$TARGET/bin" \
		"$PREFIX/lib/gcc-lib/$TARGET/3.2" \
		"$PREFIX/info" \
		"$PREFIX/man/man1" \
		"$PREFIX/share/info" \
		"$PREFIX/share/man/man1" \
		"$PREFIX/share/man/man7"
else
	mkdir -p \
		"$PREFIX/bin" \
		"$PREFIX/$TARGET/bin" \
		"$PREFIX/lib/gcc-lib/$TARGET/3.2" \
		"$PREFIX/info" \
		"$PREFIX/man/man1" \
		"$PREFIX/share/info" \
		"$PREFIX/share/man/man1" \
		"$PREFIX/share/man/man7"
fi

if top_make_has_target install; then
	if [ "$INSTALL_WITH_PRIV" = yes ]; then
		privileged_command make install
	else
		make install
	fi
else
	echo "==> top-level install target missing; using direct gcc install fallback"
	if [ "$INSTALL_WITH_PRIV" = yes ]; then
		privileged_command make -C gcc install
	else
		make -C gcc install
	fi
fi

# GCC searches its private exec directory for the assembler before PATH.
# Install a relative assembler link there so a relocated PDP10_PREFIX remains
# self-contained even though GCC 3.2 records --with-as as an absolute path.
# Without this link, relocating local.tar.gz can make the driver fall through
# to the host program named "as", which may assemble PDP-10 output incorrectly
# or fail only after compilation.
GCC_EXEC_DIR="$PREFIX/lib/gcc-lib/$TARGET/3.2"
GCC_AS_LINK="$GCC_EXEC_DIR/as"
GCC_AS_TARGET="../../../../bin/$TARGET-as"

if [ "$INSTALL_WITH_PRIV" = yes ]; then
	privileged_command ln -sfn "$GCC_AS_TARGET" "$GCC_AS_LINK"
else
	ln -sfn "$GCC_AS_TARGET" "$GCC_AS_LINK"
fi

# Verify the driver resolves "as" inside the installed prefix.  This tests the
# same lookup path used by normal -c compilations and prevents host-as fallback.
RESOLVED_AS=`"$PREFIX/bin/$TARGET-gcc" -print-prog-name=as`
if [ ! -x "$RESOLVED_AS" ]; then
	echo "error: installed GCC cannot resolve target assembler: $RESOLVED_AS" >&2
	exit 1
fi
case "$RESOLVED_AS" in
	"$PREFIX"/*) : ;;
	*)
		echo "error: installed GCC resolved assembler outside prefix: $RESOLVED_AS" >&2
		exit 1
		;;
esac

# GCC searches the target tooldir before falling back to the host PATH.
# Keep the assembler there so an installed toolchain remains relocatable even
# when configure recorded an absolute --with-as path.
if [ "$AS" = "$PREFIX/bin/$TARGET-as" ]; then
	TARGET_AS_LINK="../../bin/$TARGET-as"
else
	TARGET_AS_LINK="$AS"
fi
if [ "$INSTALL_WITH_PRIV" = yes ]; then
	privileged_command ln -sf "$TARGET_AS_LINK" "$PREFIX/$TARGET/bin/as"
else
	ln -sf "$TARGET_AS_LINK" "$PREFIX/$TARGET/bin/as"
fi

if [ "$INSTALL_WITH_PRIV" = yes ]; then
	privileged_command mkdir -p "$PREFIX/share/gcc-pdp10/config/pdp10"
	privileged_command cp "$SRC/gcc/config/pdp10/libgcc1.s" "$PREFIX/share/gcc-pdp10/config/pdp10/libgcc1.s"
else
	mkdir -p "$PREFIX/share/gcc-pdp10/config/pdp10"
	cp "$SRC/gcc/config/pdp10/libgcc1.s" "$PREFIX/share/gcc-pdp10/config/pdp10/libgcc1.s"
fi

INSTALLED_AS=`"$PREFIX/bin/$TARGET-gcc" -print-prog-name=as`
if [ ! -x "$INSTALLED_AS" ]; then
	echo "error: installed GCC cannot find target assembler: $INSTALLED_AS" >&2
	exit 1
fi

echo "==> installed cc1"
ls -l "$PREFIX/lib/gcc-lib/$TARGET/3.2/cc1"
