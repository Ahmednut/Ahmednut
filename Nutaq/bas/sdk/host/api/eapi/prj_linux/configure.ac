#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([bas_sdk], [1.0], [support@nutaq.com])
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(bas_sdk,1.0)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/socket.h termios.h unistd.h windows.h winsock2.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for libraries.
AC_CHECK_LIB([efence], [EF_Print])
AC_CHECK_LIB([rt], [clock_gettime])
AC_CHECK_LIB([ws2_32], [WSAStartup])
AC_CHECK_LIB([rpcrt4], [RpcStringFree])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_LIB([rt], [clock_gettime])
AC_CHECK_LIB([pcap], [pcap_findalldevs])
AC_CHECK_FUNCS([memcpy memset select])
AC_CHECK_FUNCS([clock_gettime])
AC_CHECK_FUNCS([strstr])
AC_CHECK_FUNCS([strtol])
AC_CHECK_FUNCS([strtoul])

#check header
AC_CHECK_HEADERS([limits.h])
AC_CHECK_HEADERS([malloc.h])
AC_CHECK_HEADERS([wchar.h])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
