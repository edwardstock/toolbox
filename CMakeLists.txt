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

set(CMAKE_CXX_LINKER_FLAGS "-static-libstdc++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC -Wextra -pedantic -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


set(PUBLIC_HEADERS include/${PROJECT_NAME}.hpp)

include_directories(include)


set(SOURCE_FILES
    ${PUBLIC_HEADERS}
    src/Logger.cpp)

include(options.cmake)

add_library(${PROJECT_NAME} INTERFACE)

option(ENABLE_MULTIBYTE "Enable wchat_t optimizations" ON)

if (ENABLE_MULTIBYTE)
	target_compile_definitions(${PROJECT_NAME} INTERFACE -DTOOLBOX_ENABLE_MB=1)
endif ()

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


include(cmakes/check_system.cmake)

install(
	FILES ${PUBLIC_HEADERS}
	DESTINATION include/${PROJECT}
)

if (${PROJECT_DEV})
	add_executable(dev main.cpp)
	target_link_libraries(dev toolboxpp)
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
	target_link_libraries(toolboxtest)

	if (WITH_COVERAGE)
		set(COVERAGE_LCOV_EXCLUDES 'libs/*' '/usr/include/*' 'tests/*')
		include(cmakes/CodeCoverage.cmake)
		append_coverage_compiler_flags()
		setup_target_for_coverage_lcov(
			NAME toolboxtest_coverage
			EXECUTABLE toolboxtest
			DEPENDENCIES toolboxtest
		)
	endif ()

endif ()
