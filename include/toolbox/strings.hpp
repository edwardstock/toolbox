/// @file strings.hpp
/// @brief String manipulation utilities: split, join, replace, trim, format, etc.

#ifndef TOOLBOXPP_STRINGS_H
#define TOOLBOXPP_STRINGS_H

#include "toolbox/toolbox_config.h"

#include <algorithm>
#include <locale>
#include <sstream>
#include <string>
#include <utility>
#include <cstdio>
#include <cstddef>
#include <vector>
#include <type_traits>

namespace toolbox {
namespace strings {

/// String length type alias for std::string::size_type.
using strlen_t = std::string::size_type;

/// Functor that compares characters in a case-insensitive manner using the given locale.
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

/// Perform a case-insensitive search for str2 inside str1.
/// @return position of the first match, or std::string::npos if not found
template<typename T>
size_t compare(const T& str1, const T& str2, const std::locale& loc = std::locale()) {
    typename T::const_iterator it =
        std::search(str1.begin(), str1.end(), str2.begin(), str2.end(), string_equaling<typename T::value_type>(loc));
    if (it != str1.end())
        return it - str1.begin();

    return std::string::npos; // not found
}

/// Check whether the source string contains the substring (case-insensitive).
/// @param substring text to search for
/// @param source text to search in
/// @return true if the substring is found
TOOLBOX_API bool has_substring_icase(const std::string& substring, const std::string& source);

/// Check whether the source string contains the substring (case-sensitive).
/// @param substring text to search for
/// @param source text to search in
/// @return true if the substring is found
TOOLBOX_API bool has_substring(const std::string& substring, const std::string& source);

/// Check whether the source string contains the given character.
/// @param substring character to search for
/// @param source text to search in
/// @return true if the character is found
TOOLBOX_API bool has_substring(char substring, const std::string& source);

/// Split a string by a string delimiter.
/// @param source text to split
/// @param delimiter separator string
/// @return vector of substrings (empty entries are preserved)
TOOLBOX_API std::vector<std::string> split(const std::string& source, const std::string& delimiter);

/// Split a string by a single-character delimiter.
/// @param source text to split
/// @param delimiter separator character
/// @return vector of substrings (empty entries are preserved)
TOOLBOX_API std::vector<std::string> split(const std::string& source, const char& delimiter);

/// Split a string into chunks of at most @p max characters.
/// @param src source string
/// @param max maximum length of each chunk
/// @return vector of chunks; if src.size() <= max, the vector contains one element
TOOLBOX_API std::vector<std::string> split_by_len(const std::string& src, size_t max);

/// Split a string into exactly two parts at the first occurrence of the delimiter.
/// @param source text to split
/// @param delimiter separator character
/// @return pair of two strings; both empty if the delimiter is not found
TOOLBOX_API std::pair<std::string, std::string> split_pair(const std::string& source, const char& delimiter);

/// Split a string into exactly two parts at the first occurrence of the delimiter.
/// @param source text to split
/// @param delimiter separator string
/// @return pair of two strings; both empty if the delimiter is not found
TOOLBOX_API std::pair<std::string, std::string> split_pair(const std::string& source, const std::string& delimiter);

/// Repeat the input string @p n times.
/// @param in input string
/// @param n repetition count
/// @return concatenated result
TOOLBOX_API std::string repeat(const std::string& in, size_t n = 1);

/// Repeat the input character @p n times.
/// @param in input character
/// @param n repetition count
/// @return resulting string
TOOLBOX_API std::string repeat(char in, size_t n = 1);

/// Remove leading and trailing whitespace (spaces, tabs, newlines, etc.).
/// @param in input string
/// @return trimmed copy
TOOLBOX_API std::string trim(const std::string& in);

/// Remove leading and trailing whitespace from the string in place.
/// @param in string to trim (modified in place)
TOOLBOX_API void trim_ref(std::string& in);

/// Replace the first occurrence of @p search with @p replace in @p source.
/// Modifies @p source in place.
/// @param search substring to find
/// @param replace replacement text
/// @param source string to modify
TOOLBOX_API void replace(
    const std::string& search,
    const std::string& replace,
    std::string& source
);

/// Replace all occurrences of @p search with @p replace in @p source.
/// Modifies @p source in place.
/// @param search substring to find
/// @param replace replacement text
/// @param source string to modify
TOOLBOX_API void substr_replace_all(
    const std::string& search,
    const std::string& replace,
    std::string& source
);

/// Replace all occurrences of each search string with the corresponding replace string.
/// Modifies @p source in place.  Both vectors must have the same size.
/// @param search substrings to find
/// @param replace replacement texts
/// @param source string to modify
TOOLBOX_API void substr_replace_all(
    const std::vector<std::string>& search,
    const std::vector<std::string>& replace,
    std::string& source
);

/// Replace all occurrences of every search string with a single replace string.
/// Modifies @p source in place.
/// @param search substrings to find
/// @param replace common replacement text
/// @param source string to modify
TOOLBOX_API void substr_replace_all(
    const std::vector<std::string>& search,
    const std::string& replace,
    std::string& source
);

/// Replace the first occurrence of @p search with @p replace and return a copy.
/// @param search substring to find
/// @param replace replacement text
/// @param source original string (not modified)
/// @return modified copy, or the same string if @p search is not found
TOOLBOX_API std::string substr_replace_ret(
    const std::string& search,
    const std::string& replace,
    const std::string& source
);

/// Replace all occurrences of @p search with @p replace and return a copy.
/// @param search substring to find
/// @param replace replacement text
/// @param source original string (not modified)
/// @return modified copy, or the same string if @p search is not found
TOOLBOX_API std::string substr_replace_all_ret(
    const std::string& search,
    const std::string& replace,
    const std::string& source
);

/// Replace all occurrences of each search string with the corresponding replace
/// string and return a copy.
///
/// Example:
/// @code
/// substr_replace_all_ret(
///     {"before1", "before2"},
///     {"after1",  "after2"},
///     "Source string with before1 and before2"
/// );
/// @endcode
/// @param search substrings to find
/// @param replace replacement texts (same size as search)
/// @param source original string (not modified)
/// @return modified copy, or the same string if nothing matched
TOOLBOX_API std::string substr_replace_all_ret(
    const std::vector<std::string>& search,
    const std::vector<std::string>& replace,
    const std::string& source
);

/// Replace all occurrences of every search string with a single replace string
/// and return a copy.
/// @param search substrings to find
/// @param replace common replacement text
/// @param source original string (not modified)
/// @return modified copy, or the same string if nothing matched
TOOLBOX_API std::string substr_replace_all_ret(
    const std::vector<std::string>& search,
    const std::string& replace,
    const std::string& source
);

/// Remove the first occurrence of @p removable from @p source.
/// Modifies @p source in place.
/// @param source string to modify
/// @param removable substring to remove
TOOLBOX_API void substr_remove(std::string& source, const std::string& removable);

/// Remove all occurrences of every string in @p removables from @p source.
/// Modifies @p source in place.
/// @param source string to modify
/// @param removables substrings to remove
TOOLBOX_API void substr_remove_all(std::string& source, std::vector<std::string> removables);

/// Return the substring before the given character.
/// If the character is bitwise-inverted (~'c'), the substring after it is
/// returned instead.
///
/// Example:
/// @code
/// substr_inverse("abcdef", 'c');   // "ab"
/// substr_inverse("abcdef", ~'c');  // "def"
/// @endcode
/// @param source input string
/// @param whence pivot character (invert with ~ to cut from the end)
/// @return extracted substring
TOOLBOX_API std::string substr_inverse(const std::string& source, char whence);

/// Return the substring between @p begin and @p end delimiters.
/// @param source input string
/// @param begin opening delimiter string
/// @param end closing delimiter string
/// @param offset starting search offset
/// @return extracted substring
TOOLBOX_API std::string substr_inverse(
    const std::string& source,
    const std::string& begin,
    const std::string& end,
    long offset = 0
);

/// Return the substring between @p begin and @p end delimiter characters.
/// @param source input string
/// @param begin opening delimiter character
/// @param end closing delimiter character
/// @param offset starting search offset
/// @return extracted substring
TOOLBOX_API std::string substr_inverse(const std::string& source, char begin, char end, long offset = 0);

/// Return the substring starting from the @p begin delimiter to the end.
/// @param source input string
/// @param begin opening delimiter string
/// @return extracted substring
TOOLBOX_API std::string substr_inverse(const std::string& source, const std::string& begin);

/// Clip a region of @p width characters around the first occurrence of @p search.
///
/// The search term is centered within the clipped window.
///
/// Example for source="aaa bbb ccc", search="bbb", width=7:
///   result = "a bbb c"
///
/// @param source input string
/// @param search term to center on
/// @param width desired output width
/// @param icase true for case-insensitive search
/// @return clipped substring, or empty string if @p search is not found
TOOLBOX_API std::string substr_clip(
    const std::string& source,
    const std::string& search,
    size_t width,
    bool icase = false
);

/// Concatenate strings with a glue separator.
/// @param glue separator inserted between elements
/// @param strings elements to join
/// @return joined string
TOOLBOX_API std::string glue(const std::string& glue, const std::vector<std::string>& strings);


/// Default transformer used by join() overloads: identity for string-like types.
auto inline join_transformer = [](const auto& v) -> std::string { return v; };

/// Join elements of an iterable container into a single string.
///
/// @tparam Iterable container type (must have begin()/end() and value_type)
/// @tparam Transformer callable that converts each element to std::string
/// @param source container to iterate
/// @param delimiter separator between elements
/// @param prefix prepended before the first element
/// @param suffix appended after the last element
/// @param truncated text appended when the limit is reached
/// @param limit maximum number of elements to include (not string length)
/// @param transformer function applied to each element before appending
/// @return joined string
template<typename Iterable, typename Transformer>
std::string join(
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
std::string join(
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
std::string join(
    const Iterable& source,
    const std::string& delimiter,
    const std::string& prefix,
    const std::string& suffix,
    const Transformer& transformer
) {
    return join<Iterable, Transformer>(source, delimiter, prefix, suffix, "...", SIZE_MAX, transformer);
}

template<typename Iterable>
std::string join(
    const Iterable& source,
    const std::string& delimiter,
    const std::string& prefix,
    const std::string& suffix
) {
    return join<Iterable>(source, delimiter, prefix, suffix, "...", SIZE_MAX, join_transformer);
}

template<typename Iterable, typename Transformer>
std::string join(
    const Iterable& source,
    const std::string& delimiter,
    const Transformer& transformer
) {
    return join<Iterable, Transformer>(source, delimiter, "", "", "...", SIZE_MAX, transformer);
}

template<typename Iterable>
std::string join(
    const Iterable& source,
    const std::string& delimiter = ", "
) {
    return join<Iterable>(source, delimiter, "", "", "...", SIZE_MAX, join_transformer);
}

/// Read the entire contents of an input file stream into a string.
/// @param inputStream open file stream
/// @return file contents as a string
TOOLBOX_API std::string to_string(std::ifstream& inputStream);

/// Convert all characters to lower case (classic "C" locale).
/// @param s input string
/// @return lower-cased copy
TOOLBOX_API std::string to_lower_case(const std::string& s);

/// Convert all characters to upper case (classic "C" locale).
/// @param s input string
/// @return upper-cased copy
TOOLBOX_API std::string to_upper_case(const std::string& s);

/// Compare two strings for equality, ignoring case.
/// @param s1 first string
/// @param s2 second string
/// @return true if the strings are equal (case-insensitive)
TOOLBOX_API bool equals_icase(const std::string& s1, const std::string& s2);

/// Check whether @p source starts with @p prefix.
/// @param source string to inspect
/// @param prefix expected prefix
/// @return true if @p source begins with @p prefix
TOOLBOX_API bool starts_with(const std::string& source, const std::string& prefix);

/// Check whether @p source starts with any of the given prefixes.
/// @tparam Iterable container of strings
/// @param source string to inspect
/// @param prefixes candidate prefixes
/// @return true if at least one prefix matches
template<typename Iterable>
bool starts_with_any(const std::string& source, const Iterable& prefixes) {
    return std::any_of(
        prefixes.begin(),
        prefixes.end(),
        [&source](const auto& prefix) {
            return starts_with(source, prefix);
        }
    );
}

/// Check whether @p source ends with @p suffix.
/// @param source string to inspect
/// @param suffix expected suffix
/// @return true if @p source ends with @p suffix
TOOLBOX_API bool ends_with(const std::string& source, const std::string& suffix);

/// Check whether @p source ends with any of the given suffixes.
/// @tparam Iterable container of strings
/// @param source string to inspect
/// @param suffixes candidate suffixes
/// @return true if at least one suffix matches
template<typename Iterable>
bool ends_with_any(const std::string& source, const Iterable& suffixes) {
    return std::any_of(
        suffixes.begin(),
        suffixes.end(),
        [&source](const auto& suffix) {
            return ends_with(source, suffix);
        }
    );
}

/// Remove @p prefix from the beginning of @p source and return a copy.
/// Returns @p source unchanged if the prefix is not present.
/// @param source input string
/// @param prefix prefix to strip
/// @return string without the prefix
TOOLBOX_API std::string remove_prefix(const std::string& source, const std::string& prefix);

/// Return the part of @p source before the first occurrence of @p delimiter.
///
/// Example: substring_before("host?q=1", "?") returns "host".
/// @param source input string
/// @param delimiter boundary string
/// @return substring before the delimiter, or the full string if not found
TOOLBOX_API std::string substring_before(const std::string& source, const std::string& delimiter);

/// Return the part of @p source after the first occurrence of @p delimiter.
///
/// Example: substring_after("sub.google.com", ".") returns "google.com".
/// @param source input string
/// @param delimiter boundary string
/// @return substring after the delimiter, or the full string if not found
TOOLBOX_API std::string substring_after(const std::string& source, const std::string& delimiter);

/// Remove everything before (and including) the first occurrence of @p delimiter.
/// @param source input string
/// @param delimiter boundary string
/// @return remaining substring, or the full string if delimiter is not found
TOOLBOX_API std::string remove_substring_before(const std::string& source, const std::string& delimiter);

/// Remove everything after (and including) the first occurrence of @p delimiter.
/// @param source input string
/// @param delimiter boundary string
/// @return remaining substring, or the full string if delimiter is not found
TOOLBOX_API std::string remove_substring_after(const std::string& source, const std::string& delimiter);

/// Remove @p suffix from the end of @p source and return a copy.
/// Returns @p source unchanged if the suffix is not present.
/// @param source input string
/// @param suffix suffix to strip
/// @return string without the suffix
TOOLBOX_API std::string remove_suffix(const std::string& source, const std::string& suffix);

/// Format a string using printf-style syntax with a caller-specified buffer size.
/// If the output exceeds @p buf_size, the buffer is automatically resized.
/// @tparam Args argument types
/// @param buf_size initial buffer size in bytes
/// @param fmt printf-style format string
/// @param args format arguments
/// @return formatted string
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

/// Format a string using printf-style syntax.
/// @tparam buf_size initial buffer size in bytes (default: 255)
/// @tparam Args argument types
/// @param fmt printf-style format string
/// @param args format arguments
/// @return formatted string
template<size_t buf_size = 255, typename... Args>
std::string format(const std::string& fmt, Args... args) {
    return format(buf_size, fmt, args...);
}

} // namespace strings
} // namespace toolbox

#endif // TOOLBOXPP_STRINGS_H
