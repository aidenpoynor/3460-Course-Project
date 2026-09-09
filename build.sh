#!/bin/bash

set -e

mkdir -p build

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "Building on Linux..."

    cd build
    cmake ..
    make

elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "win32" || "$OSTYPE" == "cygwin" ]]; then
    echo "Building on Windows..."

    cd build
    cmake ..
    cmake --build . --config Release

else
    echo "Unsupported operating system: $OSTYPE"
    exit 1
fi

echo "Build successful!"
