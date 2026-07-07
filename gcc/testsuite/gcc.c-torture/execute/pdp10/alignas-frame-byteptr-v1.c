/* PDP-10 regression: taking byte-pointer addresses of aligned automatic
   objects must not leave GCC's virtual frame register in final assembly.  */

volatile int __test_exit;

struct alignas_frame_byteptr_v1_s {
  char a;
  char b __attribute__ ((aligned (2)));
  char c;
  char d __attribute__ ((aligned (4)));
};

int
main (void)
{
  struct alignas_frame_byteptr_v1_s s;

  if ((char *) &s.b - (char *) &s != 2)
    return 1;
  if ((char *) &s.d - (char *) &s != 4)
    return 2;
  return 0;
}
