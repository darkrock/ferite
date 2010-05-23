ipc_LIBS=""
ipc_CFLAGS="-I../../"
AC_SUBST(ipc_LIBS)
AC_SUBST(ipc_CFLAGS)

if test "$system" = "MINGW"; then
   echo "Not building ipc module on Windows"
else
   modules="$modules ipc"
fi