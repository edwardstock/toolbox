#!/usr/bin/env bash

set -e

mkdir _build && cd _build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_TOOLBOX_TEST=On -DWITH_COVERAGE=Off
cmake --build . --target toolbox-test
./toolbox-test