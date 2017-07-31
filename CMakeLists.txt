cmake_minimum_required(VERSION 3.0)
project(cpphelpers)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER_FLAGS "-Werror -Wfatal-error -pedantic")
set(CMAKE_CXX_LINKER_FLAGS "-static-libstdc++")

if (APPLE)
	message(STATUS "Build for Darwin")
	set(CMAKE_CXX_COMPILER_FLAGS "${CMAKE_CXX_COMPILER_FLAGS} -arch=x86_64")
	set(CMAKE_CXX_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -undefined dynamic_lookup")
endif ()

include(options.cmake)
include(check_system.cmake)

set(PUBLIC_HEADERS
    include/cpphelpers.h
    )

include_directories(include)


set(SOURCE_FILES
    ${PUBLIC_HEADERS}
    src/Console.cpp
    src/Numbers.cpp
    src/Strings.cpp
    src/SimpleProfiler.cpp
    )

if (ENABLE_STATIC)
	add_library(cpphelpers STATIC ${SOURCE_FILES})
else ()
	add_library(cpphelpers SHARED ${SOURCE_FILES})
endif ()

install(
	TARGETS cpphelpers
	DESTINATION lib${LIB_SUFFIX}
)

install(
	FILES ${PUBLIC_HEADERS}
	DESTINATION include/${PROJECT}
)

if (${PROJECT_DEV})
	add_executable(dev main.cpp)
	target_link_libraries(dev cpphelpers)
else ()
	set(CMAKE_BUILD_TYPE "Release")
endif ()