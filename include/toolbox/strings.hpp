/*!
 * toolbox.
 * strings.hpp
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_STRINGS_H
#define TOOLBOXPP_STRINGS_H

#include "toolbox_config.h"

#include <algorithm>
#include <locale>
#include <sstream>
#include <string>
#include <utility>
#include <cstdio>
#include <vector>
#include <type_traits>

namespace toolbox {
namespace strings {

using strlen_t = std::string::size_type;

template<typename tb_char_t>
struct string_equaling {
    explicit string_equaling(const std::locale& locale)
        : _locale(locale) { }

    bool operator()(tb_char_t ch1, tb_char_t ch2) {
        return std::toupper(ch1, _locale) == std::toupper(ch2, _locale);
    }
private:
    const std::locale& _locale;
};

template<typename T>
size_t compare(const T& str1, const T& str2, const std::locale& loc = std::locale()) {
    typename T::const_iterator it =
        std::search(str1.begin(), str1.end(), str2.begin(), str2.end(), string_equaling<typename T::value_type>(loc));
    if (it != str1.end())
        return it - str1.begin();

    return std::string::npos; // not found
}

/**
 *
 * @param source
 * @param substring
 * @return
 */
TOOLBOX_API bool has_substring_icase(const std::string& substring, const std::string& source);

/**
 * Search substring in string
 * @param source
 * @param substring
 * @return
 */
TOOLBOX_API bool has_substring(const std::string& substring, const std::string& source);

/**
 * Search character in string
 * @param source
 * @param substring
 * @return
 */
TOOLBOX_API bool has_substring(char substring, const std::string& source);

/**
 * Splits string by char delimiter to vector list
 * @param source
 * @param delimiter
 * @return
 */
TOOLBOX_API std::vector<std::string> split(const std::string& source, const std::string& delimiter);

/**
 * Splits string by delimiter to vector list
 * @param source
 * @param delimiter
 * @return
 */
TOOLBOX_API std::vector<std::string> split(const std::string& source, const char& delimiter);

/*!
 * \brief Split given string by max length.
 * Example:
 *      <code>std::vector<std::string>{"ab", "c"}</code>
 * \param src source string
 * \param max max length
 * \return separated string to vector. If src length less than
 * maximum, vector will contain 1 element with src
 */
TOOLBOX_API std::vector<std::string> split_by_len(const std::string& src, size_t max);

/**
 * Split string by char delimiter into pair
 * @param source
 * @param delimiter
 * @return returns pair of 2 empty string if delimiter not found
 */
TOOLBOX_API std::pair<std::string, std::string> split_pair(const std::string& source, const char& delimiter);

TOOLBOX_API std::pair<std::string, std::string> split_pair(const std::string& source, const std::string& delimiter);

/**
 * \brief Repeat input string N times
 * \param in input string
 * \param n number of times to repeat
 * \return repeated string
 */
TOOLBOX_API std::string repeat(const std::string& in, size_t n = 1);

/*!
 * \brief Repeat input char N times
 * \param in input char
 * \param n number of times to repeat
 * \return repeated string
 */
TOOLBOX_API std::string repeat(char in, size_t n = 1);

/*!
 * \brief Remove from string symbols: \t\n\r\x0B
 * \param in any string
 * \return string without special symbols
 */
TOOLBOX_API std::string trim(const std::string& in);

/**
 * \brief Remove \t\n\r\x0B from the given string
 * \param in any string reference
 */
TOOLBOX_API void trim_ref(std::string& in);

/**
 * Replace first [search] occurrence with [replace] string in [source] string
 * Mutates original string
 * @param search
 * @param replace
 * @param source Mutable
 * @return
 */
TOOLBOX_API void replace(
    const std::string& search,
    const std::string& replace,
    std::string& source
);

/**
 * Replace all [search] occurrences with [replace] string in [source] string.
 * Mutates original string
 * @param search
 * @param replace
 * @param source Mutable
 */
TOOLBOX_API void substr_replace_all(
    const std::string& search,
    const std::string& replace,
    std::string& source
);

