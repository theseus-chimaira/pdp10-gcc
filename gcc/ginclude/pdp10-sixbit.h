#ifndef _PDP10_SIXBIT_H
#define _PDP10_SIXBIT_H 1

#ifndef __PDP10__
#error "pdp10-sixbit.h requires the PDP-10 target"
#endif

/* Return the first six char6 elements of a string literal as one
   PDP-10 word.  Casting the literal to char6 * selects SIXBIT storage;
   the ordinary dereference then reads the packed 36-bit word.  */
#define SIXBIT(name) (*((int *)((char6 *)(name))))

#endif
