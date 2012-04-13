#!/bin/bash

for sourcefile in `ls *.md`; do
    targetfile=`echo $sourcefile | sed s/\.md/\.html/`
    echo "$sourcefile -> $targetfile"
    kramdown --toc-levels 2..3 --template template.html.erb $sourcefile > $targetfile
done