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

/* The map is 145 PDP-10 words = 580 target bytes.  DAS reservations are
   expressed in words, so the active GAS path must emit BLOCK 0221 rather
   than .space 580.  */
/* { dg-final { scan-assembler "region_map:\\n[ \\t]*BLOCK[ \\t]+221" } } */
/* { dg-final { scan-assembler "mres_member:\\n[ \\t]*BLOCK[ \\t]+4" } } */
/* { dg-final { scan-assembler-not "\\.space[ \\t]+580" } } */
