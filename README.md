# ToolBox++

![build status](https://github.com/edwardstock/toolbox/actions/workflows/build.yml/badge.svg)

## Features

* **toolbox::strings** — string helpers: `matches_pattern("[0-9]", "my194string")`
* **toolbox::io** — filesystem helpers
* **toolbox::collections** — collection helpers
* **toolbox::data** — bytes buffer helpers
* **toolbox::term** — confirm / prompt for CLI apps
* **toolbox::tlog** — simple thread-safe logger
* **toolbox::time_profiler** — simple time-based profiler

## Adding to your project

### FetchContent (recommended)

```cmake
include(FetchContent)
FetchContent_Declare(
    toolbox
    GIT_REPOSITORY https://github.com/edwardstock/toolbox.git
    GIT_TAG        3.5.1
)
# Optional overrides (both default to OFF / static):
# set(toolbox_BUILD_SHARED_LIBS ON  CACHE BOOL "" FORCE)
# set(toolbox_BUILD_TESTS       OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(toolbox)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE toolbox::toolbox)
```

### add_subdirectory / git submodule

```bash
git submodule add https://github.com/edwardstock/toolbox.git libs/toolbox
```

```cmake
add_subdirectory(libs/toolbox)
target_link_libraries(my_app PRIVATE toolbox::toolbox)
```

### find_package (after global install)

```cmake
find_package(toolbox REQUIRED)
target_link_libraries(my_app PRIVATE toolbox::toolbox)
```

## Global install

```bash
git clone https://github.com/edwardstock/toolbox.git && cd toolbox
cmake --preset release
cmake --build --preset release
cmake --install build/release
```

Or to a custom prefix:

```bash
cmake --preset release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build --preset release
cmake --install build/release
```

## Build & test

```bash
cmake --preset debug          # configures into build/debug, enables tests
cmake --build --preset debug
ctest --preset debug
```

Available presets: `debug`, `release`, `debug-shared`, `release-shared`.

## Build with Clang on Linux

If you compiled Clang from source, the linker may not find `libc++.so` in the
default library path. You can add it manually to `LD_LIBRARY_PATH` or run:

```bash
sudo -s source llvm_config.sh
```