/*!
 * Replace all the given [search] strings (vector) with [replace] strings (another vector)
 * Mutates original string
 * @param search Search vector
 * @param replace Replace vector
 * @param source mutable string
 */
TOOLBOX_API void substr_replace_all(
    const std::vector<std::string>& search,
    const std::vector<std::string>& replace,
    std::string& source
);

/**
 * Replace all the given [search] strings (vector) with [replace] string in [source] string.
 * Mutates original string
 * @param search Search vector
 * @param replace Replace string
 * @param source source value to be updated
 */
TOOLBOX_API void substr_replace_all(
    const std::vector<std::string>& search,
    const std::string& replace,
    std::string& source
);

/**
 * Replaces first [search] occurrence with [replace] string in [source] string
 * @param search
 * @param replace
 * @param source Immutable
 * @return same string if nothing to replace, or copy of the [source] with replaced values
 */
TOOLBOX_API std::string substr_replace_ret(
    const std::string& search,
    const std::string& replace,
    const std::string& source
);

/**
 * Replace all [search] occurrences with [replace] string in [source] string
 * @param search
 * @param replace
 * @param source
 * @return same string if nothing to replace, or copy of the [source] with replaced values
 */
TOOLBOX_API std::string substr_replace_all_ret(
    const std::string& search,
    const std::string& replace,
    const std::string& source
);

/**
 * Replace all the given [search] strings (vector) with [replace] strings (another vector)
 * and return copy of the [source]
 * Example:
 * <code>
 *  substr_replace_all_ret(
 *      {"before1","before2"},
 *      {"after1","after2"},
 *      "Source string with before1 and before2"
 *  )
 *  </code>
 * @param search
 * @param replace
 * @param source
 * @return same string if nothing to replace, or copy of the [source] with replaced values
 */
TOOLBOX_API std::string substr_replace_all_ret(
    const std::vector<std::string>& search,
    const std::vector<std::string>& replace,
    const std::string& source
);

/**
 * Replace all the given [search] strings (vector> with [replace] string and return copy of the [source]
 * Example:
 * substringReplaceAll({"before1","before2"}, "after", "Source string with before1 and before2")
 *
 * @param search
 * @param replace
 * @param source
 * @return
 */
TOOLBOX_API std::string substr_replace_all_ret(
    const std::vector<std::string>& search,
    const std::string& replace,
    const std::string& source
);

/**
 * Find "removable" string in "source" and remove it
 * @param source Mutable value!
 * @param removable
 */
TOOLBOX_API void substr_remove(std::string& source, const std::string& removable);

/**
 * Find all "removables" strings in "source" and remove it
 * @param source
 * @param removables
 */
TOOLBOX_API void substr_remove_all(std::string& source, std::vector<std::string> removables);

/**
 * Getting substring from specified character
 * @param source
 * @param whence Whence start to cut. You can inverse character to cut from the
 end of "source"
 * Example:
 *  from start:
 *      string s = "abcdef";
                Strings::substr_inverse(s, 'c');
                result: "ab"

        from end:
                string s = "abcdef";
                Strings::substr_inverse(s, ~'c');
                result: "def"
 * @return
 */
TOOLBOX_API std::string substr_inverse(const std::string& source, char whence);

TOOLBOX_API std::string substr_inverse(
    const std::string& source,
    const std::string& begin,
    const std::string& end,
    long offset = 0
);

TOOLBOX_API std::string substr_inverse(const std::string& source, char begin, char end, long offset = 0);

TOOLBOX_API std::string substr_inverse(const std::string& source, const std::string& begin);

/**
 * How works:
 * source: "aaa bbb ccc"
 * search: "bbb"
 * width: 7
 * 1. find position of "search" in "source" = 5
 * 2. find center of "search": centerOfSearch = 5 + (search.length() / 2) = 5 +
 * 1 = 6
 * 3. find center of "width": centerOfWidth = 7 / 2 = 3
 * 4. now we can calculate start index of "source" to get "width" chars of
 * "source": begin: centerOfSearch - centerOfWidth = 6 - 3 = 3
 * 5. Result:
 *  "aa[begin+width]a bbb c[end]cc"
 * @param source
 * @param search
 * @param width
 * @param icase case sensetivity
 * @return
 */
