#!/usr/bin/env bash

BUILD_SCRIPT=""

if [ "$1" == "archive" ]; then
  BUILD_SCRIPT="-DPACKAGE_ARCHIVE=On"
fi

mkdir -p _build && cd _build
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_CONAN=Off -DENABLE_TEST=Off ${BUILD_SCRIPT}
make -j2
make package

if [ "$1" == "archive" ] && [ "$2" == "upload" ]; then
  chmod +x package_upload.sh
  if [ "$3" == "github" ]; then
    ./package_upload.sh github
  else
    ./package_upload.sh bintray
  fi

fi
