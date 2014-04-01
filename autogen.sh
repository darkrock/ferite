#!/bin/sh

BLD_SRT=`./shtool echo -n -e %B`
BLD_END=`./shtool echo -n -e %b`

echo "+-----------------------------------------------+"
echo "| "${BLD_SRT}"Ferite Development Build"${BLD_END}"                      |"
echo "| "${BLD_SRT}"This is development software. Be warned."${BLD_END}"      |"
echo "+-----------------------------------------------+"
echo

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

THEDIR="`pwd`"
cd "$srcdir"
DIE=0

cd modules;
./build_modules.sh bootstrap
cd ../

echo "running... [ "${BLD_SRT}"libtoolize"${BLD_END}" ]"
(libtoolize --automake || glibtoolize --automake)
echo "running... [ "${BLD_SRT}"aclocal"${BLD_END}"    ]"
aclocal -I . $ACLOCAL_FLAGS
echo "running... [ "${BLD_SRT}"autoheader"${BLD_END}" ]"
autoheader
echo "running... [ "${BLD_SRT}"automake"${BLD_END}"   ]"
automake -a -c
echo "running... [ "${BLD_SRT}"autoconf"${BLD_END}"   ]"
autoconf

cd "$THEDIR"
