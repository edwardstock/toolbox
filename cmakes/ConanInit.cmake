include(ConanBuild)

find_program(CONAN_BIN conan REQUIRED)

execute_process(
	COMMAND ${CONAN_BIN} remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
	ERROR_VARIABLE CONAN_ADD_REMOTE_ERR
	OUTPUT_VARIABLE CONAN_ADD_REMOTE_OUT
	RESULT_VARIABLE CONAN_ADD_REMOTE_RES
)
message(STATUS "Conan: ${CONAN_BIN} remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan")

if (CONAN_ADD_REMOTE_ERR)
	message(STATUS "Conan: Add remote error: ${CONAN_ADD_REMOTE_ERR}")
endif ()
if (CONAN_ADD_REMOTE_OUT)
	message(STATUS "Conan: Add remote output: ${CONAN_ADD_REMOTE_OUT}")
endif ()
if (CONAN_ADD_REMOTE_RES)
	message(STATUS "Conan: Add remote res: ${CONAN_ADD_REMOTE_RES}")
endif ()

conan_cmake_run(
	CONANFILE conanfile.txt
	BUILD missing
	BASIC_SETUP
)

if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
	include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup(TARGETS)
else ()
	message(WARNING "The file ${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()