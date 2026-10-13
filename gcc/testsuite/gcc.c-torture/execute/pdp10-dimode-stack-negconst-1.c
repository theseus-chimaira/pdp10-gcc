static long long __attribute__((noinline))
stack5 (int a, int b, int c, int d, long long x)
{
  return x + a + b + c + d;
}

int
main (void)
{
  long long x;

  x = stack5 (1, 2, 3, 4, -70000000001LL);
  if (x != -69999999991LL)
    return 1;
  return 0;
}
