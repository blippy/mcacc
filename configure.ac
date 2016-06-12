# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([mca], [2.0], [alt.mcarter@gmail.com])
AC_CONFIG_SRCDIR([src/mcacc.cc])
AC_CONFIG_MACRO_DIR([m4])
#AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
#PKG_PROG_PKG_CONFIG
#AC_MSG_NOTICE([Let's rock and roll])

# Externally set CFLAGS
#EXTERNAL_CFLAGS="$CFLAGS"

AC_PROG_CXX
#AC_PROG_FC([gfortran], [Fortran 90])

# Reset the externally set CFLAGS after calling AC_PROG*
#CFLAGS="$EXTERNAL_CFLAGS"


# Use the C language and compiler
#AX_CXX_COMPILE_STDCXX_14
#AC_LANG([CXX])
#AC_LANG([Fortran])
#AC_FC_SRCEXT([f90])


# Wrapping it up
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
