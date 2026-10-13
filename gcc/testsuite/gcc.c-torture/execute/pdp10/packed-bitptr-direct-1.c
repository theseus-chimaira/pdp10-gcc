typedef unsigned ushort16 __attribute__ ((size (16)));

struct rec
{
  unsigned pre : 5;
  ushort16 a[3];
  unsigned post : 4;
} __attribute__ ((packed));

static struct rec x;

void __main (void) {}
void abort (void) { for (;;) ; }

int
main (void)
{
  x.pre = 017;
  x.post = 012;
  x.a[0] = 012345;
  x.a[1] = 054321;
  x.a[2] = 076543;

  if (x.a[0] != 012345) abort ();
  if (x.a[1] != 054321) abort ();
  if (x.a[2] != 076543) abort ();

  x.a[1] += 7;
  if (x.a[1] != 054330) abort ();
  if (x.pre != 017 || x.post != 012) abort ();
  return 0;
}
