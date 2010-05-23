#!/bin/sh

F=`which feritedoc`
./build_manuals.sh
./build_api_docs.sh
./build_c_api_docs.sh `dirname $F`"/"
