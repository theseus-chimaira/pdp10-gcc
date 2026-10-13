/* { dg-do compile } */
/* { dg-options "-O1" } */

struct pair
{
  unsigned long first;
  unsigned long second;
};

struct pair zero_pair = { 0, 0 };
unsigned long following_word;

/* Two 32-bit target longs occupy eight target bytes.  */
/* { dg-final { scan-assembler "zero_pair:\n[ \t]*\.space[ \t]+8" } } */
/* { dg-final { scan-assembler-not "zero_pair:\n[ \t]*\.space[ \t]+2" } } */
