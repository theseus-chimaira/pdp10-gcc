int
main(void)
{
  volatile unsigned long long un = 590295810358829108501ULL;
  volatile unsigned long long ud = 34359750713ULL;
  volatile long long sn = -70000000001LL;
  volatile long long sd = 7LL;

  if (un / ud != 17179863011ULL)
    return 1;
  if (un % ud != 17379531658ULL)
    return 2;
  if (sn / sd != -10000000000LL)
    return 3;
  if (sn % sd != -1LL)
    return 4;
  return 0;
}
