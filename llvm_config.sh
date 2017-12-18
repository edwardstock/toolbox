#!/usr/bin/env bash

if [ hash `llvm-config 2>/dev/null` ] && [ ! -f /etc/ld.so.conf.d/llvm.conf ];
then
	sudo echo `llvm-config --libdir` >> /etc/ld.so.conf.d/llvm.conf
	sudo ldconfig
	echo "Added llvm lib dir to ldconfig"
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:`llvm-config --libdir`
fi