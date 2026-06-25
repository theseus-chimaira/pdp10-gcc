typedef unsigned long word_t;
typedef unsigned ushort16 __attribute__ ((size (16)));

static ushort16 values[3];

void
__main (void)
{
}

static ushort16 * __attribute__ ((noinline))
roundtrip (ushort16 *p)
{
  word_t raw = (word_t) p;
  return (ushort16 *) raw;
}

int
main (void)
{
  ushort16 *p;

  values[0] = 012345;
  values[1] = 054321;
  values[2] = 076543;

  p = roundtrip (&values[1]);
  if (*p != 054321)
    return 1;
  p[1] = 065432;
  if (values[2] != 065432)
    return 2;
  return 0;
}
