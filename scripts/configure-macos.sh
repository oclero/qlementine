#!/bin/bash

declare build_dir="./build"
declare generator="Xcode"
cmake -B $build_dir -G "$generator"
