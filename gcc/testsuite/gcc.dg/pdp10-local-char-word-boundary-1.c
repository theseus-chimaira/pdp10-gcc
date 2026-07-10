/* { dg-do compile { target pdp10-*-* } } */
/* { dg-options "-O2" } */

extern void touch (char *);

int
probe (int a, int b, int c, int d, int e)
{
  char out[8];

  out[0] = (char) a;
  out[1] = (char) b;
  out[2] = (char) c;
  out[3] = (char) d;
  out[4] = (char) e;
  touch (out);
  return (unsigned char) out[4];
}

/* The fifth 9-bit byte is the first byte of the second 36-bit word.  */
/* { dg-final { scan-assembler "POINT 9,\\(0?17\\),8" } } */
/* { dg-final { scan-assembler-not "POINT 9,\\(0?17\\),17" } } */
