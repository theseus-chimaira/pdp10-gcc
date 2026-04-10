#!/bin/sh
# GCC PDP-10 focused CPU-differing instruction test.
# This is intentionally not a semantic matrix sweep.
# It checks only instructions whose availability changes by CPU profile.

set -eu

failures=0
script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
libgcc1_default=$script_dir/../libgcc1.s
cc=${PDP10_GCC:-pdp10-dec-none-gcc}
libgcc1=${LIBGCC1:-$libgcc1_default}
tmp=${TMPDIR:-/tmp}/pdp10-cpu-diff-$$

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

need_mnemonic()
{
    file=$1
    mnemonic=$2
    if ! grep -E "(^|[[:space:]])$mnemonic([[:space:],]|$)" "$file" >/dev/null 2>&1; then
        fail "$file lacks mnemonic $mnemonic"
    fi
}

forbid_mnemonic()
{
    file=$1
    mnemonic=$2
    if grep -E "(^|[[:space:]])$mnemonic([[:space:],]|$)" "$file" >/dev/null 2>&1; then
        fail "$file unexpectedly uses mnemonic $mnemonic"
    fi
}

need_symbol()
{
    file=$1
    symbol=$2
    if ! grep "$symbol" "$file" >/dev/null 2>&1; then
        fail "$file lacks symbol $symbol"
    fi
}

forbid_symbol()
{
    file=$1
    symbol=$2
    if grep "$symbol" "$file" >/dev/null 2>&1; then
        fail "$file unexpectedly references symbol $symbol"
    fi
}

compile_probe()
{
    source=$1
    march=$2
    output=$tmp/`basename "$source" .c`_$march.s
    "$cc" -S -O2 -fno-builtin -fno-common \
        -munextended -mno-string -mlong-long-71bit \
        -march="$march" "$source" -o "$output"
    printf '%s\n' "$output"
}

forbid_delta_integer()
{
    file=$1
    forbid_mnemonic "$file" dmove
    forbid_mnemonic "$file" dmovem
    forbid_mnemonic "$file" dmovn
    forbid_mnemonic "$file" dmovnm
    forbid_mnemonic "$file" dadd
    forbid_mnemonic "$file" dsub
    forbid_mnemonic "$file" dmul
    forbid_mnemonic "$file" ddiv
}

forbid_delta_float()
{
    file=$1
    forbid_mnemonic "$file" dfad
    forbid_mnemonic "$file" dfsb
    forbid_mnemonic "$file" dfmp
    forbid_mnemonic "$file" dfdv
}

forbid_extended_block()
{
    file=$1
    forbid_mnemonic "$file" xmovei
    forbid_mnemonic "$file" xblt
    forbid_mnemonic "$file" extend
}

cat > "$tmp/di_ops.c" <<'EOF_C'
volatile long long a = 1234567LL;
volatile long long b = 76543LL;
volatile long long sink;

int main(void)
{
    long long x;
    long long y;
    long long z;

    x = a;
    y = b;
    z = x + y;
    if (z != 1311110LL)
        return 1;
    z = z - x;
    if (z != 76543LL)
        return 2;
    z = x * 7LL;
    if (z != 8641969LL)
        return 3;
    z = -z;
    if (z != -8641969LL)
        return 4;
    sink = z;
    return 0;
}
EOF_C

cat > "$tmp/di_muldiv.c" <<'EOF_C'
volatile long long a = 12345LL;
volatile long long b = 77LL;
volatile long long c = 13LL;
volatile long long sink;

int main(void)
{
    long long x;
    long long y;
    long long z;
    long long q;
    long long r;

    x = a;
    y = b;
    z = x * y;
    if (z != 950565LL)
        return 1;
    q = z / c;
    r = z % c;
    if (q != 73120LL)
        return 2;
    if (r != 5LL)
        return 3;
    sink = q + r;
    return 0;
}
EOF_C

cat > "$tmp/df_ops.c" <<'EOF_C'
volatile double da = 6.0;
volatile double db = 3.0;
volatile double dc = 2.0;
volatile double dsink;

int main(void)
{
    double x;
    double y;
    double z;

    x = da;
    y = db;
    z = x + y;
    if (z != 9.0)
        return 1;
    z = z - y;
    if (z != 6.0)
        return 2;
    z = z * y;
    if (z != 18.0)
        return 3;
    z = z / dc;
    if (z != 9.0)
        return 4;
    z = -z;
    if (z != -9.0)
        return 5;
    dsink = z;
    return 0;
}
EOF_C

cat > "$tmp/block_move.c" <<'EOF_C'
struct big {
    int v[16];
};

volatile struct big ga;
volatile struct big gb;

int main(void)
{
    ga = gb;
    return ga.v[0] == gb.v[0] ? 0 : 1;
}
EOF_C

note "GCC-only focused CPU-differing instruction test"

