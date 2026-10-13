typedef unsigned ushort16 __attribute__ ((size (16)));

volatile unsigned int __test_exit;
volatile unsigned int fail_id;
static ushort16 plain[3];

void
__main (void)
{
}

static ushort16
rd (ushort16 *p, int i)
{
  return p[i];
}

static void
wr (ushort16 *p, int i, ushort16 v)
{
  p[i] = v;
}

int
main (void)
{
  fail_id = 0;
  plain[0] = 012345;
  plain[1] = 054321;
  plain[2] = 065432;

  if (rd (plain, 1) != 054321)
    fail_id = 1;
  else
    {
      wr (plain, 2, 076543);
      if (plain[2] != 076543)
        fail_id = 2;
    }

  return fail_id != 0;
}
