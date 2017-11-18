# ToolBox++ [![Build Status](https://travis-ci.org/edwardstock/libcpphelpers.svg?branch=master)](https://travis-ci.org/edwardstock/libcpphelpers)


## Reqs
* cmake >= 3.0
* gcc/clang/msvc (not tested)
* make

## Build
```bash
git clone https://github.com/edwardstock/toolboxpp.git toolboxpp && cd toolboxpp/build
cmake --build . --target toolboxpp
cmake --build . --target install 
```

By default, script installs:
* header to: /usr/local/include/toolboxpp.h 
* library to: /usr/local/lib{suffix}/libtoolboxpp.a