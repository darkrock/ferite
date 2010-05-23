unix_LIBS=""
unix_CFLAGS=""
AC_SUBST(unix_LIBS)
AC_SUBST(unix_CFLAGS)

if test "$system" = "MINGW"; then
  echo "Not building Unix module on Windows"
else
  modules="$modules unix"
fi