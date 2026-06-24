/* Baseline-safe PDP-10 DF negation for freestanding libgcc.
   DF objects occupy two 36-bit words and have 62 bits of precision.
   Reuse the canonical software subtraction implementation so this helper
   remains valid on PDP-6/KA10 as well as later processors.  */

extern double __subdf3(double, double);
extern double __negdf2(double);

double
__negdf2(double a)
{
    return __subdf3(0.0, a);
}
