#!/usr/bin/env bash

set -e

rm -rf _build
mkdir -p _build && cd _build
cmake .. -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=modules/conan_provider.cmake -DCMAKE_BUILD_TYPE=Debug -Dtoolbox_BUILD_TESTS=On
cmake --build . --target toolbox-test
if [ -f "./toolbox-test" ]
then
	./toolbox-test
elif [ -f "./bin/toolbox-test" ]
then
	./bin/toolbox-test
fi
