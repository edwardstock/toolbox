#!/usr/bin/env bash

mkdir _build && cd _build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DWITH_TEST=On
cmake --build . --target toolboxtest
./bin/toolboxtest