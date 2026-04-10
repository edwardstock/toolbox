# Output directories - set only when toolbox is the top-level project so we
# don't clobber the consumer's build layout when used via FetchContent /
# add_subdirectory.
if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    if(MSVC)
        set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    endif()
endif()

# toolbox_set_compiler_flags(<target>)
#
# Apply toolbox's recommended compiler flags to <target>.
# Uses target_compile_options so the flags are strictly scoped and never
# leak into a consumer's own targets.
function(toolbox_set_compiler_flags target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W2
            /MP                  # parallel compilation
            /wd4068              # unknown pragma
            /wd4319              # '~': zero extension of int to unsigned long
            /wd4251              # class needs dll-interface to be used by clients
            /wd4275              # non dll-interface base class
            $<$<CONFIG:Debug>:/Od>
            $<$<CONFIG:Release>:/O2>
        )
    elseif(MINGW)
        target_compile_options(${target} PRIVATE
            -Wno-unknown-pragmas
            -Wno-shift-count-overflow
            $<$<CONFIG:Debug>:-g -O0 -Wall>
            $<$<CONFIG:Release>:-O3>
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wno-unused-parameter
            -Wno-shift-count-overflow
            $<$<CONFIG:Debug>:-g -O0 -Wall -Wextra -pedantic -Wno-attributes>
            $<$<CONFIG:Release>:-O3>
        )
    endif()
endfunction()
