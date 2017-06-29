# libcpphelpers
Private cpp helpers lib


## Reqs
* cmake >= 3.0
* gcc/clang/msvc (not tested)
* make

## Building
```bash
git clone https://github.com/edwardstock/libcpphelpers.git cpphelpers && cd cpphelpers/build
cmake .. --target cpphelpers -DCMAKE_BUILD_TYPE=Release
make -j4
make install
```