#!/bin/bash

declare build_dir="./build"
declare generator="Xcode"
declare qt_version="5"
cmake -B $build_dir -G "$generator" -DQT_DEFAULT_MAJOR_VERSION="$qt_version"
