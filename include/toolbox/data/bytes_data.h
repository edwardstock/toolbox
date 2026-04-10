
/// @file bytes_data.h
/// @brief Byte container with hex encoding, numeric conversions, and stream I/O.

#ifndef TOOLBOXPP_BYTES_DATA_H
#define TOOLBOXPP_BYTES_DATA_H

#include "basic_data.h"
#include "toolbox/toolbox_config.h"

#include <functional>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <type_traits>
#include <vector>

namespace toolbox {
namespace data {

/// Byte container built on top of basic_data<uint8_t>.
///
/// Adds hex string construction/output, numeric read/write helpers for
/// multi-byte integers, and stream operators.
class TOOLBOX_API bytes_data : public basic_data<uint8_t> {
public:
    using slice_type = slice<uint8_t>;

    /// Construct from a vector of signed chars.
    static bytes_data from_chars(const std::vector<char>& data);
    /// Construct from a raw signed char pointer and length.
    static bytes_data from_chars(const char* data, size_t len);
    /// Construct from a raw string (each character becomes a byte, not hex).
    static bytes_data from_string_raw(const std::string& data);

    bytes_data();
    bytes_data(const basic_data<uint8_t>& other);
    bytes_data(basic_data<uint8_t>&& other);
    explicit bytes_data(std::size_t size);
    bytes_data(std::initializer_list<uint8_t> ilist);
    bytes_data(std::initializer_list<std::vector<uint8_t>> ilist);
    bytes_data(std::vector<uint8_t> data);
    bytes_data(const slice_compatible<uint8_t>& data);
    bytes_data(const uint8_t* data, size_t len);

    /// Construct from a hex string (e.g. "deadbeef").
    bytes_data(const char* hexString);
    /// Construct from a hex string (e.g. "deadbeef").
    bytes_data(const std::string& hexString);

    /// Convert the contents to a hex string (no prefix).
    /// @return hex representation
    [[nodiscard]] std::string to_hex() const {
        return to_hex("");
    }

    /// Convert the contents to a hex string with the given prefix.
    /// @param prefix string prepended to the output (e.g. "0x")
    /// @return hex representation
    [[nodiscard]] virtual std::string to_hex(const std::string& prefix) const;

    /// Convert the raw bytes to a string (each byte becomes a character).
    [[nodiscard]] virtual std::string to_string() const;

    /// Erase all stored bytes and set size to zero.
    void clear() override;

    explicit operator uint8_t() const;
    explicit operator char() const;
    explicit operator uint16_t() const;
    explicit operator uint32_t() const;
    explicit operator uint64_t() const;

    size_type write_batch(std::map<basic_data::size_type, uint8_t>&& vals) override {
        return basic_data::write_batch(std::move(vals));
    }

    template<typename InputIterator>
    size_type write(iterator position, InputIterator start, InputIterator end) {
        size_type dist = std::distance(begin(), position);
        return write(dist, start, end);
    }

    template<typename InputIterator>
    size_type write(size_type pos, InputIterator start, InputIterator end) {
        std::map<size_type, uint8_t> tmp;
        size_type i = pos;
        for (auto it = start; it != end; ++it) {
            tmp[i++] = *it;
        }

        return write_batch(std::move(tmp));
    }

    virtual size_t write(size_t pos, uint16_t val);
    virtual size_t write(size_t pos, int32_t val);
    virtual size_t write(size_t pos, uint32_t val);
    virtual size_t write(size_t pos, uint64_t val);

    size_type write(basic_data::size_type pos, const uint8_t* data, size_t dataLen) override {
        return basic_data::write(pos, data, dataLen);
    }

    size_type write(basic_data::size_type pos, const basic_data<uint8_t>& data) override {
        return basic_data::write(pos, data);
    }

    size_type write(basic_data::size_type pos, basic_data<uint8_t>&& data) override {
        return basic_data::write(pos, data);
    }

    size_type write(basic_data::size_type pos, const std::vector<uint8_t>& data) override {
        return basic_data::write(pos, data);
    }

    size_type write(basic_data::size_type pos, std::vector<uint8_t>&& data) override {
        return basic_data::write(pos, data);
    }

    size_type write(basic_data::size_type pos, uint8_t val) override {
        return basic_data::write(pos, val);
    }

    size_type write_back(const std::vector<uint8_t>& data) override {
        return basic_data::write_back(data);
    }

    size_type write_back(std::vector<uint8_t>&& data) override {
        return basic_data::write_back(data);
    }

    size_type write_back(const basic_data<uint8_t>& data) override {
        return basic_data::write_back(data);
    }

    size_type write_back(basic_data<uint8_t>&& data) override {
        return basic_data::write_back(data);
    }

    size_type write_back(const uint8_t* data, size_t len) override {
        return basic_data::write_back(data, len);
    }

