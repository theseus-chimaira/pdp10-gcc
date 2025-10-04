#!/bin/sh
set -eu

SRC="$HOME/src/gcc-3.2"
BUILD="$SRC/build"
TARGET="pdp10-dec-none"
PREFIX="/usr/local"

# Quiet enough for old GCC internals, but do not use -w here.
# This keeps real PDP-10 backend breakage visible.
DEVCFLAGS="-std=gnu89 -g3 -O0 \
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
-Wno-unused-but-set-variable \
-Wno-format \
-Wno-switch \
-Wno-sign-compare \
-Wno-strict-aliasing \
-Wno-implicit-fallthrough \
-fno-strict-aliasing"

GCC_BUILD="$BUILD/gcc"

test -d "$GCC_BUILD" || {
	echo "missing GCC build dir: $GCC_BUILD" >&2
	echo "run the full configure/build script first" >&2
	exit 1
}

cd "$GCC_BUILD"

echo "forcing PDP-10 backend rebuild..."

rm -f \
	pdp10.o \
	insn-attrtab.o \
	insn-emit.o \
	insn-extract.o \
	insn-output.o \
	insn-peep.o \
	insn-recog.o \
	expr.o \
	expmed.o \
        insn-flags.h insn-codes.h insn-emit.c insn-recog.c insn-output.c insn-opinit.c \
        s-flags s-codes s-emit s-recog s-output s-opinit \
	cc1

# Let the GCC makefile regenerate insn-* files from pdp10.md as needed.
make cc1 \
	CFLAGS="$DEVCFLAGS" \
	HOST_CFLAGS="$DEVCFLAGS" \
	BOOT_CFLAGS="$DEVCFLAGS" \
	WARN_CFLAGS=

doas make install-common install-driver \
	CFLAGS="$DEVCFLAGS" \
	HOST_CFLAGS="$DEVCFLAGS" \
	WARN_CFLAGS=

echo "test harness:"
echo "  CC1=\"$GCC_BUILD/cc1 -quiet -O2\" sh checkall"
echo "or installed:"
echo "  CC1=\"$PREFIX/lib/gcc-lib/$TARGET/3.2/cc1 -quiet -O2\" sh checkall"
