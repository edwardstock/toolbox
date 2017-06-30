include(CheckIncludeFileCXX)


#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/autoconfig.in.h ${CMAKE_CURRENT_SOURCE_DIR}/include/autoconfig.h)

check_include_file_cxx("string" HAVE_MEMORY_H)
check_include_file_cxx("iosfwd" HAVE_IOSFWD_H)
check_include_file_cxx("vector" HAVE_VECTOR_H)
check_include_file_cxx("map" HAVE_MAP_H)
check_include_file_cxx("deque" HAVE_DEQUE_H)
check_include_file_cxx("unordered_map" HAVE_UNORDERED_MAP_H)
check_include_file_cxx("istream" HAVE_ISTREAM_H)
check_include_file_cxx("ostream" HAVE_OSTREAM_H)
check_include_file_cxx("regex" HAVE_REGEX_H)
check_include_file_cxx("sstream" HAVE_SSTREAM_H)
check_include_file_cxx("iostream" HAVE_IOSTREAM_H)
check_include_file_cxx("fstream" HAVE_FSTREAM_H)
check_include_file_cxx("exception" HAVE_EXCEPTION_H)