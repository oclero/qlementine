#!/bin/bash

declare vcpkg_dir="C:/Users/ocler/Documents/Development/vcpkg"
declare vcpkg_triplet="x64-windows"
git submodule update --init --recursive
$vcpkg_dir/vcpkg.exe install --triplet $vcpkg_triplet cpp-httplib
