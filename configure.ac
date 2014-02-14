AC_INIT([ometis],[0.1])

AC_CONFIG_FILES([lib/Makefile])

# check that we have the GCC compiler
AC_PROG_CC

#check that we have the metis.h file
AC_CHECK_HEADERS([metis.h],[],[AC_MSG_ERROR([libmetis-dev not installed])])

#check the library 
AC_CHECK_LIB([metis],[METIS_SetDefaultOptions],[],[AC_MSG_ERROR([libmetis-dev not installed])]) 

# substitute
AC_SUBST(CC)

# right at the end
AC_OUTPUT
