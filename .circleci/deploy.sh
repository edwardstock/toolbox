#!/usr/bin/env bash
set -e

conan create . scatter/testing
conan export . toolboxpp/2.0.0@scatter/testing
conan test . toolboxpp/2.0.0@scatter/testing
conan create . scatter/latest
conan upload toolboxpp/2.0.0@scatter/latest --all -r=scatter