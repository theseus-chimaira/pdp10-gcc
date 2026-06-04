typedef unsigned ushort16 __attribute__ ((size (16)));

struct outer
{
  unsigned pre:5;
  ushort16 a[3];
  unsigned post:4;
} __attribute__ ((packed));

volatile unsigned int __test_exit;

void
__main (void)
{
}

static ushort16
read_first (ushort16 *p)
{
  return *p;
}

static void
write_first (ushort16 *p, ushort16 v)
{
  *p = v;
}

int
main (void)
{
  struct outer x;

  x.pre = 017;
  x.post = 012;
  x.a[0] = 012345;

  if (read_first (x.a) != 012345)
    return 1;
  write_first (x.a, 023456);
  if (read_first (x.a) != 023456)
    return 2;
  if (x.a[0] != 023456)
    return 3;
  if (x.pre != 017 || x.post != 012)
    return 4;
  return 0;
}
