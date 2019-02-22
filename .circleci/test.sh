#!/usr/bin/env bash

cd toolboxpp && mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DWITH_TEST=On
cmake --build . --target toolboxtest
./bin/toolboxtest