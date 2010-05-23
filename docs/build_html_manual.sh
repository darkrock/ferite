#/bin/sh

if ! test -d html-manuals; then
	mkdir html-manuals
fi

name=$1
    
    echo "================ GENERATING HTML '$name' MANUAL ================" ;
	manual-builder-html $name.sgml
	if ! test -d html-manuals/$name/; then
		mkdir html-manuals/$name/
	fi
	cp $name/* html-manuals/$name/
	rm -rf $name
