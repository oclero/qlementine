#!/bin/bash

declare build_dir="./build"
declare qt5_dir="C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5"
declare generator="Visual Studio 17 2022"
cmake -B $build_dir -G "$generator" -DQt5_DIR=$qt5_dir
