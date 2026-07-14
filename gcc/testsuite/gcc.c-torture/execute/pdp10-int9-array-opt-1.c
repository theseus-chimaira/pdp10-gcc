typedef int int9_t __attribute__ ((size (9)));
typedef unsigned int uint9_t __attribute__ ((size (9)));

volatile int r0;

int
main (void)
{
  int9_t s[2];
  uint9_t u[2];

  s[0] = 0377;
  s[1] = 0400;
  u[0] = 0777;
  u[1] = 01000;
  r0 = s[0];
  if (r0 != 0377)
    return 1;
  if (s[1] != -0400)
    return 2;
  if (u[0] != 0777)
    return 3;
  if (u[1] != 0)
    return 4;
  return 0;
}
