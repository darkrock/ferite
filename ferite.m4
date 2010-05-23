dnl
dnl AM_PATH_FERITE(MIN-REQ-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl

dnl
dnl Made based on SFW.m4, which was in turn based on glib.m4 and gtk.m4
dnl

AC_DEFUN([AM_PATH_FERITE],[

dnl Various autoconf user options

AC_ARG_WITH(ferite-prefix,
[  --with-ferite-prefix=PFX   ]dnl
[Prefix where ferite is installed],
ferite_prefix="$withval", ferite_prefix="")

dnl Left for future reference
dnl    for module in . $4
dnl    do
dnl        case "$module" in
dnl            libjs|js)
dnl                extra_libs="$extra_libs --with-js"
dnl            ;;

dnl            curses)
dnl                extra_libs="$extra_libs --with-curses"
dnl            ;;
dnl        esac
dnl    done
                

    if test x$ferite_prefix != x; then
        FERITE_CONFIG="$ferite_prefix/bin/ferite-config"
    fi

    have_ferite="no";
     
    AC_PATH_PROG(FERITE_CONFIG, ferite-config, no)
	AC_PATH_PROG(BUILDER, builder, no)
	
    min_ferite_version=ifelse([$1],,1.0.0,$1)
    AC_MSG_CHECKING(for ferite - version >= $min_ferite_version)
    if test "x$FERITE_CONFIG" != xno; then
		FERITE_CFLAGS=`$FERITE_CONFIG --cflags`
		FERITE_LIBS=`$FERITE_CONFIG --libs $extra_libs`
		FERITE_PREFIX=`$FERITE_CONFIG --prefix`
		FE_LIBRARY_PATH=`$FERITE_CONFIG --library-path`
		FE_XPLAT_LIBRARY_PATH=`$FERITE_CONFIG --module-library-path`
		FE_NATIVE_LIBRARY_PATH=`$FERITE_CONFIG --native-library-path`

		ferite_config_major_version=`$FERITE_CONFIG --version | \
	            sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
	        ferite_config_minor_version=`$FERITE_CONFIG --version | \
	            sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
	        ferite_config_micro_version=`$FERITE_CONFIG --version | \
	            sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

		# Later we should do actual tests on the library
		ferite_req_major_version=`echo $min_ferite_version | \
	            sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
	        ferite_req_minor_version=`echo $min_ferite_version | \
	            sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
	        ferite_req_micro_version=`echo $min_ferite_version | \
	            sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`

		if test $ferite_req_major_version -lt $ferite_config_major_version; then
	            have_ferite="yes"
		elif test $ferite_req_major_version -eq $ferite_config_major_version && \
	                test $ferite_req_minor_version -lt $ferite_config_minor_version; then
	            have_ferite="yes"
		elif test $ferite_req_major_version -eq $ferite_config_major_version && \
	                test $ferite_req_minor_version -eq $ferite_config_minor_version && \
	                test $ferite_req_micro_version -le $ferite_config_micro_version; then
	            have_ferite="yes"
		fi
    fi

    if test "x$have_ferite" = xyes; then
        AC_MSG_RESULT(yes)
	ifelse([$2], , :, [$2])
    else
        AC_MSG_RESULT(no)
	if test "x$FERITE_CONFIG" = xno; then
	    echo "*** The ferite-config script could not be found. This script"
	    echo "*** needs to be in the PATH, or should be specified using"
	    echo "*** the --with-ferite-prefix option to configure."
	fi

	FERITE_CFLAGS=""
	FERITE_LIBS=""
	ifelse([$3], , :, [$3])
    fi

    AC_SUBST(FERITE_CFLAGS)
    AC_SUBST(FERITE_LIBS)
    AC_SUBST(FERITE_PREFIX)
    AC_SUBST(FE_LIBRARY_PATH)
    AC_SUBST(FE_XPLAT_LIBRARY_PATH)
    AC_SUBST(FE_NATIVE_LIBRARY_PATH)
])    

