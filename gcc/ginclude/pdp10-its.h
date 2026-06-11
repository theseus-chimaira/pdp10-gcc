#ifndef _PDP10_ITS_H
#define _PDP10_ITS_H 1

#ifndef __PDP10_ITS__
#error "pdp10-its.h requires GCC option -mits"
#endif

/* Raw two-word storage used by the ITS CIRC instruction.  Do not use an
   ordinary long long expression here: the PDP-10 backend may give DImode
   integer arithmetic a representation that is not a raw 72-bit AC pair. */
typedef unsigned int __pdp10_circ_pair_t __attribute__ ((mode (DI)));

static __inline__ void
pdp10_circ (unsigned long pair[2], int count)
{
  __pdp10_circ_pair_t raw;

  raw = *(__pdp10_circ_pair_t *)(void *) pair;
  __asm__ volatile ("circ %0,0(%1)" : "+r" (raw) : "r" (count));
  *(__pdp10_circ_pair_t *)(void *) pair = raw;
}

#endif
