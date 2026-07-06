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
write_first (ushort16 *p, ushort16 v)
{
  *p = v;
}

int
main (void)
{
  obj.pre = 021;
  obj.post = 013;

  /* Initialize through the already-supported run-time pointer path so this
     regression tests only the global store/reload and optimized dereference.  */
  write_first (obj.a, 012345);

  gp = obj.a;
  if (*gp != 012345)
    return 1;
  if (obj.pre != 021 || obj.post != 013)
    return 2;
  return 0;
}
