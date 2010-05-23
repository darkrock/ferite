dnl Process this file with autoconf to create configure.

echo ""
echo "Configuring triton dynamic library loader interface..."

TRITON_INCS=""
TRITON_LDFLAGS=""
loader=""

AC_PATH_PROG(unamepath, uname)
if test "_$unamepath" = _; then
   system="unknown"
   AC_DEFINE([USE_LTDL], 1, [Whether to use GNUs libltdl])
   loader="ltdl"
else
   AC_MSG_CHECKING(system type)
   system=`$unamepath -s` 
   if test "$system" = "Linux"; then
      AC_MSG_RESULT($system)
      AC_DEFINE([USE_DLFCN], 1, [Whether to use -dl])
      TRITON_LDFLAGS="-ldl "
      loader="dlfcn"
   fi
   if test "$system" = "FreeBSD"; then
      AC_MSG_RESULT($system)
      AC_DEFINE([USE_DLFCN], 1, [Whether to use -dl])
      loader="dlfcn"
   fi
   if test "$system" = "SunOS"; then
      AC_MSG_RESULT($system)
      AC_DEFINE([USE_DLFCN], 1, [Whether to use -dl])
      loader="dlfcn"
   fi
   if test "$system" = "Darwin"; then
		osx_version=`$unamepath -r | cut -c 1-2`
		if test "$osx_version" = "10"; then
	        AC_MSG_RESULT("Max OS X Snow Leopard ($system)")
			AC_DEFINE([USE_DLFCN], 1, [Whether to use snow leopard])
			loader=""
		else
 	        AC_DEFINE([USE_DARWIN], 1, [Whether to use darwin loader])
	        AC_MSG_RESULT($system)
	        loader="darwin"
		fi
   fi
   if test "$system" = "OpenBSD"; then
      AC_MSG_RESULT($system)
      AC_DEFINE([USE_DLFCN], 1, [Whether to use -dl])
      loader="openbsd"
   fi
   if test "$system" = "NetBSD"; then
      AC_MSG_RESULT($system)
      AC_DEFINE([USE_DLFCN], 1, [Whether to use -dl])
      loader="netbsd"
   fi
   mingw=`$unamepath -s | cut -c 1-5`
   if test "$mingw" = "MINGW"; then
      AC_MSG_RESULT($system)
	  AC_DEFINE([USING_WIN32], 1, [We Are Using MinGW])
	  AC_DEFINE([WIN32], 1, [We are on Windows])
	  loader="win32"
   fi
fi

if test "$loader" == ""; then
   AC_CHECK_FUNC(dlopen, FOUND_DLOPEN=1, FOUND_DLOPEN=0,)
   if test "$FOUND_DLOPEN" -eq 1; then
      AC_DEFINE([USE_DLFCN], 1, [Whether to use -dl])
      TRITON_LDFLAGS=""
   else
      AC_CHECK_LIB(dl, dlopen, FOUND_DLOPEN=1, FOUND_DLOPEN=0,)
      AC_DEFINE([USE_DLFCN], 1, [Whether to use -dl])
      TRITON_LDFLAGS="-ldl"
   fi
fi

if test "$loader" = "ltdl"; then
   LTDLDIR="libltdl"
   AC_SUBST(LTDLDIR)
   echo "running libtoolize..."
   `libtoolize --ltdl --force --copy`
   TRITON_INCS="\${top_srcdir}/libltdl"
   TRITON_LDFLAGS="\${top_builddir}/libltdl/libltdlc.la"
   AC_DEFINE([USE_LTDL], 1, [Whether to use GNUs libltdl])
   AC_LIBLTDL_CONVENIENCE
   AC_CONFIG_SUBDIRS(libltdl)
fi

AC_SUBST(TRITON_INCS)
AC_SUBST(TRITON_LDFLAGS)
