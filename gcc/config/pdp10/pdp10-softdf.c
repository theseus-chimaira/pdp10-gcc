/* Software PDP-10 D-format addition and subtraction.
   Copyright (C) 2026 Free Software Foundation, Inc.

   PDP-10 DFmode has 62 bits of precision and a copied sign bit in the
   continuation word.  PDP-6 and KA10 long floating formats are not the
   same representation, so these routines deliberately use only integer
   word operations and are valid for the oldest pdp10-dec-none baseline.

   The arithmetic result bits follow PDP-10 exponent-field behavior on
   range overflow/underflow: the final exponent is stored modulo 8 bits.
   Hardware also reports floating overflow/underflow in processor status;
   a C libgcc helper has no portable interface for setting those flags.  */

typedef unsigned int uword_t;

#define DF_SIGN 0400000000000UL
#define DF_LOW35 0377777777777UL
#define DF_FRAC27 0777777777UL
#define DF_GUARD_MASK 0377UL
#define DF_GUARD_HALF 0200UL
#define DF_NORM_BIT 0100000000000UL
#define DF_CARRY_BIT 0200000000000UL

struct u72_pair {
    uword_t hi;
    uword_t lo;
};

union df_words {
    double d;
    uword_t w[2];
};

double __adddf3(double, double);
double __subdf3(double, double);

static void
pair_rshift1(struct u72_pair *p)
{
    uword_t carry;
    carry = p->hi & 1U;
    p->hi >>= 1;
    p->lo >>= 1;
    if (carry)
        p->lo |= DF_SIGN;
}

static void
pair_lshift1(struct u72_pair *p)
{
    uword_t carry;
    carry = (p->lo & DF_SIGN) != 0;
    p->lo <<= 1;
    p->hi <<= 1;
    if (carry)
        p->hi |= 1U;
}

static int
pair_cmp(const struct u72_pair *a, const struct u72_pair *b)
{
    if (a->hi < b->hi)
        return -1;
    if (a->hi > b->hi)
        return 1;
    if (a->lo < b->lo)
        return -1;
    if (a->lo > b->lo)
        return 1;
    return 0;
}

static void
pair_add(struct u72_pair *a, const struct u72_pair *b)
{
    uword_t old;
    uword_t carry;
    old = a->lo;
    a->lo += b->lo;
    carry = a->lo < old;
    a->hi += b->hi;
    if (carry)
        a->hi += 1U;
}

static void
pair_sub(struct u72_pair *a, const struct u72_pair *b)
{
    uword_t borrow;
    borrow = a->lo < b->lo;
    a->lo -= b->lo;
    a->hi -= b->hi;
    if (borrow)
        a->hi -= 1U;
}

static void
negate_df(uword_t *hi, uword_t *lo)
{
    *hi = ~*hi;
    *lo = ~*lo;
    *lo &= DF_LOW35;
    *lo += 1U;
    if (*lo & DF_SIGN) {
        *lo &= DF_LOW35;
        *hi += 1U;
    } else {
        *lo &= DF_LOW35;
    }
    *lo |= DF_SIGN;
}

static int
decode_df(double d, struct u72_pair *m, int *exp)
{
    union df_words u;
    uword_t hi;
    uword_t lo;
    uword_t mh;
    int sign;
    int i;

    u.d = d;
    hi = u.w[0];
    lo = u.w[1];
    if (hi == 0 && lo == 0) {
        m->hi = 0;
        m->lo = 0;
        *exp = 0;
        return 0;
    }
    sign = (hi & DF_SIGN) != 0;
    if (sign) {
        hi = ~hi;
        lo = ~lo;
        lo &= DF_LOW35;
        lo += 1U;
        if (lo & DF_SIGN) {
            lo &= DF_LOW35;
            hi += 1U;
        } else {
            lo &= DF_LOW35;
        }
    } else {
        lo &= DF_LOW35;
    }
    *exp = (int)((hi >> 27) & 0377U);
    mh = hi & DF_FRAC27;
    m->hi = mh >> 1;
    m->lo = lo;
    if (mh & 1U)
        m->lo |= DF_SIGN;
    for (i = 0; i < 8; ++i)
        pair_lshift1(m);
    return sign ? -1 : 1;
}

static void
align_right(struct u72_pair *m, int count)
{
    int sticky;
    sticky = 0;
    if (count > 71) {
        if (m->hi != 0 || m->lo != 0)
            m->lo = 1U;
        m->hi = 0;
        return;
    }
    while (count-- > 0) {
        if (m->lo & 1U)
            sticky = 1;
        pair_rshift1(m);
    }
    if (sticky)
        m->lo |= 1U;
}

static double
encode_df(struct u72_pair m, int exp, int sign)
{
    union df_words u;
    uword_t rem;
    uword_t lsb;
    uword_t mh;
    uword_t ml;
    int i;

    if (m.hi == 0 && m.lo == 0) {
        u.w[0] = 0;
        u.w[1] = 0;
        return u.d;
    }

    while ((m.hi & DF_CARRY_BIT) != 0) {
        pair_rshift1(&m);
        ++exp;
    }
    while ((m.hi & DF_NORM_BIT) == 0) {
        pair_lshift1(&m);
        --exp;
    }

    rem = m.lo & DF_GUARD_MASK;
    lsb = (m.lo >> 8) & 1U;
    if (rem > DF_GUARD_HALF || (rem == DF_GUARD_HALF && lsb != 0)) {
        uword_t old;
        old = m.lo;
        m.lo += 0400U;
        if (m.lo < old)
            m.hi += 1U;
        if (m.hi & DF_CARRY_BIT) {
            pair_rshift1(&m);
            ++exp;
        }
    }

    for (i = 0; i < 8; ++i)
        pair_rshift1(&m);

    mh = (m.hi << 1) & DF_FRAC27;
    if (m.lo & DF_SIGN)
        mh |= 1U;
    ml = m.lo & DF_LOW35;
    u.w[0] = ((uword_t)(exp & 0377) << 27) | mh;
    u.w[1] = ml;
    if (sign)
        negate_df(&u.w[0], &u.w[1]);
    return u.d;
}

static double
soft_addsub(double a, double b, int subtract)
{
    struct u72_pair ma;
    struct u72_pair mb;
    int ea;
    int eb;
    int sa;
    int sb;
    int cmp;

    sa = decode_df(a, &ma, &ea);
    sb = decode_df(b, &mb, &eb);
    if (subtract)
        sb = -sb;
    if (sa == 0)
        return encode_df(mb, eb, sb < 0);
    if (sb == 0)
        return encode_df(ma, ea, sa < 0);

    if (ea > eb) {
        align_right(&mb, ea - eb);
        eb = ea;
    } else if (eb > ea) {
        align_right(&ma, eb - ea);
        ea = eb;
    }

    if (sa == sb) {
        pair_add(&ma, &mb);
        return encode_df(ma, ea, sa < 0);
    }

    cmp = pair_cmp(&ma, &mb);
    if (cmp == 0) {
        union df_words z;
        z.w[0] = 0;
        z.w[1] = 0;
        return z.d;
    }
    if (cmp > 0) {
        pair_sub(&ma, &mb);
        return encode_df(ma, ea, sa < 0);
    }
    pair_sub(&mb, &ma);
    return encode_df(mb, ea, sb < 0);
}

double
__adddf3(double a, double b)
{
    return soft_addsub(a, b, 0);
}

double
__subdf3(double a, double b)
{
    return soft_addsub(a, b, 1);
}
