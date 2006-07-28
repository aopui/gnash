dnl  
dnl    Copyright (C) 2005, 2006 Free Software Foundation, Inc.
dnl  
dnl  This program is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU General Public License as published by
dnl  the Free Software Foundation; either version 2 of the License, or
dnl  (at your option) any later version.
dnl  
dnl  This program is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl  GNU General Public License for more details.
dnl  You should have received a copy of the GNU General Public License
dnl  along with this program; if not, write to the Free Software
dnl  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

dnl Linking Gnash statically or dynamically with other modules is making a
dnl combined work based on Gnash. Thus, the terms and conditions of the GNU
dnl General Public License cover the whole combination.
dnl
dnl As a special exception, the copyright holders of Gnash give you
dnl permission to combine Gnash with free software programs or libraries
dnl that are released under the GNU LGPL and with code included in any
dnl release of Talkback distributed by the Mozilla Foundation. You may
dnl copy and distribute such a system following the terms of the GNU GPL
dnl for all but the LGPL-covered parts and Talkback, and following the
dnl LGPL for the LGPL-covered parts.
dnl
dnl Note that people who make modified versions of Gnash are not obligated
dnl to grant this special exception for their modified versions; it is their
dnl choice whether to do so. The GNU General Public License gives permission
dnl to release a modified version without this exception; this exception
dnl also makes it possible to release a modified version which carries
dnl forward this exception.
dnl  
dnl 

AC_DEFUN([AM_PATH_LIBXML2],
[dnl 
dnl Get the cflags and libraries
dnl
AC_ARG_WITH(libxml,[  --with-libxml=PFX       Prefix where libxml is installed (optional)], libxml_prefix="$withval", libxml_prefix="")
AC_ARG_WITH(libxml-libraries,[  --with-libxml-libraries=DIR   Directory where libxml library is installed (optional)], libxml_libraries="$withval", libxml_libraries="")
AC_ARG_WITH(libxml-includes,[  --with-libxml-includes=DIR   Directory where libxml header files are installed (optional)], libxml_includes="$withval", libxml_includes="")
dnl AC_ARG_ENABLE(libxmltest, [  --disable-libxmltest       Do not try to compile and run a test libxml program],, enable_libxmltest=yes)

  if test "x$libxml_libraries" != "x" ; then
    if test x"$libxml_libraries" != x"/usr/lib"; then
      LIBXML_LIBS="-L$libxml_libraries -lxml2"
    else
      LIBXML_LIBS="-lxml2"
    fi
  elif test "x$libxml_prefix" != "x" ; then
    if test x"$libxml_prefix" != x"/usr"; then
      LIBXML_LIBS="-L$libxml_prefix/lib -lxml2"
    else
      LIBXML_LIBS="-lxml2"
    fi
  fi

  if test "x$libxml_includes" != "x" ; then
    LIBXML_CFLAGS="-I$libxml_includes"
  elif test "x$libxml_prefix" != "x" ; then
    LIBXML_CFLAGS="-I$libxml_prefix/include"
  fi

  if test x"${xml}" = x"yes"; then
  dnl
  dnl Give xml2-config a chance
  dnl
  dnl NOTE: LIBXML_LIBS might end up containing
  dnl       -L/usr/lib, which I really want to avoid!
  dnl
  #no_libxml=""
  AC_PATH_PROG(XML2_CONFIG, xml2-config, , ,[$PATH])
  if test "x$XML2_CONFIG" != "x" ; then
    if test "x$LIBXML_CFLAGS" = "x" ; then
      LIBXML_CFLAGS=`$XML2_CONFIG --cflags`
    fi

    if test "x$LIBXML_LIBS" = "x" ; then
      LIBXML_LIBS=`$XML2_CONFIG --libs`
    fi
  else
    AC_MSG_RESULT(no)
  fi

  AC_MSG_CHECKING(for libxml2)

  dnl
  dnl Try finding out yourself
  dnl
  if test "x$LIBXML_CFLAGS" = "x" -o "x$LIBXML_LIBS" = "x"; then
    dirlist="${prefix} /lib64 /usr/lib64 /lib /usr/lib /usr/pkg /usr /usr/local /opt /home/latest"
    for i in $dirlist; do
      if test "x$LIBXML_CFLAGS" = "x"; then
        for j in `ls -dr $i/include/libxml2* 2>/dev/null ` ; do
  	 if test -f $j/libxml/parser.h; then
  	   LIBXML_CFLAGS=-I`(cd $j; pwd)`
  	   break
  	 fi
        done
      fi
      if test "x$LIBXML_LIBS" = "x"; then
        for j in `ls -dr $i/lib 2>/dev/null ` ; do
         if test -f $j/libxml2.so; then
           if test x"$j" != x"/usr/lib"; then
             LIBXML_LIBS="-L`(cd $j; pwd)` -lxml2"
             break
           else
             LIBXML_LIBS="-lxml2"
             break
           fi
         fi
        done
      fi
    done
  fi
  else
    LIBXML_CFLAGS=
    LIBXML_LIBS=
  fi

  if test "x$LIBXML_CFLAGS" != "x" -a  "x$LIBXML_LIBS" != "x"; then
    AC_MSG_RESULT(yes)
    AC_DEFINE(HAVE_LIBXML,1,[Define this if you have libxml2 support available])
  else
    AC_MSG_RESULT(no)
  fi

  AC_SUBST(LIBXML_CFLAGS)
  AC_SUBST(LIBXML_LIBS)
])
