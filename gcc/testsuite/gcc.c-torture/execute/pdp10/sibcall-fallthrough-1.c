/* Verify that PDP-10 JRST target substitution does not delete a
   sibling-call join block that is also reached by fall-through.  */

static volatile unsigned int got;

static void __attribute__((noinline))
sink(x)
     unsigned int x;
{
  got = x;
}

static void __attribute__((noinline))
f(left, value)
     unsigned int left;
     unsigned int value;
{
  unsigned int spaces;

  spaces = 3;
  if (left + value + spaces < 32)
    spaces = 32 - left - value;
  sink(spaces);
}

int
main()
{
  f(8, 2);
  if (got != 22)
    abort();

  f(29, 1);
  if (got != 3)
    abort();

  return 0;
}
