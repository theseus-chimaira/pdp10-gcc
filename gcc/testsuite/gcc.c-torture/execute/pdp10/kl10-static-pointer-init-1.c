static int object;

static int
f(void)
{
  return 1;
}

static int *object_pointer = &object;
static int (*function_pointer)(void) = f;
static int (*function_table[2])(void) = { f, f };

int
main(void)
{
  *object_pointer = 7;
  if (object != 7)
    __builtin_abort();
  if ((*function_pointer)() != 1)
    __builtin_abort();
  if ((*function_table[0])() != 1 || (*function_table[1])() != 1)
    __builtin_abort();
  return 0;
}
