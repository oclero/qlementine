#!/bin/bash

source $(dirname "$0")/configure-ubuntu.sh
cmake --build "$build_dir" --config Release --target "QtUpdaterTests"
cd "$build_dir"
ctest -C Release