TOOLBOX_API std::string substr_clip(
    const std::string& source,
    const std::string& search,
    size_t width,
    bool icase = false
);

/**
 * Concatenates strings by glue
 * @param glue
 * @param strings
 * @return
 */
TOOLBOX_API std::string glue(const std::string& glue, const std::vector<std::string>& strings);


TOOLBOX_API auto inline join_transformer = [](const auto& v) -> std::string { return v; };
/**
 * A bit more advanced function for concatenating strings
 * @param source any iterable type
 * @param prefix prefix for the result string
 * @param suffix suffix for the result string
 * @param delimiter delimiter between strings
 * @param truncated truncated string if limit reached. Limit for maximum items, not for total string length.
 * With limit 1 for example, string will look like: "prefix + first + delimiter + truncated + suffix"
 * <code>
 * std::vector<std::string> some {"hello", "world"}
 * join(some, ", ", "[", "]", "...", 1)
 * // result is: [hello, ...]]
 * </code>
 * @param transformer function to transform each element of the source into a type that has support for ostream
 * @return joined string
 */
template<typename Iterable, typename Transformer>
TOOLBOX_API std::string join(
    const Iterable& source,
    const std::string& delimiter,
    const std::string& prefix,
    const std::string& suffix,
    const std::string& truncated,
    size_t limit,
    const Transformer& transformer
) {
    static_assert(
        std::is_invocable_r_v<std::string, Transformer, typename Iterable::value_type>,
        "Transformer must be callable with Iterable::value_type and return std::string"
    );

    std::stringstream ss;
    ss << prefix;
    size_t i = 0;
    auto it = source.begin();
    while (it != source.end()) {
        if (i == limit) {
            ss << truncated;
            break;
        }

        ss << transformer(*it);
        ++it;

        if (it != source.end()) {
            ss << delimiter;
        }
        ++i;
    }

    ss << suffix;
    return ss.str();
}

template<typename Iterable>
TOOLBOX_API std::string join(
    const Iterable& source,
    const std::string& delimiter,
    const std::string& prefix,
    const std::string& suffix,
    const std::string& truncated,
    size_t limit
) {
    return join<Iterable>(source, delimiter, prefix, suffix, truncated, limit, join_transformer);
}

template<typename Iterable, typename Transformer>
TOOLBOX_API std::string join(
    const Iterable& source,
    const std::string& delimiter,
    const std::string& prefix,
    const std::string& suffix,
    const Transformer& transformer
) {
    return join<Iterable, Transformer>(source, delimiter, prefix, suffix, "...", SIZE_T_MAX, transformer);
}

template<typename Iterable>
TOOLBOX_API std::string join(
    const Iterable& source,
    const std::string& delimiter,
    const std::string& prefix,
    const std::string& suffix
) {
    return join<Iterable>(source, delimiter, prefix, suffix, "...", SIZE_T_MAX, join_transformer);
}

template<typename Iterable, typename Transformer>
TOOLBOX_API std::string join(
    const Iterable& source,
    const std::string& delimiter,
    const Transformer& transformer
) {
    return join<Iterable, Transformer>(source, delimiter, "", "", "...", SIZE_T_MAX, transformer);
}

template<typename Iterable>
TOOLBOX_API std::string join(
    const Iterable& source,
    const std::string& delimiter = ", "
) {
    return join<Iterable>(source, delimiter, "", "", "...", SIZE_T_MAX, join_transformer);
}

/**
 * Input file stream to string
 * @param inputStream
 * @return
 */
TOOLBOX_API std::string to_string(std::ifstream& inputStream);

/**
 * String to lower case
 * @param s
 * @return
 */
TOOLBOX_API std::string to_lower_case(const std::string& s);

