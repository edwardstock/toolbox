#pragma once

#include "toolbox/toolbox_config.h"
#include <vector>
#include <stdexcept>

namespace toolbox {
namespace data {

template<typename T>
class TOOLBOX_API slice;

template<typename T>
class TOOLBOX_API slice_compatible {
public:
    using const_iterator = typename std::vector<T>::const_iterator;
    using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;
    using size_type = typename std::vector<T>::size_type;

    virtual ~slice_compatible() = default;
    virtual slice<T> take_slice(size_type start, size_type end) const = 0;
    virtual size_type size() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;

    slice<T> take_slice_n(size_type start, size_type length) const {
        return take_slice(start, start + length);
    }

    slice<T> take_slice_from(size_type start) const {
        return take_slice(start, size());
    }

    slice<T> take_slice_first(size_type end) const {
        return take_slice(0, end);
    }

    slice<T> take_slice_last(size_type end) const {
        return take_slice(size() - end, size());
    }
};

template<typename T>
class TOOLBOX_API slice : public slice_compatible<T> {
public:
    using const_iterator = typename slice_compatible<T>::const_iterator;
    using const_reverse_iterator = typename slice_compatible<T>::const_reverse_iterator;
    using size_type = typename slice_compatible<T>::size_type;

    explicit slice(
        const const_iterator& start,
        const const_iterator& end
    ) : m_start(start)
      , m_end(end)
      , m_debug(start, end) { }

    slice(const slice& other) = default;
    slice(slice&& other) = default;

    slice& operator=(const slice& other) = default;

    std::vector<T> get() const {
        return std::vector<T>(cbegin(), cend());
    }

    size_type size() const override {
        return std::distance(m_start, m_end);
    }

    operator bool() const noexcept {
        return !empty();
    }

    bool operator==(const slice& other) const noexcept {
        return m_start == other.m_start && m_end == other.m_end;
    }

    bool operator==(const std::vector<T>& other) const noexcept {
        return std::equal(cbegin(), cend(), other.begin(), other.end());
    }

    bool operator!=(const slice& other) const noexcept {
        return !(*this == other);
    }

    bool operator!=(const std::vector<T>& other) const noexcept {
        return !std::equal(cbegin(), cend(), other.begin(), other.end());
    }

    bool empty() const noexcept {
        return m_start == m_end;
    }

    const T& operator[](size_type pos) const noexcept {
        return *(m_start + pos);
    }

    const T& at(size_type pos) const {
        if (pos >= size()) {
            throw std::out_of_range("position is out of range");
        }
        return *(m_start + pos);
    }

    const T& front() const { return *m_start; }

    const T& back() const { return *(m_end - 1); }

    const T* data() const noexcept {
        return &(*m_start);
    }

    slice<T> take_slice(size_type start, size_type end) const override {
        if (end > size()) {
            throw std::out_of_range("end position is out of range");
        }
        if (start > end) {
            throw std::out_of_range("start position is greater than end");
        }
        return slice<T>(m_start + start, m_start + end);
    }

    const_iterator cbegin() const noexcept override { return m_start; }
    const_iterator cend() const noexcept override { return m_end; }

    const_iterator begin() const noexcept override { return m_start; }
    const_iterator end() const noexcept override { return m_end; }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }
private:
    const_iterator m_start;
    const_iterator m_end;

    std::vector<T> m_debug;
};

} // namespace data
} // namespace toolbox
