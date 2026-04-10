cmake_minimum_required(VERSION 3.21)
project(toolbox
        VERSION 3.5.1
        LANGUAGES CXX
        DESCRIPTION "Toolbox - C++ helpers, especially for string and bytes handling"
        HOMEPAGE_URL "https://github.com/edwardstock/toolbox"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0074 NEW)  # find_package() uses <PackageName>_ROOT variables
cmake_policy(SET CMP0091 NEW)  # MSVC runtime library flags via CMAKE_MSVC_RUNTIME_LIBRARY
cmake_policy(SET CMP0092 NEW)  # No /W3 added to CMAKE_CXX_FLAGS on MSVC by default

# ── Options ──────────────────────────────────────────────────────────────────
# Prefixed to avoid collision when consumed via FetchContent / add_subdirectory.
# Consumers can override either option explicitly:
#   set(toolbox_BUILD_SHARED_LIBS ON  CACHE BOOL "" FORCE)
#   set(toolbox_BUILD_TESTS       OFF CACHE BOOL "" FORCE)
option(toolbox_BUILD_SHARED_LIBS
       "Build toolbox as a shared library (default: follows BUILD_SHARED_LIBS)"
       ${BUILD_SHARED_LIBS})
option(toolbox_BUILD_TESTS "Build toolbox test suite" OFF)

# ── Module path ───────────────────────────────────────────────────────────────
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")
include(compilers_flags)
include(SystemDetector)

# ── Generated config header ───────────────────────────────────────────────────
if(NOT toolbox_BUILD_SHARED_LIBS)
    set(TOOLBOX_BUILT_AS_STATIC 1)
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cfg/toolbox_config.in.h"
    "${CMAKE_CURRENT_BINARY_DIR}/include/toolbox/toolbox_config.h"
)

# ── Sources ───────────────────────────────────────────────────────────────────
set(TOOLBOX_HEADERS
    include/toolbox/data.hpp
    include/toolbox/data/basic_data.h
    include/toolbox/data/bytes_data.h
    include/toolbox/data/bytes_buffer.h
    include/toolbox/data/bytes_array.h
    include/toolbox/data/utils.h
    include/toolbox/data/literals.h
    include/toolbox/data/transformers.h
    include/toolbox/data/slice.h
    include/toolbox/data/base64.h
    include/toolbox/strings.hpp
    include/toolbox/strings/regex.h
    include/toolbox/strings/decimal_formatter.h
    include/toolbox/collections.h
    include/toolbox/term.h
    include/toolbox/log.h
    include/toolbox/io.h
    include/toolbox/time_profiler.h
)

set(TOOLBOX_SOURCES
    ${TOOLBOX_HEADERS}
    src/data/bytes_data.cpp
    src/data/bytes_buffer.cpp
    src/data/utils.cpp
    src/data/base64.cpp
    src/strings.cpp
    src/strings/regex.cpp
    src/strings/decimal_formatter.cpp
    src/io.cpp
    src/term.cpp
    src/log.cpp
    src/time_profiler.cpp
)

# ── Library target ────────────────────────────────────────────────────────────
if(toolbox_BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${TOOLBOX_SOURCES})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION   ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
    )
    # On Linux/macOS CMake handles .so symlinks automatically via VERSION/SOVERSION.
    # On Windows there is no soname concept; we produce versioned copies manually.
    if(WIN32)
        # Primary output: toolbox.3.dll  (major-versioned, what import-libs reference)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            OUTPUT_NAME "${PROJECT_NAME}.${PROJECT_VERSION_MAJOR}"
        )
        # Post-build: also write toolbox.3.5.0.dll and the plain toolbox.dll alias
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:${PROJECT_NAME}>"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.${PROJECT_VERSION}.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:${PROJECT_NAME}>"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.dll"
            COMMENT "Creating versioned DLL aliases for ${PROJECT_NAME}"
        )
        # Install all three names so non-CMake consumers can find toolbox.dll
        install(FILES
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PROJECT_NAME}.${PROJECT_VERSION}.dll"
                DESTINATION "${CMAKE_INSTALL_BINDIR}"
                COMPONENT Runtime
        )
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
    endif()
else()
    add_library(${PROJECT_NAME} STATIC ${TOOLBOX_SOURCES})
endif()

# Namespaced alias — works the same whether the project is embedded or installed
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Propagate C++17 requirement to consumers
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON   # safe to link into a shared lib even when static
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TOOLBOX_EXPORTS
    $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

toolbox_set_compiler_flags(${PROJECT_NAME})

if(NOT HAVE_REGEX_H)
    message(WARNING "Disabled \"regex\"-based functionality (no <regex> header found)")
endif()

# ── Tests ─────────────────────────────────────────────────────────────────────
if(toolbox_BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        GTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.13.0
    )
    set(BUILD_GMOCK            OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST          OFF CACHE BOOL "" FORCE)
    set(gtest_force_shared_crt ${toolbox_BUILD_SHARED_LIBS} CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(GTest)

    enable_testing()

    add_executable(${PROJECT_NAME}-test
        tests/main.cpp
        tests/term_test.cpp
        tests/filesystem_test.cpp
        tests/numbers_test.cpp
        tests/collection_test.cpp
        tests/strings_test.cpp
        tests/data_test.cpp
    )

    target_compile_definitions(${PROJECT_NAME}-test PRIVATE
        TOOLBOX_TESTING=1
        $<$<BOOL:${APPLE}>:GTEST_USE_OWN_TR1_TUPLE>
        $<$<BOOL:${APPLE}>:__GLIBCXX__>
    )

    if(NOT MSVC)
        target_compile_options(${PROJECT_NAME}-test PRIVATE -Wno-missing-field-initializers)
    endif()

    target_link_libraries(${PROJECT_NAME}-test PRIVATE GTest::gtest ${PROJECT_NAME})

    # Use the target directly — no hardcoded binary path
    add_test(NAME test-all COMMAND ${PROJECT_NAME}-test)

    # Benchmarks — separate target, not part of ctest
    add_executable(${PROJECT_NAME}-bench
        tests/main.cpp
        tests/bytes_benchmark.cpp
    )
    target_link_libraries(${PROJECT_NAME}-bench PRIVATE GTest::gtest ${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME}-bench PROPERTIES EXCLUDE_FROM_ALL ON)
endif()

# ── Install & packaging ───────────────────────────────────────────────────────
include(package)
