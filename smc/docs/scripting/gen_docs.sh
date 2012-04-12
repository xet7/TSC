#!/bin/bash

for sourcefile in `ls *.md`; do
    targetfile=`echo $sourcefile | sed s/\.md/\.html/`
    echo "$sourcefile -> $targetfile"
    kramdown --template template.html.erb $sourcefile > $targetfile
done