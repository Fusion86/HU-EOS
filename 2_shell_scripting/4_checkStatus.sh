#!/bin/bash

if [ -z "$1" ]; then
    echo "Geef een bron map als argument mee";
    exit 1
fi

if [ -z "$2" ]; then
    echo "Geef een commando als argument mee";
    exit 1
fi

for filename in `find $1 -type f`; do
    cmdstr="$2 $filename"
    eval $cmdstr >/dev/null
    printf "cmd: $cmdstr\nstatus:$?\n\n" >> 4_statusLog
done
