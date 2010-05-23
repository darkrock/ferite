posix_LIBS=""
posix_CFLAGS=""
AC_SUBST(posix_LIBS)
AC_SUBST(posix_CFLAGS)
AC_CHECK_FUNCS(getpriority setpriority)

if test "$system" = "MINGW"; then
  echo "Not building posix module under windows"
else
  modules="$modules posix"
fi