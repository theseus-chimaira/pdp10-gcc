dnl Compatibility helpers for regenerating this GCC 3.2 tree with
dnl modern Autoconf.  GCC 3.2 target-library configure inputs used
dnl private Autoconf 2.13 macros that no longer exist in Autoconf 2.70+.

m4_ifndef([AC_PROG_CC_GNU],
[AC_DEFUN([AC_PROG_CC_GNU],
[AC_CACHE_CHECK([whether we are using GNU C], [ac_cv_prog_gcc],
[cat > conftest.c <<\_ACEOF
#ifdef __GNUC__
  yes;
#endif
_ACEOF
if ${CC-cc} -E conftest.c 2>/dev/null | grep yes >/dev/null 2>&1; then
  ac_cv_prog_gcc=yes
else
  ac_cv_prog_gcc=no
fi
rm -f conftest.c])])])

m4_ifndef([AC_PROG_CC_G],
[AC_DEFUN([AC_PROG_CC_G],
[AC_CACHE_CHECK([whether ${CC-cc} accepts -g], [ac_cv_prog_cc_g],
[echo 'void f(void){}' > conftest.c
if ${CC-cc} -g -c conftest.c >/dev/null 2>&1; then
  ac_cv_prog_cc_g=yes
else
  ac_cv_prog_cc_g=no
fi
rm -f conftest*])])])

m4_ifndef([AC_PROG_CXX_GNU],
[AC_DEFUN([AC_PROG_CXX_GNU],
[AC_CACHE_CHECK([whether we are using GNU C++], [ac_cv_prog_gxx],
[cat > conftest.cc <<\_ACEOF
#ifdef __GNUC__
  yes;
#endif
_ACEOF
if ${CXX-g++} -E conftest.cc 2>/dev/null | grep yes >/dev/null 2>&1; then
  ac_cv_prog_gxx=yes
else
  ac_cv_prog_gxx=no
fi
rm -f conftest.cc])])])

m4_ifndef([AC_PROG_CXX_G],
[AC_DEFUN([AC_PROG_CXX_G],
[AC_CACHE_CHECK([whether ${CXX-g++} accepts -g], [ac_cv_prog_cxx_g],
[echo 'void f(void){}' > conftest.cc
if ${CXX-g++} -g -c conftest.cc >/dev/null 2>&1; then
  ac_cv_prog_cxx_g=yes
else
  ac_cv_prog_cxx_g=no
fi
rm -f conftest*])])])

m4_ifndef([AC_PROG_CC_WORKS], [AC_DEFUN([AC_PROG_CC_WORKS], [:])])
m4_ifndef([AC_PROG_CXX_WORKS], [AC_DEFUN([AC_PROG_CXX_WORKS], [:])])
