#!/usr/bin/env bash
chmod +x ./build/toolboxtest

if [ hash `llvm-config 2>/dev/null` ]
then
	echo "Running tests and set LD_LIBRARY_PATH including llvm-config --libdir"
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`llvm-config --libdir` ./build/toolboxtest
else
	echo "Running tests"
	./build/toolboxtest
fi