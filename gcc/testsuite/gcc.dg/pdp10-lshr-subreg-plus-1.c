/* PDP-10 regression: do not fold a logical right shift count through a
   SUBREG of the DImode MUL result into indexed LSH addressing.  Reload can
   otherwise lose the canonical low-word value used by the count.  */
/* { dg-do compile { target pdp10-*-* } } */
/* { dg-options "-O2" } */

unsigned long
pdp10_lshr_subreg_plus_1(unsigned long word, unsigned int index)
{
  unsigned int shift;

  shift = 30U - (index % 6U) * 6U;
  return (word >> shift) & 077UL;
}

/* The unsafe combined form was: lsh AC,-036(index-AC).  Keep the count in
   an ordinary SI register instead.  */
/* { dg-final { scan-assembler "subi.*036" } } */
/* { dg-final { scan-assembler-not "lsh.*-036\\(" } } */