/**
 * String to upper case
 * @param s
 * @return
 */
TOOLBOX_API std::string to_upper_case(const std::string& s);

/**
 *
 * @param s1
 * @param s2
 * @return
 */
TOOLBOX_API bool equals_icase(const std::string& s1, const std::string& s2);

/**
 * Check if string starts with prefix
 * @param source
 * @param prefix
 * @return
 */
TOOLBOX_API bool starts_with(const std::string& source, const std::string& prefix);

template<typename Iterable>
TOOLBOX_API bool starts_with_any(const std::string& source, const Iterable& prefixes) {
    return std::any_of(
        prefixes.begin(),
        prefixes.end(),
        [&source](const auto& prefix) {
            return starts_with(source, prefix);
        }
    );
}

/**
 * Check if string ends with suffix
 * @param source
 * @param suffix
 * @return
 */
TOOLBOX_API bool ends_with(const std::string& source, const std::string& suffix);

template<typename Iterable>
TOOLBOX_API bool ends_with_any(const std::string& source, const Iterable& suffixes) {
    return std::any_of(
        suffixes.begin(),
        suffixes.end(),
        [&source](const auto& suffix) {
            return ends_with(source, suffix);
        }
    );
}

/**
 * Remove prefix from string
 * @param source
 * @param prefix
 * @return
 */
TOOLBOX_API std::string remove_prefix(const std::string& source, const std::string& prefix);

/**
 * Remove right part of the string until `right_inclusive` ends
 * Example:
 * source = https://subdomain.google.com?param=1&another=2
 * right_inclusive = "?"
 * result = https://subdomain.google.com
 */
TOOLBOX_API std::string substring_before(const std::string& source, const std::string& delimiter);

/**
 * Remove left part of the string until `left_inclusive` ends
 * Example:
 * source = https://subdomain.google.com
 * left_inclusive = "." // remove everying until we wound first "." and return everything after it
 * result = google.com
 */
TOOLBOX_API std::string substring_after(const std::string& source, const std::string& delimiter);

/**
 * Remove substring before delimiter
 * @param source
 * @param delimiter
 * @return same string if nothing to remove, or copy of the [source] with removed values
 */
TOOLBOX_API std::string remove_substring_before(const std::string& source, const std::string& delimiter);

/**
 * Remove substring after delimiter
 * @param source
 * @param delimiter
 * @return same string if nothing to remove, or copy of the [source] with removed values
 */
TOOLBOX_API std::string remove_substring_after(const std::string& source, const std::string& delimiter);

/**
 * Remove suffix from string
 * @param source
 * @param suffix
 * @return
 */
TOOLBOX_API std::string remove_suffix(const std::string& source, const std::string& suffix);

/// \brief Alternative format, ability to pass dynamic buffer size
/// \tparam Args Args any type
/// \param buf_size size of result buffer
/// \param fmt
/// \param args
/// \return
template<typename... Args>
std::string format(size_t buf_size, const std::string& fmt, Args... args) {
    std::vector<char> tmp(buf_size);
    const int required = std::snprintf(tmp.data(), tmp.size(), fmt.c_str(), args...);
    if (required < 0) {
        throw std::runtime_error("Error during formatting.");
    }
    if (static_cast<size_t>(required) >= buf_size) {
        // Output was truncated, resize and try again
        tmp.resize(required + 1);
        std::snprintf(tmp.data(), tmp.size(), fmt.c_str(), args...);
    }
    return {tmp.data(), tmp.data() + required};
}

/**
 * Works like sprintf but with std::string and returns std::string
 * @tparam buf_size - ensure that buffer size enough for output result, default: buff size is 255
 * @tparam Args any type
 * @param fmt format string
 * @param args
 * @return
 */
template<size_t buf_size = 255, typename... Args>
std::string format(const std::string& fmt, Args... args) {
    return format(buf_size, fmt, args...);
}

} // namespace strings
} // namespace toolbox

#endif // TOOLBOXPP_STRINGS_H
