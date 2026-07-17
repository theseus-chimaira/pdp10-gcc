/* { dg-do compile } */
/* { dg-options "-Os" } */

typedef unsigned long pdp10_word;

struct pdp10_blt_payload
{
  pdp10_word word[14];
};

struct pdp10_blt_container
{
  pdp10_word prefix0;
  pdp10_word prefix1;
  pdp10_word prefix2;
  struct pdp10_blt_payload payload;
};

void
pdp10_blt_copy_from_member (struct pdp10_blt_container *source,
                            struct pdp10_blt_payload *destination)
{
  *destination = source->payload;
}

void
pdp10_blt_copy_to_member (struct pdp10_blt_container *destination,
                          struct pdp10_blt_payload *source)
{
  destination->payload = *source;
}

/* The payload starts 12 9-bit C storage units into the container, which is
   three PDP-10 words.  BLT effective addresses must use the word offset.  */
/* { dg-final { scan-assembler "hrli[ \\t]+[0-7]+,3\\(1\\)" } } */
/* { dg-final { scan-assembler-not "hrli[ \\t]+[0-7]+,014\\(1\\)" } } */
/* { dg-final { scan-assembler "movei[ \\t]+[0-7]+,3\\(1\\)" } } */
