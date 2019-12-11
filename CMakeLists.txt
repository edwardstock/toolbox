cmake_minimum_required(VERSION 3.0)
project(toolbox VERSION 3.0.4)
set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
if (WIN32)
	execute_process(COMMAND cmd /C "echo ${CMAKE_PROJECT_VERSION} > ${CMAKE_CURRENT_SOURCE_DIR}/version")
else ()
	execute_process(COMMAND bash -c "echo ${CMAKE_PROJECT_VERSION} > ${CMAKE_CURRENT_SOURCE_DIR}/version")
endif ()


add_definitions(/D_CRT_SECURE_NO_WARNINGS=1)
include(modules/compilers_flags.cmake)

option(ENABLE_TOOLBOX_CONAN "Enable dependency manager" On)
option(ENABLE_SHARED "Build shared library" Off)

set(TOOLBOX_EXPORTING 1)
if (ENABLE_SHARED)
	set(TOOLBOX_SHARED 1)
endif ()


include(SystemDetector)
include(options.cmake)


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
    include/toolbox/data/basic_data.h)

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
    include/toolbox/data/transformers.h
    )

if (ENABLE_SHARED)
	message(STATUS "Build shared library")
	add_library(${PROJECT_NAME} SHARED ${MULTI_SOURCES})
	set_property(TARGET ${PROJECT_NAME} PROPERTY
	             MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
else ()
	add_library(${PROJECT_NAME} STATIC ${MULTI_SOURCES})
	set_property(TARGET ${PROJECT_NAME} PROPERTY
	             MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif ()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CXX_FLAGS:         ${CMAKE_CXX_FLAGS}")
message(STATUS "CXX_DBG_FLAGS:     ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CXX_REL_FLAGS:     ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CXX_MINSIZE_FLAGS: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
message(STATUS "CXX_RELDBG_FLAGS:  ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

target_include_directories(${PROJECT_NAME} PUBLIC include)

if (ENABLE_TOOLBOX_CONAN)
	include(ConanInit)
	conan_init()
endif ()

message(STATUS "Enable conan: ${ENABLE_TOOLBOX_CONAN}")


if (ENABLE_TOOLBOX_TEST)
	set(gtest_force_shared_crt ${ENABLE_SHARED} CACHE BOOL "" FORCE)

	if (APPLE)
		add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
		add_definitions(-D__GLIBCXX__)
	endif ()

	enable_testing()
	add_definitions(-DTOOLBOX_TESTING=1)

	add_executable(${PROJECT_NAME}-test
	               tests/main.cpp
	               tests/term_test.cpp
	               tests/filesystem_test.cpp
	               tests/numbers_test.cpp
	               tests/collection_test.cpp
	               tests/strings_test.cpp
	               tests/data_test.cpp tests/bytes_benchmark.cpp)

	if (ENABLE_PVS)
		include(PVS-Studio)
		pvs_studio_add_target(TARGET ${PROJECT_NAME}.analyze ALL
		                      OUTPUT FORMAT errorfile
		                      CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/pvs.cfg
		                      ANALYZE ${PROJECT_NAME}-test
		                      SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/include
		                      LOG target.err)
	endif ()

	if (NOT MSVC)
		target_compile_options(${PROJECT_NAME}-test PRIVATE -Wno-missing-field-initializers)
	endif ()
	target_link_libraries(${PROJECT_NAME}-test CONAN_PKG::gtest)
	target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})

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
		include(modules/CodeCoverage.cmake)
		append_coverage_compiler_flags()
		setup_target_for_coverage_lcov(
			NAME ${PROJECT_NAME}-coverage
			EXECUTABLE ${PROJECT_NAME}-test
			DEPENDENCIES ${PROJECT_NAME}-test
		)
	endif ()

	add_test(NAME test-all COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-test)

endif ()

install(
	TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/toolbox
	DESTINATION include
)
