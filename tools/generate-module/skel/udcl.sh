#!/bin/sh

# define $EDITOR and EDITOR_FLAGS in your environment to customize this
CL="ChangeLog"
DSTR=`date +%d.%m.%Y`
DSTR2=`date +%H:%M\ \(%Z\)`
TF="ferite-changelog-entry-"$USER

if test -z $EDITOR; then
        EDITOR="vim"
        EDITOR_FLAGS="+"
fi

export UNAME=$USER;

if test -f $TF; then
  :
else
  echo -e "--------------------------------------------------------------------------------\n$DSTR $DSTR2, $UNAME\n\n" > $TF
  $EDITOR $EDITOR_FLAGS $TF
fi

echo "" >> $TF
cat $CL >> $TF
rm $CL
if test -f $TF"~"; then
  rm $TF"~"
fi
mv $TF $CL

