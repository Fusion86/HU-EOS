#!/bin/bash

# Ik snap niet wat `case` met deze opdracht te maken heeft

if [ -z "$1" ]; then
    echo "Geef een bron map als argument mee";
    exit 1
fi

echo "Bron: $1"

for ext in "${@:2}"
do
    file_count=$(find $1 -iname "*.$ext" | wc -l)
    echo "Er zijn $file_count bestanden met de extensie $ext"
done