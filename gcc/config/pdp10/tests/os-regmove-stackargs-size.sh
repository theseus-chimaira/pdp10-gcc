#!/bin/sh
# GCC PDP-10 -Os regmove stack-argument size regression test.
#
# PDP-10 has only four argument ACs.  When a function has incoming stack
# arguments, the regmove pass can turn a compact stack-argument return path
# into a callee-saved-register path.  That grows -Os code on KA10/PDP-6-class
# output.  Keep this case no larger than -O1.

set -eu

CC=${CC:-pdp10-dec-none-gcc}
TMPDIR=${TMPDIR:-/tmp}
TMP=${TMPDIR}/pdp10-os-regmove-stackargs-size.$$
SRC=$TMP.c
O1ASM=$TMP.o1.s
OSASM=$TMP.os.s
trap 'rm -f "$SRC" "$O1ASM" "$OSASM"' 0 1 2 3 15

cat > "$SRC" <<'EOSRC'
typedef unsigned long long udint_t;
typedef unsigned int usint_t;

static udint_t
mkdi(hi, lo)
     usint_t hi;
     usint_t lo;
{
  return (((udint_t) hi) << 36) | (udint_t) lo;
}

udint_t
all_stack_after_window(a, b, c, d, e)
     int a;
     int b;
     int c;
     int d;
     udint_t e;
{
  if (a != 8 || b != 9 || c != 10 || d != 11)
    return mkdi(0775, (usint_t) (a + b + c + d));
  return e;
}
EOSRC

compile_flags='-march=ka10 -mtune=ka10 -munextended -mno-string -mlong-long-71bit -fno-builtin'
"$CC" -S -O1 $compile_flags -o "$O1ASM" "$SRC"
"$CC" -S -Os $compile_flags -o "$OSASM" "$SRC"

count_insns()
{
  sed -e '/^[ 	]*$/d' \
      -e '/^[ 	]*\./d' \
      -e '/^[A-Za-z0-9_%.$][A-Za-z0-9_%.$]*:[ 	]*$/d' "$1" | wc -l | tr -d ' '
}

o1_count=`count_insns "$O1ASM"`
os_count=`count_insns "$OSASM"`

if grep 'push[ 	][ 	]*17,10' "$OSASM" >/dev/null 2>&1; then
  echo "FAIL: -Os kept the regmove callee-save shape for stack arguments" >&2
  exit 1
fi

if [ "$os_count" -gt "$o1_count" ]; then
  echo "FAIL: -Os stack-argument regmove code grew: O1=$o1_count Os=$os_count" >&2
  exit 1
fi

echo "PASS: -Os avoids regmove growth for stack-argument functions"
