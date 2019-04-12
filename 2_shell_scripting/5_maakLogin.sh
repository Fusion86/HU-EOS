#!/bin/bash

if [ -z "$1" ]; then
    echo "Geef een wachtwoord output bestand mee als argument";
    exit 1
fi

read -p "Enter naam pls: " naam

if [ -z "$naam" ]; then
    naam=$(whoami)
    echo "Oke dan niet"
fi

echo "Gebruikte naam: $naam"  

loop=true
while [ "$loop" = "true" ]; do
    read -sp "Pls twee keer zelfde wachtwoord, dank: " pass
    echo # Newline

    if [[ $pass =~ ^(?=.*[a-z])(?=.*[A-Z])(?=.*\d).{8,}$ ]]; then
        if [ "$prev_pass" == "$pass" ]; then
            echo "Ok we zijn klaar hier"
            md5=`echo $pass | md5sum | awk '{ print $1 }'`
            echo "$naam:$pass" >> $1
            loop=false
        else
            prev_pass = $pass
        fi
    else
        echo "Wachtwoord moet minstens 8 characters, een kleine letter, een hoofdletter en een cijfer bevatten"
    fi
done
