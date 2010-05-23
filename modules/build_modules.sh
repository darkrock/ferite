#!/bin/sh

moduledir="modules"

case "$1" in

  bootstrap)
    # generate the subdirs stuff for configure.in
    echo "Generating module list...";
    echo "echo \"\"" > ../modules-config.m4
    echo "echo \"Configuring ferite modules...\"" >> ../modules-config.m4
    echo "echo \"\"" >> ../modules-config.m4
    echo "modules=\"\"" >> ../modules-config.m4
    modulemakefiles=""
    for dir in *; do
      if test -d "$dir"; then
        cd $dir;
        if test -f "config.m4"; then
	   echo "echo \"checking module $dir ...\"" >> ../../modules-config.m4
       echo "sinclude($moduledir/$dir/config.m4)" >> ../../modules-config.m4
	   modulemakefiles="$modulemakefiles$moduledir\/$dir\/Makefile \\\\\\
"
        fi
	if test -f "Makefile.am"; then
	   if test ! -f "$dir.fec"; then
	      modulelist="$modulelist $dir";
	      if test -f makefiles; then
	         modulemakefiles="$modulemakefiles$moduledir\/$dir\/Makefile \\\\\\
"
	         modulemakefiles="$modulemakefiles"`cat makefiles`"
"
	      fi
	    fi
	fi
        cd ..
      fi
    done
    echo "AC_SUBST(modules)" >> ../modules-config.m4
    echo "echo \"\"" >> ../modules-config.m4
    sed -e "s/%modules%/$modulelist/" Makefile.am.stub > Makefile.am
    sed -e "s/%modules%/$modulemakefiles/g" ../configure.ac.stub > ../configure.ac
    ;;
  rebuild)
    # go through all the directories, generate the c code for them, and then
    # generate the makefiles, now we have to do this because configure wont...
    echo ""
    echo "------- GENERATING MAKEFILES (this might take some time) -------"
    echo ""
    for dir in *; do
      if test -d "$dir"; then
         cd $dir;
	     make maintainer-clean
         if test -f "$dir.fec"; then
		    ../../builder/builder -k -m $dir `echo \`for file in *.[ch]; do echo "-a $file "; done\` | egrep -v "^-a $dir" | sed -e "s/-a \*\.\[ch\]//"` `echo \`for file in *.fec; do echo "-A $file "; done\` | sed -e "s/-A \*\.fec//"` $dir.fec > /dev/null
		 fi
         cd ..
      fi
    done
    echo ""
  	echo "----------------------------------------------------------------"
    echo ""
    ;;
  rebuilder)
    # go through all the directories, generate the c code for them, and then
    # generate the makefiles, now we have to do this because configure wont...
    echo ""
    echo "------- GENERATING MAKEFILES (this might take some time) -------"
    echo ""
    for dir in *; do
      if test -d "$dir"; then
         cd $dir;
         if test -f "$dir.fec"; then
		    ../../builder/builder -k -m $dir `echo \`for file in *.[ch]; do echo "-a $file "; done\` | egrep -v "^-a $dir" | sed -e "s/-a \*\.\[ch\]//"` `echo \`for file in *.fec; do echo "-A $file "; done\` | sed -e "s/-A \*\.fec//"` $dir.fec > /dev/null
		 fi
         cd ..
      fi
    done
    echo ""
  	echo "----------------------------------------------------------------"
    echo ""
    ;;
  build)
    dir=$2
	if test -d "$dir"; then
	  cd $dir;
#	  make maintainer-clean
	  if test -f "$dir.fec"; then
		    ../../builder/builder -k -m $dir `echo \`for file in *.[ch]; do echo "-a $file "; done\` | egrep -v "^-a $dir" | sed -e "s/-a \*\.\[ch\]//"` `echo \`for file in *.fec; do echo "-A $file "; done\` | sed -e "s/-A \*\.fec//"` $dir.fec > /dev/null
	  fi
      cd ..
    fi
    ;;    
  *)
    echo "Usage: ./build_modules.sh ( bootstrap | rebuild | build name_of_module_dir)";
    exit 1
    ;;
    
esac

exit 0  
