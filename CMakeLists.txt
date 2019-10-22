cmake_minimum_required(VERSION 3.0)
project(toolboxpp VERSION 2.2.2)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmakes)

if (ENABLE_TOOLBOX_CONAN)
	include(ConanInit)
endif ()
include(SystemDetector)
include(options.cmake)

set(CMAKE_CXX_STANDARD 14)

# linux clang
if (NOT APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
elseif (APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
	set(CMAKE_CXX_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -std=c++11 -stdlib=libc++ -undefined dynamic_lookup")
endif ()

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W2 /std:c++14")
	set(CMAKE_CXX_FLAGS_DEBUG "/DEBUG /Od")
	set(CMAKE_CXX_FLAGS_RELEASE "/O2")
	add_definitions(/D_CRT_SECURE_NO_WARNINGS=1)
	string(REGEX REPLACE "\\/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -Wextra -pedantic -Wno-unused-parameter")
	set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()


include_directories(include)

set(PUBLIC_HEADERS include/toolboxpp.hpp)
set(SOURCE_FILES ${PUBLIC_HEADERS})


add_library(${PROJECT_NAME} INTERFACE)


if (ENABLE_MULTIBYTE)
	# DISABLED for now, it's not just working
	#	add_definitions(-DTOOLBOX_ENABLE_MB=1)
	#	if (MSVC)
	#		add_definitions(/D_UNICODE=1 /D_MBCS=1)
	#	endif ()
endif ()


if (PROJECT_DEV)
	add_executable(dev main.cpp)
	target_link_libraries(dev toolboxpp)
endif ()


if (WITH_TEST)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	if (NOT ENABLE_TOOLBOX_CONAN)
		add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/googletest)
		add_library(CONAN_PKG::gtest ALIAS gtest)
	endif ()

	if (APPLE)
		add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
		add_definitions(-D__GLIBCXX__)
	endif ()

	enable_testing()
	add_definitions(-DTOOLBOX_TESTING=1)

	add_executable(toolboxtest
	               tests/main.cpp
	               tests/console_test.cpp
	               tests/filesystem_test.cpp
	               tests/numbers_test.cpp
	               tests/collection_test.cpp
	               tests/strings_test.cpp)

	if (NOT MSVC)
		target_compile_options(toolboxtest PRIVATE -Wno-missing-field-initializers)
	endif ()
	target_include_directories(toolboxtest INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/toolboxpp.hpp)
	target_link_libraries(toolboxtest CONAN_PKG::gtest)

	if (WITH_COVERAGE)
		set(COVERAGE_LCOV_EXCLUDES
		    'libs/*'
		    '/usr/include/*'
		    'tests/*'
		    'boost/*'
		    '/usr/include/*'
		    '/usr/local/*'
		    'v1'
		    '4.9'
		    )
		include(cmakes/CodeCoverage.cmake)
		append_coverage_compiler_flags()
		setup_target_for_coverage_lcov(
			NAME toolboxtest_coverage
			EXECUTABLE toolboxtest
			DEPENDENCIES toolboxtest
		)
	endif ()

	add_test(NAME test-all COMMAND ${CMAKE_CURRENT_BINARY_DIR}/toolboxtest)

endif ()

install(
	FILES ${PUBLIC_HEADERS}
	DESTINATION include
)
