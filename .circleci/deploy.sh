#!/usr/bin/env bash
set -e

conan create . scatter/latest
conan export-pkg . toolboxpp/2.1.0@scatter/latest
conan upload toolboxpp/2.1.0@scatter/latest --all -r=scatter