cmake_minimum_required(VERSION 3.0)
project(cpphelpers)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER_FLAGS "-Werror -Wfatal-error -pedantic")
set(CMAKE_CXX_LINKER_FLAGS "-static-libstdc++")

include(options.cmake)

set(PUBLIC_HEADERS
    include/cpphelpers.h
    )

include_directories(include)


set(SOURCE_FILES
    ${PUBLIC_HEADERS}
    src/Collections.cpp
    src/Console.cpp
    src/Numbers.cpp
    src/Strings.cpp
    )

add_library(cpphelpers ${SOURCE_FILES})

install(
	TARGETS cpphelpers
	DESTINATION lib${LIB_SUFFIX}
)

install(
	FILES ${PUBLIC_HEADERS}
	DESTINATION include/${PROJECT}
)