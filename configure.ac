#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libliq], [1.0], [rison@ezfun.cn])
AC_CONFIG_SRCDIR([src/lib/service.cc])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE
LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_CPP
AC_PROG_MKDIR_P

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lbfd':
AC_CHECK_LIB([bfd], [main])
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])
# FIXME: Replace `main' with a function in `-ljsoncpp':
AC_CHECK_LIB([jsoncpp], [main])
# FIXME: Replace `main' with a function in `-lprotobuf':
AC_CHECK_LIB([protobuf], [main])
# FIXME: Replace `main' with a function in `-lprotoc':
AC_CHECK_LIB([protoc], [main])
# FIXME: Replace `main' with a function in `-lshmchannel':
AC_CHECK_LIB([shmchannel], [main])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h stdint.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([bzero clock_gettime localtime_r memset realpath socket strdup strerror])

AC_CONFIG_FILES([Makefile
                 src/lib/Makefile
                 src/rpc_gen/Makefile
                 src/sol/Makefile
                 proto/Makefile
                 test/Makefile
                 test/test_c/Makefile
                 test/test_s/Makefile])
AC_OUTPUT
