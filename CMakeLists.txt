cmake_minimum_required(VERSION 3.0)
project(toolboxpp)

if (APPLE)
	# Fix behavior of CMAKE_CXX_STANDARD when targeting macOS.
	if (POLICY CMP0025)
		cmake_policy(SET CMP0025 NEW)
	endif ()
endif ()

# gnu
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

# linux clang
if (NOT APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
elseif (APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
	set(CMAKE_CXX_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -std=c++11 -stdlib=libc++ -undefined dynamic_lookup")
elseif (MSVC)
	message(STATUS "Build for Windows")
	add_compile_options(/W1)
endif ()

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_LINKER_FLAGS "-static-libstdc++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


set(PUBLIC_HEADERS
    include/toolboxpp.h
    include/toolboxppconfig.h
    )

include_directories(include)


set(SOURCE_FILES
    ${PUBLIC_HEADERS}
    src/Console.cpp
    src/Numbers.cpp
    src/Strings.cpp
    src/FileSystem.cpp
    src/Logger.cpp)

include(options.cmake)

if (ENABLE_STATIC)
	add_library(toolboxpp STATIC ${SOURCE_FILES})
else ()
	add_library(toolboxpp SHARED ${SOURCE_FILES})
endif ()


include(check_system.cmake)

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
	enable_testing()
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
		tests/filesystem_test.cpp
		tests/numbers_test.cpp
		tests/collection_test.cpp
		tests/strings_test.cpp)

	target_compile_options(toolboxtest PRIVATE -Wno-missing-field-initializers)

	target_link_libraries(toolboxtest ${PROJECT_NAME})
	target_link_libraries(toolboxtest gtest gtest_main)

	if (WITH_COVERAGE)
		include(cmakes/CodeCoverage.cmake)
		append_coverage_compiler_flags()
		setup_target_for_coverage(
			NAME toolboxtest_coverage
			EXECUTABLE toolboxtest
			DEPENDENCIES toolboxtest
		)
	endif ()

endif ()
