/* { dg-do compile } */
/* { dg-options "-O2" } */

#include <pdp10-sixbit.h>

int
pdp10_sixbit_word (void)
{
  return SIXBIT("abc123");
}

/* SIXBIT follows KCC's uppercase-and-minus-040 encoding.  */
/* { dg-final { scan-assembler "\\.word[ \\t]+0414243212223" } } */
