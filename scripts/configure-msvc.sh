#!/bin/bash

declare build_dir="./build"
declare qt_dir="C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5"
declare generator="Visual Studio 17 2022"
declare qt_version="5"
cmake -B $build_dir -G "$generator" -DQt5_DIR=$qt_dir -DQt6_DIR=$qt_dir -DQT_DEFAULT_MAJOR_VERSION="$qt_version"
