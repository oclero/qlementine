#!/bin/bash

source $(dirname "$0")/configure-macos.sh
cmake --build "$build_dir" --config Release --target sandbox
