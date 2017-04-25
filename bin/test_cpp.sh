#!/bin/bash -l

set -x -e -u -o pipefail

BUILD_DIR="$HOME/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake /rslinac/src/libHellweg2D
make -j
make test
