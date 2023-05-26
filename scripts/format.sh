#!/bin/bash

find $(dirname "$0")/../lib/src $(dirname "$0")/../lib/include -iname *.hpp -o -iname *.cpp | xargs clang-format -i
