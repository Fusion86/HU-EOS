#!/bin/bash

if [ -z "$1" ]; then
    echo "Geef een bron map als argument mee";
    exit 1
fi

find $1 -iname "*.jpg" -exec sh -c 'magick convert "{}" $(basename "{}").png' \;
