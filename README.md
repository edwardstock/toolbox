# ToolBox++

![build status](https://github.com/edwardstock/toolbox/actions/workflows/build_conan.yml/badge.svg)

## Usage and features

* **toolbox::strings** - strings helpers like bool hasNum = matches_pattern("\[0-9\]", "my194string"")
* **toolbox::io** - filesystem helpers
* **toolbox::collections** - collections helpers
* **toolbox::data** - bytes buffer helpers
* **toolbox::term** - confirm or prompt for your CLI app
* **toolbox::tlog** - simple thread-safe logger
* **toolbox::time_profiler** - simple time-based profiler

# Add to project
## Using Conan

Add remote:

```bash
conan remote add edwardstock https://conan.edwardstock.com/artifactory/api/conan/conan-local
```


Add to `conanfile.txt`:

```ini
[requires]
toolbox/3.4.0@edwardstock/latest

[options]
toolbox/*:shared=False # default is False

[generators]
CMakeDeps
CMakeToolchaincon
```

## Include to your CMake project

* Add submodule or just clone
    * `git submodule add https://github.com/edwardstock/toolbox.git /path/to/libs/toolbox`
    * `git clone https://github.com/edwardstock/toolbox.git /path/to/libs/toolbox`

* Edit `CMakeLists.txt`:

```cmake
# your executable or library
add_executable(my_executable)
#or if library
add_library(my_library)

# include module
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/path/to/libs/toolbox)

# link with your project
target_link_libraries(my_[executable|library] toolbox)
```

or using [CMake FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html)

```cmake
include(FetchContent)
fetchcontent_declare(
	toolbox
	GIT_REPOSITORY https://github.com/edwardstock/toolbox.git
	GIT_TAG 3.4.0
)
set(toolbox_BUILD_TESTS OFF)
set(toolbox_BUILD_SHARED_LIBS OFF)
fetchcontent_makeavailable(toolbox)

add_executable(my_app main.cpp)
target_link_libraries(my_app toolbox)
```

## Global Install

This library is Header-Only, so build isn't required. But you can install globally:

```bash
git clone https://github.com/edwardstock/toolbox.git toolbox && cd toolbox/build
cmake --build . --target install
```

Then include into a project:

```cmake
find_package(toolbox REQUIRED)
target_link_libraries(my_[executable|library] toolbox::toolbox)
```

## Testing

```bash
git clone https://github.com/edwardstock/toolbox.git toolbox && cd toolbox/build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DENABLE_TEST=On
cmake --build . --target toolbox-test
./toolbox-test
```

## Build with CLang on linux

Probably, if you are compiled clang from the sources, linker may not found `libc++.so` in default lib path. You could
manually add it to LD_LIBRARY_PATH or using `sudo -s source llvm_config.sh`.

By default, script installs:

* header to: /usr/local/include/toolbox.hpp