    size_t write_back(uint8_t val) override {
        return basic_data::write_back(val);
    }

    size_t write_back(char val);
    size_t write_back(uint16_t val);
    size_t write_back(uint32_t val);
    size_t write_back(uint64_t val);

    void push_back(const std::vector<uint8_t>& data) override {
        basic_data::push_back(data);
    }

    void push_back(const uint8_t* data, size_t len) override {
        basic_data::push_back(data, len);
    }

    void push_back(const_iterator start, const_iterator end) override {
        basic_data::push_back(start, end);
    }

    void push_back(iterator start, iterator end) override {
        basic_data::push_back(start, end);
    }

    void push_back(const basic_data<uint8_t>& data) override {
        basic_data::push_back(data);
    }

    void push_back(basic_data&& data) override {
        basic_data::push_back(std::move(data));
    }

    void push_back(uint8_t val);
    void push_back(char val);

    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    void push_back(T val) {
        const std::size_t size = sizeof(T);

        for (std::size_t i = 0; i < size; ++i) {
            m_data.push_back((val >> (8 * (size - 1 - i))) & 0xFF);
        }
    }

    /// Write a raw string as bytes (each character becomes one byte, not hex).
    /// @param val string to write
    void push_back(const std::string& val);

    /// Write raw characters as bytes.
    /// @param val character array
    /// @param len number of characters to write
    void push_back(const char* val, size_t len);

    template<typename T>
    T to_num() const {
        return to_num<T>(0);
    }

    template<typename T>
    T to_num_any_size(size_t from) const {
        return to_num_any_size<T>(from, from + sizeof(T));
    }

    template<typename FromSize, typename ToSize>
    ToSize to_num_any_size(size_t pos) const {
        return to_num_any_size<ToSize>(pos, pos + sizeof(FromSize));
    }

    template<typename T>
    T to_num_any_size(size_t from, size_t to) const {
        auto data = take_range(from, std::min(to, size()));

        size_t len = data.size();
        T out = 0;

        if (len < sizeof(T)) {
            len = sizeof(T);

            for (size_t i = 0; i < len; i++) {
                if (i < data.size()) {
                    out |= static_cast<T>(data[i]) << (8u * i);
                }
            }
        } else {
            for (size_t i = 0, p = (len - 1); i < len; i++, p--) {
                out |= static_cast<T>(data[i]) << (8u * p);
            }
        }

        return out;
    }

    template<typename T>
    [[nodiscard]] uint64_t to_num_any(size_t from) const {
        return to_num_any(from, from + sizeof(T));
    }

    /// Be careful: reads from zero to a maximum of 8 bytes to avoid overflow.
    /// @return integer value decoded from stored bytes
    [[nodiscard]] uint64_t to_num_any() const;
    [[nodiscard]] uint64_t to_num_any(size_t from, size_t to) const;

    template<typename T>
    T to_num(size_t from, size_t to) const {
        auto data = take_range(from, to);

        size_t len = sizeof(T);
        if (len == 1) {
            return data[0];
        } else if (len == 2) {
            return static_cast<uint64_t>(data[0]) << 8u | static_cast<uint64_t>(data[1]);
        } else if (len == 4) {
            return static_cast<uint64_t>(data[0]) << 24u | static_cast<uint64_t>(data[1]) << 16u |
                   static_cast<uint64_t>(data[2]) << 8u | static_cast<uint64_t>(data[3]);
        } else if (sizeof(uint64_t) == len) {
            return static_cast<uint64_t>(data[0]) << 56u | static_cast<uint64_t>(data[1]) << 48u |
                   static_cast<uint64_t>(data[2]) << 40u | static_cast<uint64_t>(data[3]) << 32u |
                   static_cast<uint64_t>(data[4]) << 24u | static_cast<uint64_t>(data[5]) << 16u |
                   static_cast<uint64_t>(data[6]) << 8u | static_cast<uint64_t>(data[7]);
        }

        return 0;
    }

    template<typename T>
    T to_num(size_t readFrom) const {
        return to_num<T>(readFrom, readFrom + sizeof(T));
    }

    /// Write all data to the output stream as raw characters.
    TOOLBOX_API friend std::ostream& operator<<(std::ostream& os, const bytes_data& data);

    /// Read raw characters from the input stream and replace all existing data.
    /// Hex strings are NOT decoded; they are stored as raw characters.
    /// \return input stream chain
    TOOLBOX_API friend std::istream& operator>>(std::istream& is, bytes_data& data);
};

#ifndef _MSC_VER
TOOLBOX_API std::ostream& operator<<(std::ostream& os, const bytes_data& data);
TOOLBOX_API std::istream& operator>>(std::istream& is, bytes_data& data);
#endif

} // namespace data
} // namespace toolbox

#endif // TOOLBOXPP_BYTES_DATA_H
