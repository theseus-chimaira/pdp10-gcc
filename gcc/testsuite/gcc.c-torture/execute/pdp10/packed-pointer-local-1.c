typedef unsigned uchar8 __attribute__ ((size (8)));

struct rec
{
  uchar8 lead;
  int *p;
  uchar8 tail;
} __attribute__ ((packed));

static int target;
volatile unsigned int __test_exit;
volatile unsigned int fail_id;

void
__main (void)
{
}

int
main (void)
{
  struct rec x;
  struct rec y;
  int *q;

  fail_id = 0;
  target = 0123456;
  x.lead = (uchar8) 0123;
  x.p = &target;
  x.tail = (uchar8) 0234;

  q = x.p;
  if (*q != 0123456)
    fail_id = 1;
  else if ((int) x.lead != 0123)
    fail_id = 2;
  else if ((int) x.tail != 0234)
    fail_id = 3;
  else
    {
      y = x;
      q = y.p;
      if (*q != 0123456)
        fail_id = 4;
      else if ((int) y.lead != 0123 || (int) y.tail != 0234)
        fail_id = 5;
    }

  return fail_id != 0;
}
