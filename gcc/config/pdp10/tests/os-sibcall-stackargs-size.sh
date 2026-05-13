#!/bin/sh
# GCC PDP-10 -Os sibling-call stack-argument size regression test.
#
# A sibling call from a function that itself has incoming stack arguments can
# be larger on PDP-10 than an ordinary PUSHJ call because the backend has to
# preserve and reshuffle ACs around the tail jump.  For -Os, keep the ordinary
# call shape in that case.

set -eu

CC=${CC:-pdp10-dec-none-gcc}
TMPDIR=${TMPDIR:-/tmp}
TMP=${TMPDIR}/pdp10-os-sibcall-stackargs-size.$$
SRC=$TMP.c
ASM=$TMP.s
trap 'rm -f "$SRC" "$ASM"' 0 1 2 3 15

cat > "$SRC" <<'EOSRC'
struct pair {
    long long a;
    long long b;
};

extern struct pair make_pair(int a, int b, int c, int d, int e, int f,
                             int g, int h, int i);

struct pair call_with_stack_args(int a, int b, int c, int d, int e, int f,
                                 int g, int h, int i)
{
    return make_pair(a, b, c, d, e, f, g, h, i);
}
EOSRC

"$CC" -S -Os -march=ka10 -mtune=ka10 -munextended -mno-string \
    -mlong-long-71bit -fno-builtin -o "$ASM" "$SRC"

if grep 'jrst[ 	][ 	]*make_pair' "$ASM" >/dev/null 2>&1; then
    echo "FAIL: -Os used larger sibling call for stack-argument function" >&2
    exit 1
fi
if ! grep 'pushj[ 	][ 	]*17,make_pair' "$ASM" >/dev/null 2>&1; then
    echo "FAIL: expected ordinary PUSHJ call to make_pair" >&2
    exit 1
fi

echo "PASS: -Os avoids sibling-call growth for stack-argument functions"
