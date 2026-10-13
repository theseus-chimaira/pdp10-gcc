typedef unsigned ushort16 __attribute__ ((size (16)));

volatile unsigned int __test_exit;

void
__main (void)
{
}

struct outer
{
  unsigned pre:5;
  ushort16 a[3];
  unsigned post:4;
} __attribute__ ((packed));

static struct outer obj;

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
  obj.pre = 021;
  obj.post = 013;

  wr (obj.a, 0, 0111);
  wr (obj.a, 1, 0222);
  wr (obj.a, 2, 0333);

  if (rd (obj.a, 0) != 0111)
    return 1;
  if (rd (obj.a, 1) != 0222)
    return 2;
  if (rd (obj.a, 2) != 0333)
    return 3;

  wr (obj.a, 1, 0444);
  if (rd (obj.a, 1) != 0444)
    return 4;
  if (obj.pre != 021 || obj.post != 013)
    return 5;

  return 0;
}
