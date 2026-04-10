
/// @file bytes_buffer.h
/// @brief Byte buffer with pop (consume) operations from front and back.

#ifndef TOOLBOXPP_BYTES_BUFFER_H
#define TOOLBOXPP_BYTES_BUFFER_H

#include "bytes_data.h"
#include "toolbox/toolbox_config.h"

namespace toolbox {
namespace data {

/// Byte buffer that extends bytes_data with destructive read (pop) operations.
///
/// Data is removed from the buffer as it is read, making it suitable for
/// stream-like consumption of incoming byte sequences.
class TOOLBOX_API bytes_buffer : public bytes_data {
public:
    bytes_buffer();
    bytes_buffer(const basic_data& other);
    bytes_buffer(basic_data&& other);
    explicit bytes_buffer(std::size_t size);
    bytes_buffer(std::initializer_list<uint8_t> ilist);
    bytes_buffer(std::initializer_list<std::vector<uint8_t>> ilist);
    bytes_buffer(std::vector<uint8_t> data);
    bytes_buffer(const slice<uint8_t> data);
    bytes_buffer(const uint8_t* data, size_t len);
    /// Construct from a hex string (e.g. "deadbeef").
    bytes_buffer(const char* hexString);
    /// Construct from a hex string (e.g. "deadbeef").
    bytes_buffer(const std::string& hexString);

    /// Pop one byte from the front, write it at @p outPosition in @p out.
    /// @param outPosition write offset in the output container
    /// @param out destination container
    /// @return number of bytes popped (1 on success, 0 if empty)
    size_t pop_front_to(size_t outPosition, basic_data<uint8_t>& out);

    /// Pop @p readLength bytes from the front, write starting at @p position in @p out.
    /// @param readLength number of bytes to pop
    /// @param position write offset in the output container
    /// @param out destination container
    /// @return number of bytes actually popped
    size_t pop_front_to(size_t readLength, size_t position, basic_data<uint8_t>& out);

    /// Pop one byte from the back and append it to @p out.
    /// @param out destination container
    /// @return number of bytes popped
    size_t pop_back_to(basic_data<uint8_t>& out);

    /// Pop @p readLength bytes from the back and append them to @p out.
    /// @param readLength number of bytes to pop
    /// @param out destination container
    /// @return number of bytes actually popped
    size_t pop_back_to(size_t readLength, basic_data<uint8_t>& out);

    /// Pop @p readLength bytes from the front, write starting at @p position iterator in @p out.
    /// @param readLength number of bytes to pop
    /// @param position write iterator in the output container
    /// @param out destination container
    /// @return number of bytes actually popped
    size_t pop_front_to(size_t readLength, iterator position, basic_data<uint8_t>& out);
};

} // namespace data
} // namespace toolbox

#endif //TOOLBOXPP_BYTES_BUFFER_H
