#!/bin/sh
# GCC PDP-10 -Os branch-count loop size regression test.
#
# GCC's generic branch-count loop conversion can replace compact PDP-10
# compare-and-branch loops with extra setup code.  Under -Os, guarded
# pointer loops should keep the smaller ordinary loop shape.

set -eu

cc=${PDP10_GCC:-pdp10-dec-none-gcc}
tmp=${TMPDIR:-/tmp}/pdp10-os-branch-count-$$
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

cat > "$tmp/os_branch_count.c" <<'EOF_C'
extern volatile int sink;

int guarded_sum(p, n)
int *p;
int n;
{
    int *e;
    int s;

    if (n <= 0)
        return 0;

    e = p + n;
    s = 0;

    do {
        s += *p++;
    } while (p != e);

    return s;
}

void guarded_copy(d, s, n)
int *d;
int *s;
int n;
{
    int *e;

    if (n <= 0)
        return;

    e = d + n;
    do {
        *d++ = *s++;
    } while (d != e);

    sink = d[-1];
}
EOF_C

common='-fno-builtin -fno-common -munextended -mno-string -mlong-long-71bit -march=ka10 -mtune=ka10'
asm_os=$tmp/os_branch_count_os.s
asm_bct=$tmp/os_branch_count_bct.s

"$cc" -S -Os $common "$tmp/os_branch_count.c" -o "$asm_os"
"$cc" -S -Os -fbranch-count-reg $common "$tmp/os_branch_count.c" -o "$asm_bct"

os_count=`insn_count "$asm_os"`
bct_count=`insn_count "$asm_bct"`

if test "$os_count" -ge "$bct_count"; then
    fail "-Os branch-count loop output did not shrink: -Os=$os_count forced-bct=$bct_count"
fi

if grep 'doloop_end' "$asm_os" >/dev/null 2>&1; then
    fail "-Os still emits branch-count doloop output"
fi

if test "$failures" -ne 0; then
    note "$failures failure(s)"
    exit 1
fi

note "PASS: -Os avoids branch-count loop growth"