for march in 166 ka10 ki10 ks10; do
    di_ops_s=`compile_probe "$tmp/di_ops.c" "$march"`
    di_muldiv_s=`compile_probe "$tmp/di_muldiv.c" "$march"`
    df_ops_s=`compile_probe "$tmp/df_ops.c" "$march"`
    block_move_s=`compile_probe "$tmp/block_move.c" "$march"`

    case $march in
    166|ka10)
        forbid_delta_integer "$di_ops_s"
        forbid_delta_integer "$di_muldiv_s"
        forbid_delta_float "$df_ops_s"
        need_symbol "$di_muldiv_s" __divdi3
        need_symbol "$di_muldiv_s" __moddi3
        need_symbol "$df_ops_s" __adddf3
        need_symbol "$df_ops_s" __subdf3
        need_symbol "$df_ops_s" __muldf3
        need_symbol "$df_ops_s" __divdf3
        ;;
    ki10)
        need_mnemonic "$di_ops_s" dmove
        need_mnemonic "$di_ops_s" dmovn
        need_mnemonic "$di_muldiv_s" dmove
        forbid_mnemonic "$di_ops_s" dadd
        forbid_mnemonic "$di_ops_s" dsub
        forbid_mnemonic "$di_muldiv_s" dmul
        forbid_mnemonic "$di_muldiv_s" ddiv
        need_symbol "$di_muldiv_s" __divdi3
        need_symbol "$di_muldiv_s" __moddi3
        need_mnemonic "$df_ops_s" dfad
        need_mnemonic "$df_ops_s" dfsb
        need_mnemonic "$df_ops_s" dfmp
        need_mnemonic "$df_ops_s" dfdv
        forbid_symbol "$df_ops_s" __adddf3
        forbid_symbol "$df_ops_s" __muldf3
        ;;
    ks10)
        need_mnemonic "$di_ops_s" dadd
        need_mnemonic "$di_ops_s" dsub
        need_mnemonic "$di_ops_s" dmovn
        need_mnemonic "$di_muldiv_s" dmul
        need_mnemonic "$di_muldiv_s" dadd
        forbid_mnemonic "$di_muldiv_s" ddiv
        need_symbol "$di_muldiv_s" __divdi3
        need_symbol "$di_muldiv_s" __moddi3
        need_mnemonic "$df_ops_s" dfad
        need_mnemonic "$df_ops_s" dfsb
        need_mnemonic "$df_ops_s" dfmp
        need_mnemonic "$df_ops_s" dfdv
        forbid_symbol "$df_ops_s" __adddf3
        forbid_symbol "$df_ops_s" __muldf3
        ;;
    esac

    need_mnemonic "$block_move_s" blt
    forbid_extended_block "$block_move_s"
    note "codegen $march checked"
done

if [ "$failures" -ne 0 ]; then
    note "CPU-differing instruction checks failed: $failures"
    exit 1
fi

p10bare=${P10BARE:-}
if [ -z "$p10bare" ]; then
    p10bare=`command -v p10bare 2>/dev/null || true`
fi

if [ -n "$p10bare" ] && [ -f "$libgcc1" ]; then
    cat > "$tmp/baretest-crt-no-main.s" <<'EOF_S'
        .text
        .globl __start
__start:
        movei 1,__bss_start
__bt_bssclr:
        cail 1,__bss_end
        jrst __bt_bssdone
        setzm 0(1)
        aoja 1,__bt_bssclr
__bt_bssdone:
        movei 17,__bt_stack-1
        pushj 17,main
        movem 1,__test_exit
        halt .
__bt_halt:
        jrst __bt_halt

        .bss
__bss_start:
        .globl __test_exit
__test_exit:
        .block 1
__bt_stack:
        .block 01000
__bss_end:
        .text
EOF_S
    if "$p10bare" --machine ki10 --mode deposit \
        --workdir "$tmp/run" --name cpu_diff_di_muldiv_ki10 \
        --libgcc1 "$libgcc1" --expect __test_exit=0 \
        --gcc-extra=-munextended --gcc-extra=-mno-string \
        --gcc-extra=-mlong-long-71bit \
        "$tmp/baretest-crt-no-main.s" "$tmp/di_muldiv.c" >/dev/null; then
        note "semantic ki10 di_muldiv checked"
    else
        fail "ki10 semantic di_muldiv failed"
    fi
    if "$p10bare" --machine ki10 --mode deposit \
        --workdir "$tmp/run" --name cpu_diff_df_ops_ki10 \
        --libgcc1 "$libgcc1" --expect __test_exit=0 \
        --gcc-extra=-munextended --gcc-extra=-mno-string \
        --gcc-extra=-mlong-long-71bit \
        "$tmp/baretest-crt-no-main.s" "$tmp/df_ops.c" >/dev/null; then
        note "semantic ki10 df_ops checked"
    else
        fail "ki10 semantic df_ops failed"
    fi
else
    note "semantic ki10 run skipped: set P10BARE and LIBGCC1 to enable it"
fi

if [ "$failures" -ne 0 ]; then
    note "CPU-differing instruction tests failed: $failures"
    exit 1
fi

note "CPU-differing instruction tests passed"
