dnl -*- Autoconf -*-

AC_DEFUN([SST_zesto_CONFIG], [
  AC_ARG_WITH([qsim],
    [AS_HELP_STRING([--with-qsim@<:@=DIR@:>@],
      [Use QSim package installed in optionally specified DIR])])

  happy="yes"
  AS_IF([test "$with_qsim" = "no"], [happy="no"])

  CPPFLAGS_saved="$CPPFLAGS"
  LDFLAGS_saved="$LDFLAGS"
  LIBS_saved="$LIBS"

  AS_IF([test ! -z "$with_qsim" -a "$with_qsim" != "yes"],
    [QSIM_CPPFLAGS="-I$with_qsim -DUSE_QSIM"
     CPPFLAGS="$QSIM_CPPFLAGS $CPPFLAGS"
     QSIM_LDFLAGS="-L$with_qsim"
     LDFLAGS="$QSIM_LDFLAGS $LDFLAGS"],
    [QSIM_CPPFLAGS=
     QSIM_LDFLAGS=])

  AC_LANG_PUSH(C++)
  AC_CHECK_HEADERS([qsim-client.h], [], [happy="no"])
  AC_CHECK_LIB([qsim], [qsim_present], 
    [QSIM_LIB="-lqsim -lqsim-client"], [happy="no"])
  AC_LANG_POP(C++)

  CPPFLAGS="$CPPFLAGS_saved"
  LDFLAGS="$LDFLAGS_saved"
  LIBS="$LIBS_saved"

  AC_SUBST([QSIM_CPPFLAGS])
  AC_SUBST([QSIM_LDFLAGS])
  AC_SUBST([QSIM_LIB])

  AS_IF([test "$happy" = "yes"],
    [QSIM_CPPFLAGS="$QSIM_CPPFLAGS -DUSE_QSIM"]
  )

  AS_IF([test "$happy" = "yes"], [$1], [$2])
])
