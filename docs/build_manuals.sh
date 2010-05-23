#/bin/sh

for name in manual embed feritedoc; do
    
    echo "================ GENERATING HTML '$name' MANUAL ================" ;
	manual-builder-html $name.sgml
	cp $name/* html-manuals/$name/
	rm -rf $name
	
    echo "================ GENERATING PDF '$name' MANUAL =================" ;
	manual-builder-pdf $name.sgml

done
