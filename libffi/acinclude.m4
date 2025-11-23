dnl Regenerable libffi local macros.  These used to live only in the
dnl generated aclocal.m4, which made aclocal.m4 non-removable.
AC_DEFUN([AC_COMPILE_CHECK_SIZEOF],
[AC_CHECK_SIZEOF([$1], [], [#include <sys/types.h>
$2])])

AC_DEFUN([AC_C_BIGENDIAN_CROSS],
[AC_C_BIGENDIAN(
  [AC_DEFINE([WORDS_BIGENDIAN], [1], [whether byteorder is bigendian])
   BYTEORDER=4321],
  [BYTEORDER=1234],
  [AC_MSG_ERROR([unknown endianess - sorry, please pre-set ac_cv_c_bigendian])],
  [AC_MSG_ERROR([universal endian builds are not supported here])])
AC_DEFINE_UNQUOTED([BYTEORDER], [$BYTEORDER], [1234 = LIL_ENDIAN, 4321 = BIGENDIAN])])

sinclude(../libtool.m4)
dnl The lines below arrange for aclocal not to bring libtool.m4
dnl AC_PROG_LIBTOOL into aclocal.m4, while still arranging for automake
dnl to add a definition of LIBTOOL to Makefile.in.
ifelse(yes,no,[
AC_DEFUN([AC_PROG_LIBTOOL],)
AC_SUBST(LIBTOOL)
])
