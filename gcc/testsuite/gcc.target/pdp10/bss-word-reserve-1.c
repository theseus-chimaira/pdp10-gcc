/* { dg-do compile } */
/* { dg-options "-O1 -march=pdp6" } */

typedef unsigned int word_t;

struct stripe_map
{
  word_t member_count;
  word_t member_unit[4];
  word_t start[4];
  word_t bad_first[4];
  word_t bad_count[4];
  word_t bad_total;
  word_t bad_run[127];
};

static struct stripe_map region_map;
static word_t mres_member[4];

word_t *map_address (void) { return &region_map.start[0]; }
word_t *member_address (void) { return &mres_member[0]; }

/* The map is 145 PDP-10 words = 580 target bytes.  GCC's GAS output
   hooks pass sizes in target bytes, and DAS .space accepts those same
   decimal target-byte units.  Keep that byte-count contract intact.  */
/* { dg-final { scan-assembler "region_map:\\n[ \\t]*\\.space[ \\t]+580" } } */
/* { dg-final { scan-assembler "mres_member:\\n[ \\t]*\\.space[ \\t]+16" } } */
/* { dg-final { scan-assembler-not "[ \\t]BLOCK[ \\t]" } } */
