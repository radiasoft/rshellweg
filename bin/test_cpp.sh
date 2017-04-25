#!/bin/bash -l

set -x -e -u -o pipefail

REPO_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." && pwd )"
BUILD_DIR="$HOME/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake "$REPO_DIR/src/libHellweg2D"
make -j
make test
