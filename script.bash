#!/bin/bash

rm -f script-out.txt
touch script-out.txt
for txt in `find "$1" -type f -name "*.txt"`; do
    cat $txt | sed -e "s/[^A-Za-z0-9]\+/\n/g" | sed -e "/^\s*$/d" >> script-out.txt
done

cat script-out.txt | sort | uniq -c | sed -e "s/^\\s\+//g" | sort -rn | head -10 | sed -e "s/^\([0-9]\+\) \(.*\)$/\2 \1/g"
