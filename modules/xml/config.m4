dnl xml module stuff

xml_LIBS=""
xml_CFLAGS=""

AC_CHECK_LIB(xml2, xmlParseDocument, xml=yes,
  echo "Cannot find libXML2: xml support require libXML2 from http://xmlsoft.org",)

if test "$xml" = yes; then
  xml_LIBS="$xml_LIBS "`xml2-config --libs`
  xml_CFLAGS="$xml_CFLAGS "`xml2-config --cflags`
  modules="$modules xml"
  echo "Building xml support..."
fi

AC_SUBST(xml_LIBS)
AC_SUBST(xml_CFLAGS)
