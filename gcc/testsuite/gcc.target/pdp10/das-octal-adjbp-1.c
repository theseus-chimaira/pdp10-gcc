/* { dg-do compile } */
/* { dg-options "-O2 -march=pdp6" } */

typedef unsigned ushort16 __attribute__ ((size (16)));

ushort16 *gp;

void
step (void)
{
  gp = gp + 1;
}

/* A reloaded S=1 logical exact16 pointer advances by 16 one-bit positions.
   DAS parses instruction effective-address constants as octal by default, so
   the decimal value 16 must reach the assembler as explicit octal 020.  */
/* { dg-final { scan-assembler "movei[ \\t]+0,020" } } */
/* { dg-final { scan-assembler-not "movei[ \\t]+0,16([ \\t\\n]|$)" } } */
