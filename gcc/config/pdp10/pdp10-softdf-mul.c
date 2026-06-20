/* Software PDP-10 D-format multiplication.
   Copyright (C) 2026 Free Software Foundation, Inc.

   A PDP-10 DF value occupies two 36-bit words and has 62 bits of
   precision.  This implementation uses only integer operations valid on
   the pdp10-dec-none baseline.  Eighteen-bit limbs keep each limb product
   within one 36-bit unsigned int.  */

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

double __muldf3(double, double);

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
wide_bit(const uword_t *v, int bit)
{
    return (int)((v[bit / LIMB_BITS] >> (bit % LIMB_BITS)) & 1U);
}

static int
wide_any_below(const uword_t *v, int bit)
{
    int i;
    for (i = 0; i < bit; ++i)
        if (wide_bit(v, i))
            return 1;
    return 0;
}

static void
mul_limbs(const uword_t *a, const uword_t *b, uword_t *r)
{
    int i;
    int j;

    for (i = 0; i < 8; ++i)
        r[i] = 0;

    for (i = 0; i < 4; ++i) {
        uword_t carry;
        carry = 0;
        for (j = 0; j < 4; ++j) {
            int k;
            uword_t prod;
            uword_t sum;
            k = i + j;
            prod = a[i] * b[j];
            sum = r[k] + (prod & LIMB_MASK) + carry;
            r[k] = sum & LIMB_MASK;
            carry = (prod >> LIMB_BITS) + (sum >> LIMB_BITS);
        }
        j = i + 4;
        while (carry != 0 && j < 8) {
            uword_t sum;
            sum = r[j] + (carry & LIMB_MASK);
            r[j] = sum & LIMB_MASK;
            carry = (carry >> LIMB_BITS) + (sum >> LIMB_BITS);
            ++j;
        }
    }
}

static void
extract_rounded62(const uword_t *prod, int shift, uword_t *q)
{
    int i;
    int half;
    int sticky;
    uword_t carry;

    for (i = 0; i < 4; ++i)
        q[i] = 0;
    for (i = 0; i < 62; ++i)
        if (wide_bit(prod, i + shift))
            q[i / LIMB_BITS] |= (uword_t)1U << (i % LIMB_BITS);

    half = wide_bit(prod, shift - 1);
    sticky = wide_any_below(prod, shift - 1);
    if (!half || (!sticky && (q[0] & 1U) == 0))
        return;

    carry = 1U;
    for (i = 0; i < 4 && carry != 0; ++i) {
        uword_t sum;
        sum = q[i] + carry;
        q[i] = sum & LIMB_MASK;
        carry = sum >> LIMB_BITS;
    }
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

static void
q_rshift1(uword_t *q)
{
    int i;
    uword_t carry;
    carry = 0;
    for (i = 3; i >= 0; --i) {
        uword_t next;
        next = q[i] & 1U;
        q[i] >>= 1;
        if (carry)
            q[i] |= 0400000U;
        carry = next;
    }
}

double
__muldf3(double a, double b)
{
    struct df_parts pa;
    struct df_parts pb;
    uword_t prod[8];
    uword_t q[4];
    int shift;
    int exp;
    int sign;

    if (!decode_parts(a, &pa) || !decode_parts(b, &pb)) {
        union df_words z;
        z.w[0] = 0;
        z.w[1] = 0;
        return z.d;
    }

    mul_limbs(pa.limb, pb.limb, prod);
    if (wide_bit(prod, 123)) {
        shift = 62;
        exp = pa.exp + pb.exp - 128;
    } else {
        shift = 61;
        exp = pa.exp + pb.exp - 129;
    }
    extract_rounded62(prod, shift, q);

    /* Rounding may produce exactly 2^62; renormalize it.  */
    if ((q[3] & 0400U) != 0) {
        q_rshift1(q);
        ++exp;
    }

    sign = pa.sign ^ pb.sign;
    return encode_parts(q, exp, sign);
}
