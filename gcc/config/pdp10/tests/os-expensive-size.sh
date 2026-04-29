#!/bin/sh
# GCC PDP-10 -Os expensive-optimizations size regression test.
#
# GCC 3.2 implements -Os as optimize=2 plus optimize_size.  That enables
# -fexpensive-optimizations.  On PDP-10 this can keep DImode temporaries
# live across calls, use callee-saved ACs, and grow a save/restore frame.
# The -Os result should stay no larger than the simpler -O1 shape for this
# DImode call ABI case.

set -eu

cc=${PDP10_GCC:-pdp10-dec-none-gcc}
tmp=${TMPDIR:-/tmp}/pdp10-os-expensive-$$
failures=0

mkdir "$tmp"
trap 'rm -rf "$tmp"' EXIT HUP INT TERM

note()
{
    printf '%s\n' "$*"
}

fail()
{
    note "FAIL: $*"
    failures=`expr "$failures" + 1`
}

insn_count()
{
    awk '
        /^[[:space:]]*$/ { next }
        /^[[:space:]]*\./ { next }
        /^[A-Za-z_.$%][A-Za-z0-9_.$%]*:[[:space:]]*$/ { next }
        /^[[:space:]]*[A-Za-z_.$%][A-Za-z0-9_.$%]*:[[:space:]]*$/ { next }
        /^[[:space:]]*[^[:space:];!]+/ { n++ }
        END { print n + 0 }
    ' "$1"
}

cat > "$tmp/os_expensive.c" <<'EOF_C'
typedef unsigned int uSint;
typedef long long Dint;

volatile uSint semantic_fail_id;
volatile uSint semantic_sink;

static Dint ga;
static Dint gb;
static Dint gc;

static void fail(id, got)
uSint id;
uSint got;
{
    semantic_fail_id = id;
    semantic_sink = got;
}

static int check_word(p, off, expect, id)
Dint *p;
int off;
uSint expect;
uSint id;
{
    uSint *w;
    w = (uSint *)p;
    if (w[off] != expect) {
        fail(id, w[off]);
        return 0;
    }
    return 1;
}

static Dint id_dint(x)
Dint x;
{
    return x;
}

static Dint second_dint(a, b)
Dint a;
Dint b;
{
    ga = a;
    return b;
}

static void store_three(a, b, c)
Dint a;
Dint b;
Dint c;
{
    ga = a;
    gb = b;
    gc = c;
}

int main()
{
    Dint a;
    Dint b;
    Dint c;
    Dint x;
    uSint *w;

    semantic_fail_id = 0;
    semantic_sink = (uSint)033;

    w = (uSint *)&a;
    w[0] = (uSint)010001;
    w[1] = (uSint)010002;

    w = (uSint *)&b;
    w[0] = (uSint)020001;
    w[1] = (uSint)020002;

    w = (uSint *)&c;
    w[0] = (uSint)030001;
    w[1] = (uSint)030002;

    x = id_dint(a);
    if (!check_word(&x, 0, (uSint)010001, 03000)) return (int)semantic_fail_id;
    if (!check_word(&x, 1, (uSint)010002, 03001)) return (int)semantic_fail_id;

    x = second_dint(a, b);
    if (!check_word(&ga, 0, (uSint)010001, 03002)) return (int)semantic_fail_id;
    if (!check_word(&ga, 1, (uSint)010002, 03003)) return (int)semantic_fail_id;
    if (!check_word(&x, 0, (uSint)020001, 03004)) return (int)semantic_fail_id;
    if (!check_word(&x, 1, (uSint)020002, 03005)) return (int)semantic_fail_id;

    store_three(a, b, c);
    if (!check_word(&ga, 0, (uSint)010001, 03006)) return (int)semantic_fail_id;
    if (!check_word(&ga, 1, (uSint)010002, 03007)) return (int)semantic_fail_id;
    if (!check_word(&gb, 0, (uSint)020001, 03010)) return (int)semantic_fail_id;
    if (!check_word(&gb, 1, (uSint)020002, 03011)) return (int)semantic_fail_id;
    if (!check_word(&gc, 0, (uSint)030001, 03012)) return (int)semantic_fail_id;
    if (!check_word(&gc, 1, (uSint)030002, 03013)) return (int)semantic_fail_id;

    semantic_fail_id = 0;
    semantic_sink = (uSint)1;
    return 0;
}
EOF_C

common='-fno-builtin -fno-common -munextended -mno-string -mlong-long-71bit -march=base'
asm_o1=$tmp/os_expensive_o1.s
asm_os=$tmp/os_expensive_os.s

"$cc" -S -O1 $common "$tmp/os_expensive.c" -o "$asm_o1"
"$cc" -S -Os $common "$tmp/os_expensive.c" -o "$asm_os"

o1_count=`insn_count "$asm_o1"`
os_count=`insn_count "$asm_os"`

if test "$os_count" -gt "$o1_count"; then
    fail "-Os DImode call ABI output is larger than -O1: -Os=$os_count -O1=$o1_count"
fi


if test "$failures" -ne 0; then
    note "$failures failure(s)"
    exit 1
fi

note "PASS: -Os avoids expensive DImode call-frame growth"
