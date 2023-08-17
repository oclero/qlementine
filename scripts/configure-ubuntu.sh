#!/bin/bash

declare build_dir="./build"
declare qt_version="5"
cmake -B $build_dir -DQT_DEFAULT_MAJOR_VERSION="$qt_version"
