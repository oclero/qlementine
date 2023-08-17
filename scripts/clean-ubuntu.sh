#!/bin/bash

rm -f $(dirname "$0")/../CMakeLists.txt.user
source $(dirname "$0")/configure-ubuntu.sh
cmake --build "$build_dir" --target clean
