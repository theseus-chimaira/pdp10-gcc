#if defined(TEST_71)
# if !defined(__LONG_LONG_71BIT__) || defined(__LONG_LONG_72BIT__)
#  error wrong 71-bit mode macros
# endif
#endif
#if defined(TEST_72)
# if !defined(__LONG_LONG_72BIT__) || defined(__LONG_LONG_71BIT__)
#  error wrong 72-bit mode macros
# endif
#endif
int wide_int_mode_macro_probe;
