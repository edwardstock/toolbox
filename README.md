# ToolBox++ [![Build Status](https://travis-ci.org/edwardstock/toolboxpp.svg?branch=master)](https://travis-ci.org/edwardstock/toolboxpp)


## Reqs
* cmake >= 3.0
* gnu gcc 4.7+/clang 5+/msvc (not tested)
* make

## Build
```bash
git clone https://github.com/edwardstock/toolboxpp.git toolboxpp && cd toolboxpp/build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target toolboxpp
cmake --build . --target install 
```

## Build with CLang on linux
Probably, if you are compiled clang from the sources, linker may not found `libc++.so` in default lib path. You could manually add it to LD_LIBRARY_PATH or using `sudo -s source llvm_config.sh`.

## Build, Run tests, and Install if ok
```bash
git clone https://github.com/edwardstock/toolboxpp.git toolboxpp && cd toolboxpp/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_TEST=ON
cmake --build .
./toolboxtest && cmake --build . --target install
```

By default, script installs:
* header to: /usr/local/include/toolboxpp.h (also toolboxconfig.h)
* library to: /usr/local/lib{suffix}/libtoolboxppd.a