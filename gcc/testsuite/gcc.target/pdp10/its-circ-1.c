/* { dg-do compile } */
/* { dg-options "-O2 -march=ka10 -mits" } */
/* { dg-final { scan-assembler "circ" } } */

#include <pdp10-its.h>

void
circ_pair (unsigned long pair[2], int count)
{
  pdp10_circ (pair, count);
}
