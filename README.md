# libcpphelpers
Private cpp helpers lib


## Reqs
* cmake >= 3.0
* gcc/clang/msvc (not tested)
* make

## Building
```bash
git clone https://github.com/edwardstock/libcpphelpers.git cpphelpers && cd cpphelpers/build
cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build . --target cpphelpers
cmake install
```

By default, script installs:
* header to: /usr/local/include/cpphelpers.h 
* library to: /usr/local/lib{suffix}/libcpphelpers.a