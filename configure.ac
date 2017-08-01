#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([liq], [1.2.3], [rison@ezfun.cn])
AC_CONFIG_SRCDIR([src/rpc_gen/crpc_gen.cc])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE()
LT_INIT()

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lbase':
# AC_CHECK_LIB([base], [main])
# FIXME: Replace `main' with a function in `-ldl':
# AC_CHECK_LIB([dl], [main])
# FIXME: Replace `main' with a function in `-lprotobuf':
# AC_CHECK_LIB([protobuf], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stddef.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([bzero memset strdup strndup])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/lib/Makefile
                 src/rpc_gen/Makefile
                 src/sol/Makefile
                 ])
AC_OUTPUT
