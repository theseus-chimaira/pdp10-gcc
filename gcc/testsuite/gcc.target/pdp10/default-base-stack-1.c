/* { dg-do compile } */
/* { dg-options "-Os" } */

extern void pdp10_default_base_sink (int *);

int
pdp10_default_base_frame (int x)
{
  int local = x;

  pdp10_default_base_sink (&local);
  return local;
}

/* A compiler configured without --with-cpu defaults to the PDP-6/166
   base profile.  Its stack adjustment must therefore use local-addressing
   instructions rather than KL10 extended-addressing XMOVEI.  */
/* { dg-final { scan-assembler "add[ \\t]+017" } } */
/* { dg-final { scan-assembler-not "xmovei[ \\t]+017" } } */
