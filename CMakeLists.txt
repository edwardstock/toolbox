cmake_minimum_required(VERSION 3.19)
project(toolbox
        VERSION 3.4.0
        LANGUAGES CXX
        DESCRIPTION "Toolbox - C++ helpers, especially for string and bytes handling"
)

set(CMAKE_CXX_STANDARD 14)
cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0091 NEW)

# Options
option(toolbox_BUILD_SHARED_LIBS "Build shared library instead of static" OFF)
option(toolbox_BUILD_TESTS "Build test target" Off)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cfg/version.in ${CMAKE_CURRENT_SOURCE_DIR}/version.info @ONLY NEWLINE_STYLE UNIX)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)

add_definitions(/D_CRT_SECURE_NO_WARNINGS=1)
include(modules/compilers_flags.cmake)

include(SystemDetector)

if (NOT toolbox_BUILD_SHARED_LIBS)
	set(TOOLBOX_BUILT_AS_STATIC 1)
endif ()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cfg/toolbox_config.in.h ${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox/toolbox_config.h)

set(MULTI_HEADERS
    include/toolbox/toolbox_config.h
    include/toolbox/data.hpp
    include/toolbox/data/bytes_data.h
    include/toolbox/data/bytes_buffer.h
    include/toolbox/data/utils.h
    include/toolbox/data/literals.h
    include/toolbox/strings.hpp
    include/toolbox/strings/regex.h
    include/toolbox/term.h
    include/toolbox/log.h
    include/toolbox/io.h
    include/toolbox/strings/decimal_formatter.h
    include/toolbox/time_profiler.h
    include/toolbox/collections.h
    include/toolbox/data/base64.h
    include/toolbox/data/basic_data.h
    include/toolbox/data/bytes_array.h
    include/toolbox/data/transformers.h
)

set(MULTI_SOURCES
    ${MULTI_HEADERS}
    src/data/bytes_data.cpp
    src/data/bytes_data.cpp
    src/data/bytes_buffer.cpp
    src/data/utils.cpp
    src/strings.cpp
    src/strings/regex.cpp
    src/io.cpp
    src/strings/decimal_formatter.cpp
    src/term.cpp
    src/log.cpp
    src/time_profiler.cpp
    src/data/base64.cpp
)

if (POLICY CMP0091)
	cmake_policy(SET CMP0091 NEW)
endif ()

if (toolbox_BUILD_SHARED_LIBS)
	message(STATUS "Build shared library")
	add_library(${PROJECT_NAME} SHARED ${MULTI_SOURCES})
	if (NOT WIN32)
		target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
	endif ()
else ()
	add_library(${PROJECT_NAME} STATIC ${MULTI_SOURCES})
endif ()
target_compile_definitions(${PROJECT_NAME} PRIVATE TOOLBOX_EXPORTS)


target_include_directories(${PROJECT_NAME} PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include>
)

if (CMAKE_CONAN_BUILD)
	message(STATUS "Enable conan: ${ENABLE_CONANCMAKE_CONAN_BUILD}")
else ()
	include(FetchContent)
endif ()

if (NOT HAVE_REGEX_H)
	message(WARNING "Disabled \"regex\"-based functionality")
endif ()


if (toolbox_BUILD_TESTS)
	if (NOT CMAKE_CONAN_BUILD)
		fetchcontent_declare(
			GTest
			GIT_REPOSITORY https://github.com/google/googletest.git
			GIT_TAG v1.13.0
		)
		set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
		set(gtest_force_shared_crt ${toolbox_BUILD_SHARED_LIBS} CACHE BOOL "" FORCE)
		fetchcontent_makeavailable(GTest)
	else ()
		message(STATUS "Finding gtest...")
		find_package(GTest CONFIG REQUIRED)
	endif ()

	if (APPLE)
		add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
		add_definitions(-D__GLIBCXX__)
	endif ()

	enable_testing()
	add_definitions(-DTOOLBOX_TESTING=1)

	add_executable(
		${PROJECT_NAME}-test
		tests/main.cpp
		tests/term_test.cpp
		tests/filesystem_test.cpp
		tests/numbers_test.cpp
		tests/collection_test.cpp
		tests/strings_test.cpp
		tests/data_test.cpp
		tests/bytes_benchmark.cpp
	)

	if (NOT MSVC)
		target_compile_options(${PROJECT_NAME}-test PRIVATE -Wno-missing-field-initializers)
	endif ()
	if (CMAKE_CONAN_BUILD)
		target_link_libraries(${PROJECT_NAME}-test GTest::GTest)
	else ()
		target_link_libraries(${PROJECT_NAME}-test gtest)
	endif ()
	target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})

	add_test(NAME test-all COMMAND ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME}-test)

endif ()

include(package)
