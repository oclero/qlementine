#!/bin/bash

source $(dirname "$0")/configure-msvc.sh
cmake --build "$build_dir" --config Release
