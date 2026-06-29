/* { dg-do compile } */
/* { dg-options "-O2 -march=ka10" } */

int
pdp10_oct_34 (void)
{
  return 34;
}

int
pdp10_oct_35 (void)
{
  return 35;
}

int
pdp10_oct_43 (void)
{
  return 43;
}

/* DAS interprets unprefixed integer literals as decimal.  PDP-10 GCC
   emits assembler integer operands in octal, so values containing only
   decimal digits still require an explicit leading zero.  */
/* { dg-final { scan-assembler "movei[ \\t]+1,042" } } */
/* { dg-final { scan-assembler "movei[ \\t]+1,043" } } */
/* { dg-final { scan-assembler "movei[ \\t]+1,053" } } */
/* { dg-final { scan-assembler-not "movei[ \\t]+1,42([ \\t\\n]|$)" } } */
/* { dg-final { scan-assembler-not "movei[ \\t]+1,43([ \\t\\n]|$)" } } */
/* { dg-final { scan-assembler-not "movei[ \\t]+1,53([ \\t\\n]|$)" } } */
