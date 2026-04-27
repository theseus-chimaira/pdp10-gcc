#!/bin/sh
# GCC PDP-10 -Os block-reordering size regression test.
#
# GCC 3.2 implements -Os as optimize=2 plus optimize_size.  That enables
# -freorder-blocks.  A previous PDP-10 -Os tuning disabled block reordering
# globally to make a few DImode compare shapes smaller.  That regressed
# larger loop-heavy cases where -Os should keep the compact -O2 layout.
# Keep this as a guard against disabling the pass globally again; the DImode
# compare case should be fixed with a targeted cbranchdi improvement.

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

script_dir=`dirname "$0"`
pdp10_h=${PDP10_GCC_PDP10_H:-$script_dir/../pdp10.h}

if test -f "$pdp10_h"; then
    if grep 'flag_reorder_blocks[[:space:]]*=[[:space:]]*0' "$pdp10_h" >/dev/null 2>&1; then
        fail "-Os must not disable flag_reorder_blocks globally in pdp10.h"
    fi
fi

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
struct item { int a; int b; int c; };
struct item tab[64];

int loop_mix(int n)
{
    int i;
    int s;

    s = 0;
    for (i = 0; i < n; i++) {
        if (tab[i & 63].a != 0)
            s += tab[i & 63].b;
        else
            s -= tab[i & 63].c;
        if ((s & 7) == 3)
            tab[i & 63].c = s;
    }

    return s;
}

int loop_countdown(int n)
{
    int s;

    s = 0;
    while (n > 0) {
        n--;
        s += tab[n & 63].a;
        if (s > 1000)
            s -= tab[n & 63].b;
    }

    return s;
}
EOF_C

asm_o2=$tmp/os_reorder_blocks_o2.s
asm_os=$tmp/os_reorder_blocks_os.s

common="-fno-builtin -fno-common -munextended -mno-string -mlong-long-71bit -march=ks10 -mtune=ks10"
"$cc" -S -O2 $common "$tmp/os_reorder_blocks.c" -o "$asm_o2"
"$cc" -S -Os $common "$tmp/os_reorder_blocks.c" -o "$asm_os"

o2_count=`instruction_count "$asm_o2"`
os_count=`instruction_count "$asm_os"`

# -Os may still differ slightly from -O2 because optimize_size affects
# other passes.  The important regression guard here is that -Os stays in
# the same compact range instead of falling back to the much larger -O1
# layout by disabling block reordering globally.
limit=`expr "$o2_count" + 1`
if test "$os_count" -gt "$limit"; then
    fail "-Os loop output is not in the compact -O2 range: -Os=$os_count -O2=$o2_count"
fi

if test "$failures" -ne 0; then
    note "$failures failure(s)"
    exit 1
fi

note "PASS: -Os keeps compact block layout for loop-heavy code"
