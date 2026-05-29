struct packed_char_record
{
  char a;
  char b;
  char c;
} __attribute__ ((packed));

static struct packed_char_record x;

void
__main (void)
{
}

int
main (void)
{
  x.a = 1;
  x.b = 2;
  x.c = 3;

  if (x.a != 1)
    return 1;
  if (x.b != 2)
    return 2;
  if (x.c != 3)
    return 3;
  if (x.a + 10 * x.b + 100 * x.c != 321)
    return 4;

  return 0;
}
