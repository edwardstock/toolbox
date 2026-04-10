/// @file io.h
/// @brief Simple filesystem helpers (read, write, check existence).

#ifndef TOOLBOXPP_IO_H
#define TOOLBOXPP_IO_H

#include "toolbox/toolbox_config.h"

#include <string>

namespace toolbox {
namespace io {

/// Read the entire contents of a file into a string.
/// @param path absolute or relative file path
/// @return file contents as a string
TOOLBOX_API std::string file_read_full(const std::string& path);

/// Write a string to a file, replacing any existing contents.
/// @param path absolute or relative file path
/// @param data string to write
/// @return true on success, false on failure
TOOLBOX_API bool file_write_string(const std::string& path, const std::string& data);

/// Check whether a file exists at the given path.
/// @param path absolute or relative file path
/// @return true if the file exists
TOOLBOX_API bool file_exists(const std::string& path);

} // namespace io
} // namespace toolbox

#endif // TOOLBOXPP_IO_H
