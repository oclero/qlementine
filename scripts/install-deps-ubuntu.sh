#!/bin/bash

declare vcpkg_dir="../vcpkg"
declare vcpkg_triplet="x64-linux"
git submodule update --init --recursive
$vcpkg_dir/vcpkg install --triplet $vcpkg_triplet cpp-httplib
