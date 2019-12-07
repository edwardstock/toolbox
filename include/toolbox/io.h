/*!
 * toolbox.
 * io.h
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_IO_H
#define TOOLBOXPP_IO_H

#include <string>
namespace toolbox {
namespace io {

std::string file_read_full(const std::string& path);
bool file_write_string(const std::string& path, const std::string& data);
bool file_exists(const std::string& path);

} // namespace io
} // namespace toolbox

#endif // TOOLBOXPP_IO_H
