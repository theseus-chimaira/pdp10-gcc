#!/bin/sh
# GCC PDP-10 -Os block-reordering size regression test.
#
# GCC 3.2 implements -Os as optimize=2 plus optimize_size.  That enables
# -freorder-blocks.  On PDP-10, block reordering can break compact skip
# instruction forms and add unconditional JRST instructions.  For size,
# -Os should keep the compact -O1 branch shape for simple DImode compares.

set -eu

failures=0
cc=${PDP10_GCC:-pdp10-dec-none-gcc}
tmp=${TMPDIR:-/tmp}/pdp10-os-reorder-blocks-$$

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

instruction_count()
{
    awk '
    {
        line = $0
        sub(/;.*/, "", line)
        gsub(/^[ \t]+/, "", line)
        if (line == "")
            next
        while (line ~ /^[A-Za-z_%$][A-Za-z0-9_%$.-]*:[ \t]*/) {
            sub(/^[A-Za-z_%$][A-Za-z0-9_%$.-]*:[ \t]*/, "", line)
            if (line == "")
                next
        }
        if (line == "")
            next
        split(line, a, /[ \t,]+/)
        if (a[1] ~ /^\./)
            next
        if ($0 !~ /^[ \t]/ && line !~ /[ \t,]/)
            next
        count++
    }
    END { print count + 0 }' "$1"
}

cat > "$tmp/os_reorder_blocks.c" <<'EOF_C'
int cmpdi_eq(long long a, long long b)
{
    return a == b;
}

int cmpdi_ne(long long a, long long b)
{
    return a != b;
}

int cmpdi_lt(long long a, long long b)
{
    return a < b;
}

int cmpdi_le(long long a, long long b)
{
    return a <= b;
}

int cmpdi_gt(long long a, long long b)
{
    return a > b;
}

int cmpdi_ge(long long a, long long b)
{
    return a >= b;
}

int cmpdi_ult(unsigned long long a, unsigned long long b)
{
    return a < b;
}

int cmpdi_ule(unsigned long long a, unsigned long long b)
{
    return a <= b;
}

int cmpdi_ugt(unsigned long long a, unsigned long long b)
{
    return a > b;
}

int cmpdi_uge(unsigned long long a, unsigned long long b)
{
    return a >= b;
}
EOF_C

asm_o1=$tmp/os_reorder_blocks_o1.s
asm_os=$tmp/os_reorder_blocks_os.s

common="-fno-builtin -fno-common -munextended -mno-string -mlong-long-71bit -march=ks10 -mtune=ks10"
"$cc" -S -O1 $common "$tmp/os_reorder_blocks.c" -o "$asm_o1"
"$cc" -S -Os $common "$tmp/os_reorder_blocks.c" -o "$asm_os"

o1_count=`instruction_count "$asm_o1"`
os_count=`instruction_count "$asm_os"`

if test "$os_count" -gt "$o1_count"; then
    fail "-Os DImode compare output is larger than -O1: -Os=$os_count -O1=$o1_count"
fi

if test "$failures" -ne 0; then
    note "$failures failure(s)"
    exit 1
fi

note "PASS: -Os keeps compact block order for DImode compares"
