/* { dg-do compile } */
/* { dg-options "-O2" } */

char6 pdp10_char6_data[7] = "abc123";
char6 *pdp10_char6_ptr = "abc123";

int
pdp10_char6_index (char6 *p)
{
  return p[2];
}

/* KCC-compatible char6 strings are emitted as PDP-10 SIXBIT.  */
/* { dg-final { scan-assembler "\\.word[ \\t]+0414243212223" } } */
/* char6 pointer indexing uses a 6-bit byte pointer.  */
/* { dg-final { scan-assembler "ildb[ \\t]+1,1" } } */
