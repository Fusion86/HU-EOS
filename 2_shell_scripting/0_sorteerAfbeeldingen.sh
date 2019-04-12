#!/bin/bash

if [ -z "$1" ]; then
    echo "Geef een bron map als argument mee";
    exit 1
fi

mkdir -p Afbeeldingen
find $1 \( -name "*.jpg" -o -name "*.png" \) -exec cp {} Afbeeldingen/ \;
