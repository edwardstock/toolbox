#!/usr/bin/env bash
set -e
set -x

VERS=`git rev-parse --short HEAD`
if [ -f "version" ]
then
    VERS=`cat version | tr -d "\n"`
fi

if [ -f "../version" ]
then
    VERS=`cat ../version | tr -d "\n"`
fi

if [ "${1}" != "" ]
then
    VERS=${1}
fi

conan create . scatter/latest
conan export-pkg . toolboxpp/${VERS}@edwardstock/latest -f
conan upload toolboxpp/${VERS}@edwardstock/latest --all -r=edwardstock