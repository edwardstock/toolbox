# libcpphelpers [![Build Status](https://travis-ci.org/edwardstock/libcpphelpers.svg?branch=master)](https://travis-ci.org/edwardstock/libcpphelpers)
Private cpp helpers lib


## Reqs
* cmake >= 3.0
* gcc/clang/msvc (not tested)
* make

## Build
```bash
git clone https://github.com/edwardstock/libcpphelpers.git cpphelpers && cd cpphelpers/build
cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build . --target cpphelpers
cmake install .
```

By default, script installs:
* header to: /usr/local/include/cpphelpers.h 
* library to: /usr/local/lib{suffix}/libcpphelpers.a


~~~{.cpp}
void main(int argc, char** argv);
~~~