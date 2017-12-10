cmake_minimum_required(VERSION 3.0)
project(toolboxpp)

set(CXX_FLAGS "-Wall -Wextra -pedantic -Wno-unused-parameter")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_LINKER_FLAGS "-static-libstdc++")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS} -O3")

if (APPLE)
	message(STATUS "Build for Darwin")
	set(CMAKE_CXX_COMPILER_FLAGS "${CMAKE_CXX_COMPILER_FLAGS} -stdlib=libc++ -arch=x86_64")
	set(CMAKE_CXX_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -stdlib=libc++ -undefined dynamic_lookup")
endif ()

include(options.cmake)
include(check_system.cmake)

set(PUBLIC_HEADERS
    include/toolboxpp.h
    include/toolboxconfig.h)

include_directories(include)


set(SOURCE_FILES
    ${PUBLIC_HEADERS}
    src/Console.cpp
    src/Numbers.cpp
    src/Strings.cpp
    src/FileSystem.cpp src/Logger.cpp)

if (ENABLE_STATIC)
	add_library(toolboxpp STATIC ${SOURCE_FILES})
else ()
	add_library(toolboxpp SHARED ${SOURCE_FILES})
endif ()

install(
	TARGETS toolboxpp
	DESTINATION lib${LIB_SUFFIX}
)

install(
	FILES ${PUBLIC_HEADERS}
	DESTINATION include/${PROJECT}
)

if (${PROJECT_DEV})
	add_executable(dev main.cpp)
	target_link_libraries(dev toolboxpp)
else ()
	set(CMAKE_BUILD_TYPE "Release")
endif ()


if (${WITH_TEST})
	add_definitions(-DTOOLBOX_TESTING=1)
	set(GTEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/googletest/googletest)

	include_directories(${GTEST_SOURCE_DIR}/include ${GTEST_SOURCE_DIR})
	add_subdirectory(${GTEST_SOURCE_DIR})

	if (WIN32)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
	elseif (APPLE)
		add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
		add_definitions(-D__GLIBCXX__)
	endif ()

	add_executable(
		toolboxtest
		tests/console_test.cpp
		tests/filesystem_test.cpp tests/numbers_test.cpp tests/collection_test.cpp tests/strings_test.cpp)

	target_link_libraries(toolboxtest gtest gtest_main)
	target_link_libraries(toolboxtest ${PROJECT_NAME})

endif ()
