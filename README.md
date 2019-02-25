# ToolBox++

| Bintray | Windows | Linux & macOS |
|:--------:|:---------:|:-----------------:|
|[ ![Download](https://api.bintray.com/packages/edwardstock/scatter/toolboxpp%3Ascatter/images/download.svg) ](https://bintray.com/edwardstock/scatter/toolboxpp%3Ascatter/_latestVersion)|unavailable|[![CircleCI](https://circleci.com/gh/edwardstock/toolboxpp/tree/master.svg?style=svg)](https://circleci.com/gh/edwardstock/toolboxpp/tree/master)|



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

See [include/toolboxpp.hpp](include/toolboxpp.hpp) - almost all code are commented

# Add to project

## Using Conan

Add remote:
```bash
conan remote add scatter https://api.bintray.com/conan/edwardstock/scatter
```

```bash
conan install <path/to/your/project> toolboxpp/2.0.0@scatter/stable
```

or using `conanfile.txt`
```ini
[requires]
toolboxpp/2.0.0@scatter/stable

[generators]
cmake
```

## Include to your CMake project
* Add submodule or just clone
  * `git submodule add https://github.com/edwardstock/toolboxpp.git /path/to/libs/toolboxpp` 
  * `git clone https://github.com/edwardstock/toolboxpp.git /path/to/libs/toolboxpp`
  
* Edit `CMakeLists.txt`:
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

## Global Install
This library is Header-Only, so build isn't required. But you can install globally:
```bash
git clone https://github.com/edwardstock/toolboxpp.git toolboxpp && cd toolboxpp/build
cmake --build . --target install
```

## Testing
```bash
git clone https://github.com/edwardstock/toolboxpp.git toolboxpp && cd toolboxpp/build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_TEST=On
cmake --build . --target toolboxtest
./bin/toolboxtest
```

## Build with CLang on linux
Probably, if you are compiled clang from the sources, linker may not found `libc++.so` in default lib path. You could manually add it to LD_LIBRARY_PATH or using `sudo -s source llvm_config.sh`.

By default, script installs:
* header to: /usr/local/include/toolboxpp.hpp