filesystem_LIBS=""
filesystem_CFLAGS=""
AC_SUBST(filesystem_LIBS)
AC_SUBST(filesystem_CFLAGS)
AC_CHECK_FUNCS(flock)

modules="$modules filesystem"
