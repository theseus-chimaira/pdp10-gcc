/* { dg-do compile } */
/* { dg-options "-O2" } */

int
pdp10_char6_cast_word (void)
{
  return *((int *)((char6 *)"abc123"));
}

/* The cast retargets the literal to SIXBIT storage before decay.  */
/* { dg-final { scan-assembler "\\.word[ \\t]+0414243212223" } } */
