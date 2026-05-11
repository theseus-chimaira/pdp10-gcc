#!/bin/sh
set -eu

CC=${CC:-pdp10-dec-none-gcc}
AS=${AS:-pdp10-dec-none-as}
TMPDIR=${TMPDIR:-/tmp}
TMP=${TMPDIR}/pdp10-jcry0-adddi-asm.$$
SRC=$TMP.c
ASM=$TMP.s
OUT=$TMP.out
trap 'rm -f "$SRC" "$ASM" "$OUT"' 0 1 2 3 15

cat > "$SRC" <<'EOSRC'
long long add_ll(long long a, long long b)
{
    return a + b;
}
EOSRC

"$CC" -S -Os -march=ka10 -mtune=ka10 -munextended -mno-string \
    -mlong-long-71bit -o "$ASM" "$SRC"
if ! grep 'jcry0' "$ASM" >/dev/null 2>&1; then
    echo "FAIL: expected compact jcry0 carry sequence" >&2
    exit 1
fi
if grep 'jcry0[  ][  ]*\[' "$ASM" >/dev/null 2>&1; then
    echo "FAIL: compiler must not depend on JCRY0 instruction literals" >&2
    exit 1
fi
if grep 'tlc\|caml' "$ASM" >/dev/null 2>&1; then
    echo "FAIL: expected direct carry test, not sign-bit compare carry" >&2
    exit 1
fi
"$AS" --start 0 -o "$OUT" "$ASM"
echo "PASS: GCC -Os DImode add carry emits assembler-supported JCRY0"
