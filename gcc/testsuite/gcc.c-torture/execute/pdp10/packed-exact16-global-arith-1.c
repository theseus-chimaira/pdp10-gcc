typedef unsigned ushort16 __attribute__ ((size (16)));

struct outer
{
  unsigned pre:5;
  ushort16 a[3];
  unsigned post:4;
} __attribute__ ((packed));

static struct outer obj;
static ushort16 *gp;
volatile unsigned int __test_exit;

void
__main (void)
{
}

static void __attribute__ ((noinline))
write_at (ushort16 *p, int i, ushort16 v)
{
  p[i] = v;
}

static ushort16 __attribute__ ((noinline))
read_at (ushort16 *p, int i)
{
  return p[i];
}

int
main (void)
{
  ushort16 *p;

  obj.pre = 021;
  obj.post = 013;
  write_at (obj.a, 0, 0111);
  write_at (obj.a, 1, 0222);
  write_at (obj.a, 2, 0333);

  /* Constant address formation must advance an S=1 logical exact16 pointer
     by sixteen bit positions per C element, not by one bit.  */
  p = &obj.a[1];
  if (*p != 0222)
    return 1;

  /* The same representation must survive global storage and arithmetic.  */
  gp = obj.a;
  gp = gp + 1;
  if (*gp != 0222)
    return 2;
  gp[1] = 0444;
  if (read_at (obj.a, 2) != 0444)
    return 3;
  if (obj.pre != 021 || obj.post != 013)
    return 4;
  return 0;
}
