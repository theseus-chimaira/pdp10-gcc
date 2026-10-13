typedef int char9 __attribute__ ((size (9)));
typedef int short18 __attribute__ ((size (18)));
typedef unsigned uchar8 __attribute__ ((size (8)));
typedef unsigned ushort16 __attribute__ ((size (16)));

struct inner
{
  char6 c6;
  char9 c9;
  short18 h18;
} __attribute__ ((packed));

struct record
{
  int tag;
  struct inner item[2];
  uchar8 bytes[3];
  ushort16 value;
} __attribute__ ((packed));

volatile unsigned int __test_exit;

void
__main (void)
{
}

static struct record
make_record (void)
{
  struct record r;

  r.tag = 0123456;
  r.item[0].c6 = -31;
  r.item[0].c9 = -255;
  r.item[0].h18 = -012345;
  r.item[1].c6 = 30;
  r.item[1].c9 = 254;
  r.item[1].h18 = 012345;
  r.bytes[0] = 0;
  r.bytes[1] = 0377;
  r.bytes[2] = 0123;
  r.value = 65535;
  return r;
}

int
main (void)
{
  struct record a;
  struct record b;

  a = make_record ();
  b = a;

  if (b.value != 65535)
    return 1;
  if (b.bytes[1] != 0377)
    return 2;
  if (b.item[1].h18 != 012345)
    return 3;
  if (b.tag != 0123456)
    return 4;
  return 0;
}
