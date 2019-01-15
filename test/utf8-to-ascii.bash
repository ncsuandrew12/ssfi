#!/bin/bash

for txt in `find "$1" -type f -name "*.txt"`; do
    sed -i 's/[“”]/"/g' $txt
    sed -i "s/[‘’]/\'/g" $txt
    sed -i 's/[áâà]/a/g' $txt
    sed -i 's/[Ć]/C/g' $txt
    sed -i 's/[éêèëæœ]/e/g' $txt
    sed -i 's/[ï]/i/g' $txt
    sed -i 's/[öô]/o/g' $txt
    sed -i 's/[—]/-/g' $txt
    sed -i 's/[£]//g' $txt
    sed -i 's/[§]/S/g' $txt
    sed -i 's/[ ]/ /g' $txt

    LANG=C sed -i 's/[\d128-\d255]//g' $txt
done

