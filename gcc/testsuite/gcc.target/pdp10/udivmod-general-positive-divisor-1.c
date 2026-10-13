/* { dg-do compile } */
/* { dg-options "-O2" } */

void
pdp10_udivmod_general_positive_divisor (unsigned long dividend,
                                        unsigned long divisor,
                                        unsigned long *quotient,
                                        unsigned long *remainder)
{
  *quotient = dividend / divisor;
  *remainder = dividend % divisor;
}

/* The general unsigned divide pattern has a fast path for a nonnegative
   divisor.  PDP-10 assembly integers are octal, so the branch must use +16
   (14 decimal instructions), not +14 (12 decimal instructions).  Landing two
   instructions early skips DIV and leaves the dividend in the remainder.  */
/* { dg-final { scan-assembler "jumpge[ \\t]+[0-7]+,\\.\\+16" } } */
/* { dg-final { scan-assembler-not "jumpge[ \\t]+[0-7]+,\\.\\+14" } } */
