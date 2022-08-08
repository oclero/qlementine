#!/bin/bash

declare build_dir="./build"
declare vcpkg_toolchain_file="../vcpkg/scripts/buildsystems/vcpkg.cmake"
declare vcpkg_triplet="x64-linux"
cmake -B $build_dir -DCMAKE_TOOLCHAIN_FILE="$vcpkg_toolchain_file" -DVCPKG_TARGET_TRIPLET=$vcpkg_triplet -DVCPKG_APPLOCAL_DEPS=OFF
