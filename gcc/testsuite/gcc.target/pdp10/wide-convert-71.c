/* { dg-do compile } */
/* { dg-options "-O1 -march=pdp6 -mlong-long-71bit" } */
typedef unsigned long long u71;
typedef long long i71;
u71 zu(unsigned int x) { return (u71)x; }
i71 zs(int x) { return (i71)x; }
