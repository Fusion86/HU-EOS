#!/bin/bash

if [ -z "$1" ]; then
    echo "Geef een bron map als argument mee";
    exit 1
fi

find $1 -printf '%s %p\n'| sort -nr | head -10
