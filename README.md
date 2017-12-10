# ToolBox++ [![Build Status](https://travis-ci.org/edwardstock/toolboxpp.svg?branch=master)](https://travis-ci.org/edwardstock/toolboxpp)


## Reqs
* cmake >= 3.0
* gcc/clang/msvc (not tested)
* make

## Build
```bash
git clone https://github.com/edwardstock/toolboxpp.git toolboxpp && cd toolboxpp/build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target toolboxpp
cmake --build . --target install 
```

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