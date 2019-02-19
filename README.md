# ToolBox++ [![CircleCI](https://circleci.com/gh/edwardstock/toolboxpp/tree/master.svg?style=svg)](https://circleci.com/gh/edwardstock/toolboxpp/tree/master)


## Reqs
* cmake >= 3.0
* gnu gcc 4.9+/clang 5+/msvc (not tested)
* make

## Usage and features

Namespace for all things is toolboxpp.

What helpers contained:
* **toolboxpp::strings** - strings helpers like bool hasNum = hasRegex("[0-9]", "my194string"")
* **toolboxpp::fs** - filesystem helpers
* **toolboxpp::collection** - collections helpers
* **toolboxpp::numbers** - string numbers helpers
* **toolboxpp::console** - confirm or prompt for your CLI app
* **toolboxpp::Logger** - simple thread-safe logger
* **toolboxpp::Profiler** - simple time-based profiler

See [include/toolboxpp.hpp](include/toolboxpp.h) - almost all code are commented


## Include to your CMake project
* Add submodule or just clone
  * `git submodule add https://github.com/edwardstock/toolboxpp.git /path/to/libs/toolboxpp` 
  * `git clone https://github.com/edwardstock/toolboxpp.git /path/to/libs/toolboxpp`
  
* Edit CMakelists.txt:
```cmake
# your executable or library
add_executable(my_executable) 
#or if library
add_library(my_library)

# include module
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/path/to/libs/toolboxpp)

# link with your project
target_link_libraries(my_[executable|library] toolboxpp)
```
* That's all!

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
* header to: /usr/local/include/toolboxpp.hpp