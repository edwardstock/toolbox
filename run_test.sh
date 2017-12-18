#!/usr/bin/env bash
chmod +x ./toolboxtest

if [ hash `llvm-config 2>/dev/null` ]
then
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`llvm-config --libdir` ./build/toolboxtest
else
	./build/toolboxtest
fi