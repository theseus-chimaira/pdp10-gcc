#!/bin/sh
# GCC PDP-10 -Os multiply-cost size regression test.
#
# PDP-10 MUL/MULI produces a doubleword result and can clobber AC+1.
# For packed byte-pointer index scaling by 3, choosing MULI under -Os can
# increase register pressure enough to grow a stack frame.  The size model
# should prefer the shift/add form for this case.

set -eu

failures=0
cc=${PDP10_GCC:-pdp10-dec-none-gcc}
tmp=${TMPDIR:-/tmp}/pdp10-os-mul-cost-$$

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

forbid_mnemonic()
{
    file=$1
    mnemonic=$2
    if grep -E "(^|[[:space:]])$mnemonic([[:space:],]|$)" "$file" >/dev/null 2>&1; then
        fail "$file unexpectedly uses mnemonic $mnemonic"
    fi
}

need_mnemonic()
{
    file=$1
    mnemonic=$2
    if ! grep -E "(^|[[:space:]])$mnemonic([[:space:],]|$)" "$file" >/dev/null 2>&1; then
        fail "$file lacks mnemonic $mnemonic"
    fi
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

cat > "$tmp/os_mul_cost.c" <<'EOF_C'
struct __attribute__((packed)) s3 {
    unsigned char a;
    unsigned short b;
};

extern struct s3 gr[16];

void init_one(int i, int v)
{
    gr[i].a = v + 1;
    gr[i].b = v + 2;
    gr[i + 1].a = v + 3;
    gr[i + 1].b = v + 4;
}

int fold_one(int i)
{
    return gr[i].a + gr[i].b + gr[i + 1].a + gr[i + 1].b;
}
EOF_C

asm=$tmp/os_mul_cost.s
"$cc" -S -Os -fno-builtin -fno-common \
    -munextended -mno-string -mlong-long-71bit \
    -march=base "$tmp/os_mul_cost.c" -o "$asm"

forbid_mnemonic "$asm" muli
need_mnemonic "$asm" lsh
need_mnemonic "$asm" add
forbid_pattern "$asm" '^[[:space:]]*add[[:space:]]+17,\[' \
    "$asm uses a stack-frame growth pattern from the old MULI choice"

if test "$failures" -ne 0; then
    note "$failures failure(s)"
    exit 1
fi

note "PASS: -Os uses shift/add scaling and avoids MULI frame growth"
