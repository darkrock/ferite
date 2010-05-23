network_LIBS=""
if test "$system" = "SunOS"; then
   network_LIBS="-lnsl -lsocket -lresolv"
fi
if test "$system" = "MINGW"; then
   network_LIBS="-lws2_32 "
fi

network_CFLAGS=""
AC_SUBST(network_LIBS)
AC_SUBST(network_CFLAGS)
AC_CHECK_FUNCS(getaddrinfo inet_ntop inet_pton fcntl)

modules="$modules network"
