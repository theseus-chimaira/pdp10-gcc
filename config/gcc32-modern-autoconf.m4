dnl Compatibility helpers for regenerating this GCC 3.2 tree with
dnl modern Autoconf.  GCC 3.2 target-library configure inputs used
dnl private Autoconf 2.13 macros that are obsolete or missing in
dnl Autoconf 2.70+.

dnl Override Autoconf's obsolete compatibility macros.  The GCC 3.2 target
dnl libraries need compile/preprocess-only compiler checks because a bare
dnl cross target may not be able to link target programs yet.

dnl Modern Autoconf compile-test helpers expect ac_objext to be known.
dnl GCC 3.2 used private compiler probes that intentionally avoid the full
dnl AC_PROG_CC/AC_PROG_CXX link test, so set the object suffix explicitly.
AC_DEFUN([GCC32_SET_OBJECT_SUFFIX],
[ac_objext=o])
AC_DEFUN([AC_PROG_CC_GNU],
[AC_CACHE_CHECK([whether we are using GNU C], [gcc32_cv_prog_cc_gnu],
[cat > conftest.c <<\_ACEOF
#ifdef __GNUC__
  yes;
#endif
_ACEOF
if ${CC-cc} -E conftest.c 2>/dev/null | grep yes >/dev/null 2>&1; then
  gcc32_cv_prog_cc_gnu=yes
else
  gcc32_cv_prog_cc_gnu=no
fi
rm -f conftest.c])
GCC32_SET_OBJECT_SUFFIX])

AC_DEFUN([AC_PROG_CC_G],
[AC_CACHE_CHECK([whether ${CC-cc} accepts -g], [gcc32_cv_prog_cc_g],
[echo 'void f(void){}' > conftest.c
if ${CC-cc} -g -c conftest.c >/dev/null 2>&1; then
  gcc32_cv_prog_cc_g=yes
else
  gcc32_cv_prog_cc_g=no
fi
rm -f conftest*])])

AC_DEFUN([AC_PROG_CXX_GNU],
[AC_CACHE_CHECK([whether we are using GNU C++], [gcc32_cv_prog_cxx_gnu],
[cat > conftest.cc <<\_ACEOF
#ifdef __GNUC__
  yes;
#endif
_ACEOF
if ${CXX-g++} -E conftest.cc 2>/dev/null | grep yes >/dev/null 2>&1; then
  gcc32_cv_prog_cxx_gnu=yes
else
  gcc32_cv_prog_cxx_gnu=no
fi
rm -f conftest.cc])
GCC32_SET_OBJECT_SUFFIX])

AC_DEFUN([AC_PROG_CXX_G],
[AC_CACHE_CHECK([whether ${CXX-g++} accepts -g], [gcc32_cv_prog_cxx_g],
[echo 'void f(void){}' > conftest.cc
if ${CXX-g++} -g -c conftest.cc >/dev/null 2>&1; then
  gcc32_cv_prog_cxx_g=yes
else
  gcc32_cv_prog_cxx_g=no
fi
rm -f conftest*])])

m4_ifndef([AC_PROG_CC_WORKS], [AC_DEFUN([AC_PROG_CC_WORKS], [:])])
m4_ifndef([AC_PROG_CXX_WORKS], [AC_DEFUN([AC_PROG_CXX_WORKS], [:])])

dnl Modern replacements for old Autoconf spellings used by GCC 3.2.
AC_DEFUN([AC_TRY_COMPILE],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([$1], [$2])], [$3], [$4])])

AC_DEFUN([AC_TRY_LINK],
[AC_LINK_IFELSE([AC_LANG_PROGRAM([$1], [$2])], [$3], [$4])])

AC_DEFUN([AC_TRY_RUN],
[AC_RUN_IFELSE([AC_LANG_SOURCE([$1])], [$2], [$3], [$4])])

AC_DEFUN([AC_LANG_C], [AC_LANG([C])])
AC_DEFUN([AC_LANG_CPLUSPLUS], [AC_LANG([C++])])
AC_DEFUN([AC_CHECK_TOOL_PREFIX], [:])
AC_DEFUN([AC_ISC_POSIX], [:])
AC_DEFUN([AC_CONFIG_HEADER], [AC_CONFIG_HEADERS([$1])])
AC_DEFUN([AC_FUNC_VFORK], [AC_FUNC_FORK])

AC_DEFUN([AC_TYPE_SIGNAL],
[AC_DEFINE([RETSIGTYPE], [void],
           [Define as the return type of signal handlers.])])

AC_DEFUN([AC_HEADER_TIME],
[AC_CHECK_HEADERS([sys/time.h time.h])
AC_CACHE_CHECK([whether time.h and sys/time.h may both be included],
  [gcc32_cv_header_time],
  [AC_COMPILE_IFELSE(
     [AC_LANG_PROGRAM([[#include <sys/types.h>
#include <sys/time.h>
#include <time.h>]], [[]])],
     [gcc32_cv_header_time=yes],
     [gcc32_cv_header_time=no])])
if test "$gcc32_cv_header_time" = yes; then
  AC_DEFINE([TIME_WITH_SYS_TIME], [1],
            [Define if you can safely include both sys/time.h and time.h.])
fi])

AC_DEFUN([AC_STRUCT_ST_BLKSIZE],
[AC_CHECK_MEMBERS([struct stat.st_blksize],
  [AC_DEFINE([HAVE_ST_BLKSIZE], [1],
             [Define if struct stat has an st_blksize member.])], [],
  [[#include <sys/types.h>
#include <sys/stat.h>]])])

AC_DEFUN([AC_STRUCT_ST_BLOCKS],
[AC_CHECK_MEMBERS([struct stat.st_blocks], [], [],
  [[#include <sys/types.h>
#include <sys/stat.h>]])])

AC_DEFUN([AC_STRUCT_ST_RDEV],
[AC_CHECK_MEMBERS([struct stat.st_rdev],
  [AC_DEFINE([HAVE_ST_RDEV], [1],
             [Define if struct stat has an st_rdev member.])], [],
  [[#include <sys/types.h>
#include <sys/stat.h>]])])
