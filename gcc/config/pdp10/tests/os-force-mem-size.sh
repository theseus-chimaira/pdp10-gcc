#!/bin/sh
# GCC PDP-10 -Os force-mem size regression test.
#
# GCC 3.2 implements -Os as optimize=2 plus optimize_size.  That enables
# -fforce-mem unless the target disables it.  On PDP-10, forcing a reused
# two-word memory operand into pseudos may keep it live across calls in
# callee-saved ACs.  The resulting save/restore frame is larger than two
# explicit reloads.

set -eu

failures=0
cc=${PDP10_GCC:-pdp10-dec-none-gcc}
tmp=${TMPDIR:-/tmp}/pdp10-os-force-mem-$$

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

forbid_pattern()
{
    file=$1
    pattern=$2
    message=$3
    if grep -E "$pattern" "$file" >/dev/null 2>&1; then
        fail "$message"
    fi
}

need_pattern()
{
    file=$1
    pattern=$2
    message=$3
    if ! grep -E "$pattern" "$file" >/dev/null 2>&1; then
        fail "$message"
    fi
}

need_count_at_least()
{
    file=$1
    pattern=$2
    minimum=$3
    message=$4
    count=`grep -E "$pattern" "$file" 2>/dev/null | wc -l | sed 's/[[:space:]]//g'`
    if test "$count" -lt "$minimum"; then
        fail "$message"
    fi
}

cat > "$tmp/os_force_mem.c" <<'EOF_C'
double dneg = -6.0;
double dtwo = 2.0;
int fail_id;
int got;
int got1;

int main(void)
{
    fail_id = 0;
    got = (int)(dneg / dtwo);
    if (got != -3) {
        fail_id = 1;
        return 1;
    }
    got1 = (int)(6.0 / dtwo);
    if (got1 != 3) {
        fail_id = 2;
        return 1;
    }
    return 0;
}
EOF_C

asm=$tmp/os_force_mem.s
"$cc" -S -Os -fno-builtin -fno-common \
    -munextended -mno-string -mlong-long-71bit \
    -march=base "$tmp/os_force_mem.c" -o "$asm"

forbid_pattern "$asm" '^[[:space:]]*add[[:space:]]+17,\[' \
    "$asm grows a save/restore frame under -Os"
forbid_pattern "$asm" '^[[:space:]]*movem[[:space:]]+1[0-7],' \
    "$asm saves callee-saved ACs for forced memory operands"
need_count_at_least "$asm" '^[[:space:]]*move[[:space:]]+3,dtwo' 2 \
    "$asm should reload dtwo directly for both calls"

if test "$failures" -ne 0; then
    note "$failures failure(s)"
    exit 1
fi

note "PASS: -Os does not force memory operands into callee-saved ACs"
