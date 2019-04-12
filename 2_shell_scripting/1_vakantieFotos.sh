#!/bin/bash

if [ -z "$1" ]; then
    echo "Geef een bron map als argument mee";
    exit 1
fi

mkdir -p Afbeeldingen/Vakantie
find $1 -name "vakantie-*" -exec cp {} Afbeeldingen/Vakantie \;
