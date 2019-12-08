if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W2 /MP")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DEBUG /Od")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
	string(REGEX REPLACE "\\/W3" " " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	add_compile_options(/wd4068)
	add_compile_options(/wd4319)
	add_compile_options(/wd4251)
	add_compile_options(/wd4275)

	if (NOT ENABLE_SHARED)
		foreach (flag_var
		         CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
		         CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
			if (${flag_var} MATCHES "/MD")
				string(REGEX REPLACE "MD" "MT" ${flag_var} "${${flag_var}}")
			endif (${flag_var} MATCHES "/MD")
		endforeach (flag_var)
	endif ()

	foreach (flag_var
	         CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
	         CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if (${flag_var} MATCHES "/MD")
			string(REGEX REPLACE "MD" "MDd" ${flag_var} "${${flag_var}}")
		endif (${flag_var} MATCHES "/MD")
		if (${flag_var} MATCHES "/MT")
			string(REGEX REPLACE "MT" "MTd" ${flag_var} "${${flag_var}}")
		endif (${flag_var} MATCHES "/MT")
	endforeach (flag_var)

	message(STATUS ${CMAKE_CXX_FLAGS})
	message(STATUS ${CMAKE_CXX_FLAGS_DEBUG})
	message(STATUS ${CMAKE_CXX_FLAGS_RELEASE})
	message(STATUS ${CMAKE_CXX_FLAGS_MINSIZEREL})
	message(STATUS ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})

elseif (MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wno-unknown-pragmas -Wno-shift-count-overflow")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
else ()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wno-unused-parameter -Wno-shift-count-overflow")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -Wextra -pedantic")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()