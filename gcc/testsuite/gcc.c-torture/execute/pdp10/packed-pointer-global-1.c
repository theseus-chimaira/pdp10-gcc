typedef int Sint __attribute__ ((mode (SI)));
typedef unsigned uSint __attribute__ ((mode (SI)));
typedef unsigned uchar8 __attribute__ ((size (8)));

volatile uSint __test_exit;
volatile uSint fail_id;
volatile uSint sink;

static Sint target;

struct rec
{
  uchar8 lead;
  Sint *p;
  uchar8 tail;
} __attribute__ ((packed));

static struct rec g;

static int
check_int (int got, int exp, int id)
{
  if (got != exp)
    {
      fail_id = (uSint) id;
      sink = (uSint) got;
      return 0;
    }
  return 1;
}

void
__main (void)
{
}

int
main (void)
{
  Sint *q;

  fail_id = 0;
  target = 0654321;
  g.lead = (uchar8) 0345;
  g.p = &target;
  g.tail = (uchar8) 0256;

  q = g.p;
  if (!check_int ((int) *q, 0654321, 1))
    return (int) fail_id;
  if (!check_int ((int) g.lead, 0345, 2))
    return (int) fail_id;
  if (!check_int ((int) g.tail, 0256, 3))
    return (int) fail_id;

  fail_id = 0;
  return 0;
}
