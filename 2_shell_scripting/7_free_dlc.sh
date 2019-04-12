#!/bin/sh

# Dit is het enige nuttige bash script dat ik in mijn leven gemaakt heb
# Dit script kan je aanroepen vanaf je mac, windows, linux of raspberry ding en dan gaat het je code compileren
# Als de code op een niet raspberry device gecompiled word dan geven we de variable FAKE_BRICKPI mee, dit houd in
# dat ipv de echte brickpi library alle functies stubbed worden, zodat je de code ook kan testen op windows, mac enz (excl. de brickpi functies natuurlijk)
#
# Zie https://github.com/Fusion86/V1GP-18-Jhin/blob/v2/CMakeLists.txt
# Stubbed BrickPi3 https://github.com/Fusion86/V1GP-18-Jhin/blob/v2/src/BrickPi3Fake.cpp (gemaakt met mjradwin/stubgen)

mkdir -p build
cd build

if grep -q "Raspberry" /proc/device-tree/model &>/dev/null; then
    # Compile on Raspberry Pi
    cmake -D CMAKE_BUILD_TYPE=Debug ..
    make
else
    # Check if we need to use MSYS or normal Makefiles (MSYS is for Windows)
    if uname -a | grep -q 'MSYS_NT'; then
        GEN="MSYS Makefiles"
    else
        GEN="Unix Makefiles"
    fi

    echo "-- Using generator: $GEN"

    # Fake compile (to check if the code can compile, all BrickPi functions are stubbed)
    cmake -D CMAKE_BUILD_TYPE=Debug -D FAKE_BRICKPI=1 -G "$GEN" ..
    make
fi