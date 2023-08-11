#!/bin/bash

declare build_dir="./build"
cmake -B $build_dir -DQT_DEFAULT_MAJOR_VERSION=5
