/* Bare-link regression: 9-bit pointer subtraction uses the %BADL9 table.
   The pdp10-dec-none GCC driver must resolve it from its own libgcc.a.  */
long
pdp10_badl9_link_probe (char *a, char *b)
{
  return a - b;
}
