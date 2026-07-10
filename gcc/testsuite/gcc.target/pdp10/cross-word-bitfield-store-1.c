/* { dg-do compile } */
/* { dg-options "-O0" } */

struct cross_word_bits
{
  unsigned int a : 20;
  unsigned int b : 20;
  unsigned int c : 20;
};

void
set_cross_word_c (struct cross_word_bits *p)
{
  p->c = 0x23456U;
}

/* The preceding field shares the first four bits of c's containing word.
   A whole-word store here would corrupt b.  */
/* { dg-final { scan-assembler "dpb.*POINT 20" } } */
