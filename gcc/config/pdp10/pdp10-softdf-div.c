/* Software PDP-10 D-format division.
   Copyright (C) 2026 Free Software Foundation, Inc.

   A PDP-10 DF value occupies two 36-bit words and has 62 bits of
   precision.  This implementation uses restoring binary division over
   four 18-bit limbs and therefore requires no KI10-or-later floating
   instructions and no wider integer type.  */

typedef unsigned int uword_t;

#define DF_SIGN 0400000000000UL
#define DF_LOW35 0377777777777UL
#define DF_FRAC27 0777777777UL
#define LIMB_BITS 18
#define LIMB_MASK 0777777U

struct df_parts {
    uword_t limb[4];
    int exp;
    int sign;
};

union df_words {
    double d;
    uword_t w[2];
};

double __divdf3(double, double);

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
decode_parts(double d, struct df_parts *p)
{
    union df_words u;
    uword_t hi;
    uword_t lo;
    uword_t mh;

    u.d = d;
    hi = u.w[0];
    lo = u.w[1];
    if (hi == 0 && lo == 0) {
        p->limb[0] = 0;
        p->limb[1] = 0;
        p->limb[2] = 0;
        p->limb[3] = 0;
        p->exp = 0;
        p->sign = 0;
        return 0;
    }

    p->sign = (hi & DF_SIGN) != 0;
    if (p->sign) {
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

    p->exp = (int)((hi >> 27) & 0377U);
    mh = hi & DF_FRAC27;
    p->limb[0] = lo & LIMB_MASK;
    p->limb[1] = ((lo >> 18) & 0377777U) | ((mh & 1U) << 17);
    p->limb[2] = (mh >> 1) & LIMB_MASK;
    p->limb[3] = (mh >> 19) & 0377U;
    return 1;
}

static int
limb_cmp(const uword_t *a, const uword_t *b)
{
    int i;
    for (i = 3; i >= 0; --i) {
        if (a[i] < b[i])
            return -1;
        if (a[i] > b[i])
            return 1;
    }
    return 0;
}

static void
limb_sub(uword_t *a, const uword_t *b)
{
    int i;
    uword_t borrow;
    borrow = 0;
    for (i = 0; i < 4; ++i) {
        uword_t bi;
        uword_t old;
        bi = b[i] + borrow;
        old = a[i];
        a[i] = (old - bi) & LIMB_MASK;
        borrow = old < bi;
    }
}

static void
limb_lshift1(uword_t *a)
{
    int i;
    uword_t carry;
    carry = 0;
    for (i = 0; i < 4; ++i) {
        uword_t next;
        next = (a[i] >> 17) & 1U;
        a[i] = ((a[i] << 1) & LIMB_MASK) | carry;
        carry = next;
    }
}

static void
limb_rshift1(uword_t *a)
{
    int i;
    uword_t carry;
    carry = 0;
    for (i = 3; i >= 0; --i) {
        uword_t next;
        next = a[i] & 1U;
        a[i] >>= 1;
        if (carry)
            a[i] |= 0400000U;
        carry = next;
    }
}

static void
limb_inc(uword_t *a)
{
    int i;
    uword_t carry;
    carry = 1U;
    for (i = 0; i < 4 && carry != 0; ++i) {
        uword_t v;
        v = a[i] + carry;
        a[i] = v & LIMB_MASK;
        carry = v >> LIMB_BITS;
    }
}

static void
set_qbit(uword_t *q, int bit)
{
    q[bit / LIMB_BITS] |= (uword_t)1U << (bit % LIMB_BITS);
}

static int
round_up(const uword_t *rem, const uword_t *den, int qodd)
{
    uword_t twice[4];
    int i;
    int cmp;

    for (i = 0; i < 4; ++i)
        twice[i] = rem[i];
    limb_lshift1(twice);
    cmp = limb_cmp(twice, den);
    return cmp > 0 || (cmp == 0 && qodd);
}

static double
encode_parts(const uword_t *q, int exp, int sign)
{
    union df_words u;
    uword_t lo;
    uword_t mh;

    lo = q[0] | ((q[1] & 0377777U) << 18);
    mh = ((q[1] >> 17) & 1U) | (q[2] << 1) | (q[3] << 19);
    u.w[0] = ((uword_t)(exp & 0377) << 27) | (mh & DF_FRAC27);
    u.w[1] = lo & DF_LOW35;
    if (sign)
        negate_df(&u.w[0], &u.w[1]);
    return u.d;
}

double
__divdf3(double a, double b)
{
    struct df_parts pa;
    struct df_parts pb;
    uword_t rem[4];
    uword_t q[4];
    int i;
    int first;
    int exp;
    int sign;

    if (!decode_parts(a, &pa)) {
        union df_words z;
        z.w[0] = 0;
        z.w[1] = 0;
        return z.d;
    }
    if (!decode_parts(b, &pb)) {
        /* PDP-10 hardware signals floating divide check here.  There is no
           portable C mechanism for raising that processor condition from a
           libgcc helper.  Keep the operation deterministic for freestanding
           callers; ordinary C code must not depend on division by zero.  */
        union df_words z;
        z.w[0] = 0;
        z.w[1] = 0;
        return z.d;
    }

    for (i = 0; i < 4; ++i) {
        rem[i] = pa.limb[i];
        q[i] = 0;
    }

    if (limb_cmp(pa.limb, pb.limb) >= 0) {
        limb_sub(rem, pb.limb);
        set_qbit(q, 61);
        first = 60;
        exp = pa.exp - pb.exp + 129;
    } else {
        first = 61;
        exp = pa.exp - pb.exp + 128;
    }

    for (i = first; i >= 0; --i) {
        limb_lshift1(rem);
        if (limb_cmp(rem, pb.limb) >= 0) {
            limb_sub(rem, pb.limb);
            set_qbit(q, i);
        }
    }

    if (round_up(rem, pb.limb, (q[0] & 1U) != 0)) {
        limb_inc(q);
        if ((q[3] & 0400U) != 0) {
            limb_rshift1(q);
            ++exp;
        }
    }

    sign = pa.sign ^ pb.sign;
    return encode_parts(q, exp, sign);
}
