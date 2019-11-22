/*!
 * toolboxpp. 2017
 *
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_HPP
#define TOOLBOXPP_HPP

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
#define TOOLBOXPP_WIN32 1
#endif

#ifdef _MSC_VER
#include <tchar.h>
#include <ctype.h>
#define __CHAR_TO_LOWER(c) _totlower(c)
#define __CHAR_TO_UPPER(c) _totupper(c)

#ifdef TOOLBOX_ENABLE_MB
#include <wchar.h>
#include <clocale>
#define __WCHAR_TO_LOWER(c) _towlower_l(c, _get_current_locale())
#define __WCHAR_TO_UPPER(c) _towupper_l(c, _get_current_locale())
#endif // TOOLBOX_ENABLE_MB
#else // other
#define __CHAR_TO_LOWER(c) std::tolower(c)
#define __WCHAR_TO_LOWER(c) std::towlower(c)
#define __CHAR_TO_UPPER(c) std::toupper(c)
#define __WCHAR_TO_UPPER(c) std::towupper(c)
#endif

#include <atomic>
#include <chrono>
#include <string>
#include <iosfwd>
#include <vector>
#include <map>
#include <deque>
#include <regex>
#include <queue>
#include <unordered_map>
#include <istream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <locale>
#include <sys/stat.h>
#include <cassert>

#ifndef TOOLBOXPP_WIN32
#include <termios.h>
#endif

#ifndef _MSC_VER
#include <unistd.h>  /* for EINTR */
#endif

#include <errno.h>   /* for errno */
#include <stack>

#ifndef MS_STDLIB_BUGS
#  if (_MSC_VER || __MINGW32__ || __MSVCRT__)
#    define MS_STDLIB_BUGS 1
#  else
#    define MS_STDLIB_BUGS 0
#  endif
#endif

#if MS_STDLIB_BUGS
#  include <io.h>
#  include <fcntl.h>
#endif

#include <locale>
#ifndef INIT_WCHAR_UNICODE
#if MS_STDLIB_BUGS
#define INIT_WCHAR_UNICODE() \
constexpr char cp_utf16le[] = ".1200"; \
setlocale( LC_ALL, cp_utf16le ); \
_setmode( _fileno(stdout), _O_WTEXT )
#else
#define INIT_WCHAR_UNICODE() \
  setlocale( LC_ALL, "" ); \
  std::locale::global(std::locale("")); \
  std::wcin.imbue(std::locale()); \
  std::wcout.imbue(std::locale())
#endif // if MS_STDLIB_BUGS
#endif // ifndef INIT_WCHAR_UNICODE

#define L_LEVEL(level) toolboxpp::Logger::get().setLevel(level)

#define L_DEBUG(tag, message) toolboxpp::Logger::get().debug(__FILE__, __LINE__, tag, message);
#define L_DEBUG_F(tag, message, ...) \
    { \
        char __tmp_buff[512]; \
        sprintf(__tmp_buff, message, __VA_ARGS__); \
        toolboxpp::Logger::get().debug(__FILE__, __LINE__, tag, __tmp_buff); \
    }

#define L_INFO(tag, message) toolboxpp::Logger::get().info(__FILE__, __LINE__, tag, message);
#define L_INFO_F(tag, message, ...) \
    { \
        char __tmp_buff[512]; \
        sprintf(__tmp_buff, message, __VA_ARGS__); \
        toolboxpp::Logger::get().info(__FILE__, __LINE__, tag, __tmp_buff); \
    }

#define L_WARN(tag, message) toolboxpp::Logger::get().warning(__FILE__, __LINE__, tag, message);
#define L_WARN_F(tag, message, ...) \
    { \
        char __tmp_buff[512]; \
        sprintf(__tmp_buff, message, __VA_ARGS__); \
        toolboxpp::Logger::get().warning(__FILE__, __LINE__, tag, __tmp_buff); \
    }
#define L_ERR(tag, message) toolboxpp::Logger::get().error(__FILE__, __LINE__, tag, message);
#define L_ERR_F(tag, message, ...) \
    { \
        char __tmp_buff[512]; \
        sprintf(__tmp_buff, message, __VA_ARGS__); \
        toolboxpp::Logger::get().error(__FILE__, __LINE__, tag, __tmp_buff); \
    }
#define L_CRIT(tag, message) toolboxpp::Logger::get().critical(__FILE__, __LINE__, tag, message);
#define L_CRIT_F(tag, message, ...) \
    { \
        char __tmp_buff[512]; \
        sprintf(__tmp_buff, message, __VA_ARGS__); \
        toolboxpp::Logger::get().critical(__FILE__, __LINE__, tag, __tmp_buff); \
    }

#ifdef _MSC_VER
#define L_TRAP(tag, message) \
    L_CRIT(tag, message) \
    __debugbreak()
#else
#include <csignal>
#define L_TRAP(tag, message) \
    L_CRIT(tag, message); \
    raise(SIGABRT)
#endif

#define L_FLUSH() toolboxpp::Logger::get().flush()

namespace toolboxpp {

namespace collection {

template<typename T>
struct is_map {
  static const bool value = false;
};
template<typename K, typename V>
struct is_map<std::map<K, V>> {
  static const bool value = true;
};
template<typename K, typename V>
struct is_map<std::unordered_map<K, V>> {
  static const bool value = true;
};
template<typename K, typename V>
struct is_map<std::unordered_multimap<K, V>> {
  static const bool value = true;
};
template<typename K, typename V>
struct is_map<std::multimap<K, V>> {
  static const bool value = true;
};

template<typename map_type, typename key_type = typename map_type::key_type>
std::vector<key_type> mapKeysToVector(const map_type &map) {
    static_assert(is_map<map_type>::value, "Only map can be passed");
    std::vector<key_type> v(map.size());
    size_t i = 0;
    for (auto &it: map) {
        v[i] = it.first;
        i++;
    }

    return v;
}

template<typename map_type, typename value_type = typename map_type::value_type::second_type>
std::vector<value_type> mapValuesToVector(const map_type &map) {
    static_assert(is_map<map_type>::value, "Only map can be passed");
    std::vector<value_type> v(map.size());
    size_t i = 0;
    for (auto &it: map) {
        v[i] = it.second;
        i++;
    }

    return v;
}
}

namespace strings {
using strlen_t = std::string::size_type;

template<typename _CharT>
struct StringEqualing {
    explicit StringEqualing(const std::locale &locale) :
        _locale(locale) { }
    bool operator()(_CharT ch1, _CharT ch2) {
        return std::toupper(ch1, _locale) == std::toupper(ch2, _locale);
    }
private:
    const std::locale &_locale;
};

template<typename T>
size_t stringCompare(const T &str1, const T &str2, const std::locale &loc = std::locale()) {
    typename T::const_iterator it = std::search(str1.begin(), str1.end(),
                                                str2.begin(), str2.end(),
                                                StringEqualing<typename T::value_type>(loc)
    );
    if (it != str1.end()) return it - str1.begin();

    return std::string::npos; // not found
}

/**
 *
 * @param source
 * @param substring
 * @return
 */
inline bool hasSubstringIgnoreCase(const std::string &substring, const std::string &source) {
    size_t cmp = stringCompare(source, substring);
    return cmp != std::string::npos;
}

/**
 * Search substring in string
 * @param source
 * @param substring
 * @return
 */
inline bool hasSubstring(const std::string &substring, const std::string &source) {
    return source.find(substring) != std::string::npos;
}
/**
 * Serch character in string
 * @param source
 * @param substring
 * @return
 */
inline bool hasSubstring(char substring, const std::string &source) {
    return source.find(substring) != std::string::npos;
}

#ifdef TOOLBOX_ENABLE_MB
/**
 * Serch character in string
 * @param source
 * @param substring
 * @return
 */
inline bool hasWSubstring(wchar_t substring, const std::wstring &source) {
    return source.find(substring) != std::string::npos;
}
#endif

/**
 * Match regex pattern
 * @param pattern
 * @param source
 * @return
 */
inline bool hasRegex(const std::regex &pattern, const std::string &source) {
    std::smatch match;
    return std::regex_search(source, match, pattern);
}
/**
 * Match regex pattern.
 * @param pattern Just string. By default: search with flag std::regex_constants::icase
 * @param source
 * @return
 */
inline bool hasRegex(const std::string &pattern, const std::string &source) {
    return hasRegex(std::regex(pattern, std::regex_constants::icase), source);
}
/**
 * Returns all found groups in source with pattern
 * @param rx custom regex object
 * @param s
 * @return
 */
inline std::vector<std::vector<std::string>> matchAllRegexp(const std::regex &pattern, const std::string &source) {
    std::vector<std::vector<std::string>> capturedGroups;
    std::vector<std::string> capturedSubgroups;
    const std::sregex_token_iterator endIterator;
    for (std::sregex_token_iterator it(source.cbegin(), source.cend(), pattern); it != endIterator; ++it) {
        capturedSubgroups.clear();
        std::string group = *it;
        std::smatch res;
        if (std::regex_search(group, res, pattern)) {
            for (size_t i = 0; i < res.size();
                 i++) { // NOLINT(modernize-loop-convert), sometimes foreach has strange effect - size is 0, but iterator have != 0 items
                capturedSubgroups.push_back(res[i]);
            }

            if (!capturedSubgroups.empty()) {
                capturedGroups.push_back(capturedSubgroups);
            }
        }
    }

    return capturedGroups;
}
/**
 * Returns all found groups in source with pattern
 * @param pattern
 * @param source
 * @return
 */
inline std::vector<std::vector<std::string>> matchAllRegexp(const std::string &pattern, const std::string &source) {
    return matchAllRegexp(std::regex(pattern, std::regex_constants::icase), source);
}
/**
 * Take first regex match and return it
 * @param pattern
 * @param source
 * @return
 */
inline std::string matchRegexpFirst(const std::regex &pattern, const std::string &source) {
    std::smatch results;
    std::string result;
    bool found = std::regex_search(source, results, pattern);
    if (!found || results.size() < 2) {
        return std::string();
    }

    return results[1];
}
/**
 * Take first regex match and return it (with custom regex object)
 * @param pattern
 * @param source
 * @return
 */
inline std::string matchRegexpFirst(const std::string &pattern, const std::string &source) {
    return matchRegexpFirst(std::regex(pattern, std::regex_constants::icase), source);
}

/**
 * Less boilerplate for std::regex
 * Attention! Don't use this method to check if regex pattern has in string, use {@code hasRegex()} instead,
 * because different compilers compiles different results
 * @param rxPattern
 * @param source
 * @return
 */
inline const std::vector<std::string> matchRegexp(const std::regex &rxPattern, const std::string &source) {
    std::smatch result;
    std::regex_search(source, result, rxPattern);

    std::vector<std::string> out(result.size());
    const size_t cnt = result.size();
    for (size_t i = 0; i < cnt; i++) {
        out[i] = result[i];
    }

    return out;
}

/**
 * Less boilerplate for std::regex
 * Attention! Don't use this method to check if regex pattern has in string, use {@code hasRegex()} instead,
 * because different compilers compiles different results
 * @param pattern
 * @param source
 * @return
 */
inline const std::vector<std::string> matchRegexp(const std::string &pattern, const std::string &source) {
    return matchRegexp(std::regex(pattern, std::regex_constants::icase), source);
}


/**
 * Splits string by char delimiter to vector list
 * @param source
 * @param delimiter
 * @return
 */
inline std::vector<std::string> split(const std::string &source, const std::string &delimiter) {
    if (delimiter.empty()) {
        return std::vector<std::string>(0);
    }

    std::string src = source;

    std::vector<std::string> result;
    size_t current = 0;
    while (current != src.npos) {
        current = src.find(delimiter);
        if (current != src.npos && src.substr(current, delimiter.length()) == delimiter) {
            result.push_back(src.substr(0, current));
            src = src.substr(current + (delimiter.length()), src.npos);
        }
    }
    if (src.length() > 0) {
        result.push_back(src);
    }

    return result;
}
/**
 * Splits string by delimiter to vector list
 * @param source
 * @param delimiter
 * @return
 */
inline std::vector<std::string> split(const std::string &source, const char &delimiter) {
    std::stringstream ss;
    ss.str(source);
    std::string item;
    std::vector<std::string> elements;
    while (std::getline(ss, item, delimiter)) {
        elements.push_back(item);
    }

    return elements;
}

/**
 * \brief Split given string by max length. Example: abc, max len: 2, output will: std::vector<std::string>{"ab", "c"}
 * \param src source string
 * \param max max length
 * \return separated string to vector. If src length less than maximum, vector will contain 1 element with src
 */
inline std::vector<std::string> splitByLength(const std::string &src, size_t max) {
    std::vector<std::string> out;
    if (src.length() > max) {
        size_t got = 0;
        for (size_t i = 0; i < src.length(); i += max) {
            size_t n;
            if (got + max > src.length()) {
                n = src.length() - got;
            } else {
                n = got + max;
                got = n;
            }
            auto part = src.substr(i, n);

            out.push_back(part);
        }
    } else {
        out.push_back(src);
    }

    return out;
}

/**
 * Splits string by char delimiter to pair
 * @param source
 * @param delimiter
 * @return
 */
inline std::pair<std::string, std::string> splitPair(const std::string &source, const char &delimiter) {
    std::vector<std::string> elements = split(source, delimiter);

    if (elements.empty()) {
        return std::pair<std::string, std::string>(std::string(), std::string());
    }

    if (elements.size() == 1) {
        return std::pair<std::string, std::string>(elements.at(0), std::string());
    }

    return std::pair<std::string, std::string>(elements.at(0), elements.at(1));
}

inline std::pair<std::string, std::string> splitPair(const std::string &source, const std::string &delimiter) {
    if (delimiter.empty()) {
        return std::pair<std::string, std::string>(std::string(), std::string());
    }
    return splitPair(source, delimiter.c_str()[0]);
}

/**
 * Replaces first occurrence "search" and replaces with "replace" parameter in "source" string.
 * @param search
 * @param replace
 * @param source Immutable
 * @return If search string were not found, function will return source string.
 */
inline std::string substringReplace(const std::string &search,
                                    const std::string &replace,
                                    const std::string &source) {
    if (source.empty() || source.length() < search.length()) {
        return source;
    }

    size_t found = source.find(search);
    if (found == std::string::npos) {
        return source;
    }

    std::string begin, end;
    begin = source.substr(0, found);
    end = source.substr(found + search.length(), std::string::npos);
    std::stringstream ss;
    ss << begin << replace << end;

    return ss.str();
}

/**
 * Works like std::string substringReplace(const std::string &search, const std::string &replace, const std::string &source);
 * but replaces all "search" occurrences with "replace" string
 * @param search
 * @param replace
 * @param source
 * @return
 */
inline std::string substringReplaceAll(const std::string &search,
                                       const std::string &replace,
                                       const std::string &source) {
    std::string result = substringReplace(search, replace, source);
    while (result.find(search) != std::string::npos) {
        result = substringReplace(search, replace, result);
    }

    return result;
}

/**
 * Replaces list search strings with replace list. Vectors size must be equals!
 * Example:
 *
 * substringReplaceAll({"before1","before2"}, {"after1","after2"}, "Source string with before1 and before2")
 *
 *
 * @param search
 * @param replace
 * @param source
 * @return
 */
inline std::string substringReplaceAll(const std::vector<std::string> &search,
                                       const std::vector<std::string> &replace,
                                       const std::string &source) {
    std::vector<std::string> toReplace;
    if (replace.size() != search.size() && replace.size() == 1) {
        for (std::size_t i = 0; i < search.size(); i++) {
            toReplace.push_back(replace[0]);
        }
    } else if ((search.size() != replace.size() && replace.size() > 1) || search.empty() || replace.empty()) {
        throw std::invalid_argument("search & replace vectors must be equal size and not empty!");
    } else {
        toReplace = replace;
    }

    std::string result;

    for (size_t i = 0; i < search.size(); i++) {
        result = substringReplaceAll(search[i], toReplace[i], i == 0 ? source : result);
    }

    return result;
}

/**
 * \brief Repeat input string N times
 * \param in input string
 * \param n number of times to repeat
 * \return repeated string
 */
inline std::string repeat(const std::string &in, size_t n = 1) {
    std::stringstream ss;
    for (size_t i = 0; i < n; i++) {
        ss << in;
    }

    return ss.str();
}

/*!
 * \brief Repeat input char N times
 * \param in input char
 * \param n number of times to repeat
 * \return repeated string
 */
inline std::string repeat(char in, size_t n = 1) {
    std::stringstream ss;
    for (size_t i = 0; i < n; i++) {
        ss << in;
    }

    return ss.str();
}

/*!
 * @see substringReplace(const std::string &search,
									const std::string &replace,
									const std::string &source) but mutable behavior
 * @param search
 * @param replace
 * @param source Mutable
 * @return
 */
inline void replace(const std::string &search, const std::string &replace, std::string &source) {
    source = substringReplace(search, replace, source);
}

/**
 * @see substringReplaceAll(const std::string &search,
									   const std::string &replace,
									   const std::string &source)
 * @param search
 * @param replace
 * @param source Mutable
 */
inline void replaceAll(const std::string &search, const std::string &replace, std::string &source) {
    source = substringReplaceAll(search, replace, source);
}

/*!
 *
 * @param search Search vector
 * @param replace Replace vector
 * @param source mutable string
 */
inline void replaceAll(const std::vector<std::string> &search,
                       const std::vector<std::string> &replace,
                       std::string &source) {
    source = substringReplaceAll(search, replace, source);
}

/**
 * Find "removable" string in "source" and remove it
 * @param source Mutable value!
 * @param removable
 */
inline void removeSubstrings(std::string &source, const std::string &removable) {
    size_t n = removable.length();

    for (size_t i = source.find(removable); i != std::string::npos; i = source.find(removable)) {
        source.erase(i, n);
    }
}
/**
 * Find all "removables" strings in "source" and remove it
 * @param source
 * @param removables
 */
inline void removeSubstrings(std::string &source, std::vector<std::string> removables) {
    for (auto &replace : removables) {
        removeSubstrings(source, replace);
    }
}

/**
 * Getting substring from specified character
 * @param source
 * @param whence Whence start to cut. You can inverse character to cut from the end of "source"
 * Example:
 *  from start:
 *      string s = "abcdef";
		Strings::substrInverse(s, 'c');
		result: "ab"

	from end:
		string s = "abcdef";
		Strings::substrInverse(s, ~'c');
		result: "def"
 * @return
 */
inline std::string substrInverse(const std::string &source, char whence) {
    strlen_t from;
    std::string out;
    if (~whence >= 0) {
        from = source.rfind(~whence);
        out = source.substr(from + 1, std::string::npos);
    } else {
        from = source.find(whence);
        out = source.substr(0, from);
    }

    return out;
}
inline std::string substrInverse(const std::string &source,
                                 const std::string &begin,
                                 const std::string &end,
                                 long offset = 0) {
    strlen_t to, from = source.find(begin);
    if (from == std::string::npos) {
        from = 0;
    }
    to = source.rfind(end);

    from += -(offset);
    to += 1 + offset;
    std::string out = source.substr(0, from);
    out.append(source.substr(to, std::string::npos));
    return out;
}

inline std::string substrInverse(const std::string &source, char begin, char end, long offset = 0) {
    return substrInverse(source, std::string(1, begin), std::string(1, end), offset);
}
inline std::string substrInverse(const std::string &source, const std::string &begin) {
    strlen_t from = source.find(begin);
    if (from == std::string::npos) {
        from = 0;
    }
    return source.substr(0, from);
}

/**
 * How works:
 * source: "aaa bbb ccc"
 * search: "bbb"
 * width: 7
 * 1. find position of "search" in "source" = 5
 * 2. find center of "search": centerOfSearch = 5 + (search.length() / 2) = 5 + 1 = 6
 * 3. find center of "width": centerOfWidth = 7 / 2 = 3
 * 4. now we can calculate start index of "source" to get "width" chars of "source":
 *  begin: centerOfSearch - centerOfWidth = 6 - 3 = 3
 * 5. Result:
 *  "aa[begin+width]a bbb c[end]cc"
 * @param source
 * @param search
 * @param width
 * @param icase case sensetivity
 * @return
 */
inline std::string clipSubstring(const std::string &source,
                                 const std::string &search,
                                 size_t width,
                                 bool icase = false) {
    size_t pos = icase ? stringCompare(source, search) : source.find(search);
    if (pos == std::string::npos) {
        return source;
    }

    size_t searchLength = search.length();
    size_t sourceLength = source.length();

    // if width equals search length, return search string
    if (searchLength == width || sourceLength == searchLength || sourceLength == width) {
        return search;
    }

    // if width size less than search size, return part of search string
    if (width < searchLength || sourceLength < width) {
        return search.substr(0, width);
    }

    long centerOfSearch, centerOfWidth, begin;

    centerOfSearch = pos + (searchLength / 2);
    centerOfWidth = width / 2;

    if (centerOfSearch - centerOfWidth < 0) {
        begin = 0;
    } else {
        begin = centerOfSearch - centerOfWidth;
    }

    return source.substr((size_t) begin, width);
}

/**
 * Concatenates strings by glue
 * @param glue
 * @param strings
 * @return
 */
inline std::string glue(const std::string &glue, const std::vector<std::string> &strings) {
    std::string out;

    size_t size = strings.size();
    size_t i = 0;
    for (auto &s: strings) {
        if (i == 0 || i == size) {
            out.append(s);
        } else {
            out.append(glue).append(s);
        }

        i++;
    }

    return out;
}

/**
 * Input file stream to string
 * @param inputStream
 * @return
 */
inline std::string toString(std::ifstream &inputStream) {
    return std::string((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
}

/**
 * String to lower case
 * @param s
 * @return
 */
inline std::string toLower(const std::string &s) {
    std::stringstream tmp;
    for (char i : s) {
        tmp << static_cast<char>(__CHAR_TO_LOWER(i));
    }

    return tmp.str();
}

#ifdef TOOLBOX_ENABLE_MB
/**
 * String to lower case (wide char)
 * @param s
 * @return
 */
inline std::wstring toWLower(const std::wstring &s) {
    std::wstringstream tmp;
    for (wchar_t i : s) {
        tmp << static_cast<wchar_t>(__WCHAR_TO_LOWER(i));
    }

    return tmp.str();
}
#endif

/**
 * String to upper case
 * @param s
 * @return
 */
inline std::string toUpper(const std::string &s) {
    std::stringstream tmp;
    for (char i : s) {
        tmp << static_cast<char>(__CHAR_TO_UPPER(i));
    }

    return tmp.str();
}

#ifdef TOOLBOX_ENABLE_MB
/**
 * String to upper case
 * @param s
 * @return
 */
inline std::wstring toWUpper(const std::wstring &s) {
    std::wstringstream tmp;
    for (wchar_t i : s) {
        tmp << static_cast<wchar_t>(__WCHAR_TO_UPPER(i));
    }

    return tmp.str();
}
#endif

/**
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool equalsIgnoreCase(const std::string &s1, const std::string &s2) {
    if (s1.length() != s2.length()) return false;

    return std::equal(
        s1.begin(), s1.end(),
        s2.begin(),
        [](unsigned char a, unsigned char b) {
          return __CHAR_TO_LOWER(a) == __CHAR_TO_LOWER(b);
        }
    );
}

#ifdef TOOLBOX_ENABLE_MB
/**
 * Wide version
 * @param s1
 * @param s2
 * @return
 */
inline bool equalsIgnoreWCase(const std::wstring &s1, const std::wstring &s2) {
    if (s1.length() != s2.length()) return false;

    return std::equal(
        s2.begin(), s2.end(),
        s1.begin(),
        [](wchar_t a, wchar_t b) {
          return __WCHAR_TO_LOWER(a) == __WCHAR_TO_LOWER(b);
        }
    );
}
#endif

/**
 * Works like sprintf but with std::string and returns std::string
 * @tparam Args any type
 * @param format format string
 * @param args
 * @return
 */
template<typename... Args>
inline std::string format(const std::string &format, Args ...args) {
    auto *tmp = new char[format.length()];

    sprintf(tmp, format.c_str(), args...);

    std::string out = std::string(tmp);
    delete[] tmp;

    return out;
}
}

namespace fs {
inline const std::string readFile(const std::string &path) {
    std::ifstream input(path);

    if (!input.is_open()) {
        perror(strerror(errno));
        return "";
    }

    const std::string out = toolboxpp::strings::toString(input);
    input.close();
    return out;
}
inline bool writeFile(const std::string &path, const std::string &data) {
    std::ofstream out(path);
    if (!out.is_open()) {
        perror(strerror(errno));
        return false;
    }

    out << data;
    out.flush();
    out.close();
    return true;
}
inline bool exists(const std::string &path) {
    struct stat s;
    return stat(path.c_str(), &s) == 0;
}
}

namespace numbers {
inline bool isInteger(const std::string &input) {
    return std::regex_match(input, std::regex(R"(^[-]?[0-9eE+]+?$)"));
}
inline bool isReal(const std::string &input) {
    return std::regex_match(input, std::regex(R"(^[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$)"));
}

class decimal_formatter {
public:
    decimal_formatter() = default;
    decimal_formatter(int num) {
        std::stringstream ss;
        ss << num;
        m_num = ss.str();
    }
    decimal_formatter(double num) {
        std::stringstream ss;
        ss << num;
        m_num = ss.str();
    }
    decimal_formatter(float num) {
        std::stringstream ss;
        ss << num;
        m_num = ss.str();
    }
    decimal_formatter(const std::string &num) : m_num(num) { }
    decimal_formatter(const char *&num) : m_num(std::string(num)) { }
    decimal_formatter(std::string &&num) : m_num(std::move(num)) { }

    std::string operator()(const std::string &num) {
        m_num = num;
        return format();
    }

    decimal_formatter &set_delimiter(const std::string &delimiter) {
        m_delimiter = delimiter;
        return *this;
    }
    decimal_formatter &set_delimiter(char delimiter) {
        std::stringstream ss;
        ss << delimiter;
        m_delimiter = ss.str();
        return *this;
    }
    decimal_formatter &set_decimals_group(size_t num) {
        m_decimals = num;
        return *this;
    }
    decimal_formatter &set_max_fractions(size_t max_fractions) {
        m_max_fractions = max_fractions;
        return *this;
    }
    decimal_formatter &set_min_fractions(size_t min_fractions) {
        m_min_fractions = min_fractions;
        return *this;
    }

    std::string format() const {
        if (m_num.empty()) {
            throw std::runtime_error("Empty number passed to decimal formatter");
        }
        std::pair<std::string, std::string> lr = toolboxpp::strings::splitPair(m_num, '.');

        std::stringstream out;
        std::stack<std::string> parts;

        if (m_min_fractions > 0) {
            size_t fr_size = lr.second.size();
            // if fractial size less than min required fractions, adding zeroes to end
            if (fr_size < m_min_fractions) {
                parts.push(toolboxpp::strings::repeat('0', m_min_fractions - fr_size));
                parts.push(lr.second);
            } else if (fr_size >= m_min_fractions) {
                if (fr_size > m_max_fractions) {
                    parts.push(lr.second.substr(0, m_max_fractions));
                } else {
                    parts.push(lr.second);
                }
            }

            parts.push(".");
        }

        size_t prev_i;
        for (size_t i = lr.first.size(); i > 0;) {
            prev_i = i;
            if (m_decimals >= i) {
                i = 0;
            } else {
                i -= m_decimals;
            }

            size_t len = (i == 0) ? prev_i : m_decimals;
            std::string part = lr.first.substr(i, len);

            parts.push(std::move(part));
            if (i != 0) {
                parts.push(std::string(m_delimiter));
            }
        }

        while (!parts.empty()) {
            out << parts.top();
            parts.pop();
        }

        return out.str();
    }
private:
    std::string m_num;
    size_t m_decimals = 3;
    size_t m_max_fractions = 18;
    size_t m_min_fractions = 4;
    std::string m_delimiter = " ";
};
}

namespace data {

/// \brief Convert hex string to unsigned byte vector
/// \param hex
/// \return
static std::vector<uint8_t> hexToBytes(const std::string &hex) {
    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        auto byte = (uint8_t) strtol(byteString.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

/// \brief Convert byte array to hex string
/// \param data
/// \param len
/// \return output len will be len*2, as 1 byte in hex is a 2 chars
static std::string bytesToHex(const uint8_t *data, size_t len) {
    std::stringstream ss;
    ss << std::hex;
    for (size_t i = 0; i < len; ++i) {
        ss << std::setw(2) << std::setfill('0') << (int) data[i];
    }

    return ss.str();
}

/// \brief Convert any integral type to byte array
/// \tparam NumT any integral type, that can be asserted with std::is_integral
/// \param num value
/// \param out output vector ref
template<typename NumT>
void numToBytes(const NumT num, std::vector<uint8_t> &out) {
    static_assert(std::is_integral<NumT>::value, "Only integral types can be passed");

    size_t sz = sizeof(num);
    for (size_t i = 0; i < sz; i++) {
        out[(out.size() - 1) - i] = (num >> (i * 8));
    }
}

inline uint8_t operator ""_byte(unsigned long long val) {
    return (uint8_t) val;
}

inline uint8_t operator ""_bytes(unsigned long long val) {
    return (uint8_t) val;
}

inline uint16_t operator ""_dbyte(unsigned long long val) {
    return (uint16_t) val;
}

inline uint16_t operator ""_dbytes(unsigned long long val) {
    return (uint16_t) val;
}

/// \brief Special class to help you handle bytes
class bytes_data {
public:
    using container_t = std::vector<uint8_t>;
    using iterator = container_t::iterator;
    using const_iterator = container_t::const_iterator;
    using reverse_iterator = container_t::reverse_iterator;

    template<typename NumT>
    static bytes_data read_number(NumT num) {
        bytes_data out(sizeof(num));
        numToBytes<NumT>(num, out.get());
        return out;
    }

    template<typename NumT>
    static bytes_data read_number(NumT num, size_t outSize) {
        assert(outSize >= sizeof(num));
        bytes_data out(outSize);
        numToBytes<NumT>(num, out.get());
        return out;
    }

    bytes_data() = default;
    virtual ~bytes_data() = default;
    bytes_data(const bytes_data &other) = default;
    bytes_data(bytes_data &&other) = default;
    bytes_data &operator=(bytes_data other) noexcept {
        std::swap(m_data, other.m_data);
        return *this;
    }

    explicit bytes_data(std::size_t size) {
        m_data.resize(size);
    }

    bytes_data(const char *hexString) : m_data(hexToBytes(hexString)) { }
    bytes_data(const std::string &hexString) : m_data(hexToBytes(hexString)) { }
    bytes_data(std::vector<uint8_t> data) : m_data(std::move(data)) { }
    bytes_data(const uint8_t *data, size_t len) {
        m_data.resize(len);
        memcpy(this->data(), data, len);
    }

    const std::vector<uint8_t> &get() const {
        return m_data;
    }

    std::vector<uint8_t> &get() {
        return m_data;
    }

    uint8_t at(size_t n) const {
        return get().at(n);
    }

    std::string to_hex() const {
        return bytesToHex(&m_data[0], size());
    }

    size_t size() const {
        return m_data.size();
    }

    bool empty() const {
        return m_data.empty();
    }

    operator bool() const noexcept {
        return !empty();
    }

    uint8_t *data() {
        return &m_data[0];
    }

    const uint8_t *cdata() const {
        return &m_data[0];
    }

    std::string to_string() const {
        return std::string(get().begin(), get().end());
    }

    std::vector<uint8_t> take_first(size_t n) const {
        std::vector<uint8_t> out;
        out.insert(out.begin(), m_data.begin(), m_data.begin() + n);
        return out;
    }

    std::vector<uint8_t> take_last(size_t n) const {
        std::vector<uint8_t> out;
        out.insert(out.begin(), m_data.end() - n, m_data.end());
        return out;
    }

    std::vector<uint8_t> take_range_from(size_t from) const {
        return take_range(from, size());
    }

    std::vector<uint8_t> take_range_to(size_t to) const {
        return take_range(0, to);
    }

    std::vector<uint8_t> take_range(size_t from, size_t to) const {
        if (from > to) {
            throw std::out_of_range("'from' can't be more than 'to'");
        } else if (to > size()) {
            throw std::out_of_range("'to' can't be more than size()");
        } else if (from > size()) {
            throw std::out_of_range("'from' can't be more than size()");
        }

        std::vector<uint8_t> out;
        out.reserve(to - from);
        out.insert(out.begin(), m_data.begin() + from, m_data.begin() + to);
        return out;
    }

    size_t write(size_t pos, const uint8_t *data, size_t dataLen) {
        if (pos >= size()) {
            throw std::out_of_range("Position overflows size of this buffer. Use push_back method instead");
        }
        for (size_t i = 0; i < dataLen; i++) {
            m_data[i + pos] = data[i];
        }
        return dataLen;
    }

    /// \brief Write method overwrite existing data, and if input data.size() > buffer.size(), it resizes current buffer
    /// \param pos
    /// \param data
    /// \return
    size_t write(size_t pos, const bytes_data &data) {
        return write(pos, data.get());
    }

    size_t write(size_t pos, const std::vector<uint8_t> &data) {
        return write(pos, data.begin(), data.end());
    }

    size_t write(size_t pos, std::vector<uint8_t> &&data) {
        return write(pos, data.begin(), data.end());
    }

    template<typename _InputIterator>
    size_t write(iterator position, _InputIterator start, _InputIterator end) {
        size_t dist = std::distance(begin(), position);
        return write(dist, start, end);
    }

    template<typename _InputIterator>
    size_t write(size_t pos, _InputIterator start, _InputIterator end) {
        std::map<size_t, uint8_t> tmp;
        size_t i = pos;
        for (auto it = start; it != end; ++it) {
            tmp[i++] = *it;
        }

        return write_batch(std::move(tmp));
    }

    void write(size_t pos, uint8_t val) {
        if (pos >= size()) {
            size_t alloc;
            if (size() != 0) {
                alloc = size() + 1;
            } else {
                alloc = 1;
            }
            m_data.resize(alloc);
        }
        m_data[pos] = val;
    }

    void write(size_t pos, uint16_t val) {
        write_batch(
            {
                {pos + 0, (uint8_t) (val >> 8u)},
                {pos + 1, (uint8_t) (val)}
            });
    }

    void write(size_t pos, uint32_t val) {
        write_batch(
            {
                {pos + 0, (uint8_t) (val >> 24u)},
                {pos + 1, (uint8_t) (val >> 16u)},
                {pos + 2, (uint8_t) (val >> 8u)},
                {pos + 3, (uint8_t) (val)}
            });
    }

    void write(size_t pos, uint64_t val) {
        write_batch(
            {
                {pos + 0, (uint8_t) (val >> 56u)},
                {pos + 1, (uint8_t) (val >> 48u)},
                {pos + 2, (uint8_t) (val >> 40u)},
                {pos + 3, (uint8_t) (val >> 32u)},
                {pos + 4, (uint8_t) (val >> 24u)},
                {pos + 5, (uint8_t) (val >> 16u)},
                {pos + 6, (uint8_t) (val >> 8u)},
                {pos + 7, (uint8_t) (val)}
            });
    }

    size_t write_tail(size_t pos, const uint8_t *data, size_t dataLen) {
        std::vector<uint8_t> tmp(data, data + dataLen);
        return write_tail(pos, tmp);
    }

    size_t write_tail(size_t pos, const bytes_data &data) {
        return write_tail(pos, data.get());
    }

    size_t write_tail(size_t pos, const std::vector<uint8_t> &data) {
        std::vector<uint8_t> tmp(m_data.begin(), m_data.begin() + pos);
        tmp.insert(tmp.begin() + pos, data.begin(), data.end());
        clear();
        m_data = std::move(tmp);
        return data.size();
    }

    /// \brief batch reduces allocations
    /// \param vals map of positions -> values
    /// \return written values count
    size_t write_batch(std::map<size_t, uint8_t> &&vals) {
        if (vals.empty()) {
            return 0;
        }

        size_t minPos = SIZE_MAX;
        size_t maxPos = 0;
        for (const auto &it: vals) {
            minPos = std::min(minPos, it.first);
            maxPos = std::max(maxPos, it.first);
        }

        if (maxPos >= size()) {
            size_t alloc;
            if (size() != 0) {
                alloc = minPos + vals.size() - size();
                alloc += size();
            } else {
                alloc = vals.size();
            }
            m_data.resize(alloc);
        }

        for (auto &&el: vals) {
            m_data[el.first] = el.second;
        }

        return vals.size();
    }

    void push_back(uint8_t val) {
        m_data.push_back(val);
    }

    void push_back(uint16_t val) {
        m_data.push_back(val >> 8u);
        m_data.push_back(val & 0xFFu);
    }

    void push_back(uint32_t val) {
        m_data.push_back(val >> 24u);
        m_data.push_back(val >> 16u);
        m_data.push_back(val >> 8u);
        m_data.push_back(val & 0xFFu);
    }

    void push_back(uint64_t val) {
        m_data.push_back(val >> 56u);
        m_data.push_back(val >> 48u);
        m_data.push_back(val >> 40u);
        m_data.push_back(val >> 32u);
        m_data.push_back(val >> 24u);
        m_data.push_back(val >> 16u);
        m_data.push_back(val >> 8u);
        m_data.push_back(val & 0xFFu);
    }

    void push_back(const std::vector<uint8_t> &data) {
        m_data.insert(m_data.end(), data.begin(), data.end());
    }

    void push_back(const bytes_data &data) {
        push_back(data.get());
    }

    const_iterator cbegin() const {
        return m_data.cbegin();
    }

    const_iterator begin() const {
        return m_data.begin();
    }

    iterator begin() {
        return m_data.begin();
    }

    const_iterator cend() const {
        return m_data.cend();
    }

    const_iterator end() const {
        return m_data.end();
    }

    iterator end() {
        return m_data.end();
    }

    void push_back(const uint8_t *data, size_t len) {
        std::vector<uint8_t> tmp(data, data + len);
        m_data.insert(m_data.end(), tmp.begin(), tmp.end());
    }

    void push_back(const_iterator start, const_iterator end) {
        m_data.insert(m_data.end(), start, end);
    }

    void push_back(iterator start, iterator end) {
        m_data.insert(m_data.end(), start, end);
    }

    inline uint8_t &operator[](std::size_t idx) noexcept {
        return m_data[idx];
    }

    inline uint8_t operator[](std::size_t idx) const noexcept {
        return m_data[idx];
    }

    bool operator==(const bytes_data &other) const noexcept {
        return m_data == other.m_data;
    }

    bool operator!=(const bytes_data &other) const noexcept {
        return m_data != other.m_data;
    }

    /// \brief Make sure that 'other' have at least size() elements, otherwise UB is your friend
    /// \param other
    /// \return
    bool operator==(const uint8_t *other) const noexcept {
        if (!other || !other[0] || !other[m_data.size() - 1]) {
            return false;
        }

        for (size_t i = 0; i < m_data.size(); i++) {
            if (m_data[i] != other[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const uint8_t *other) const noexcept {
        return !operator==(other);
    }

    void clear() {
        static std::atomic<uint8_t> s_cleanseCounter{0u};
        auto *p = data();
        size_t const len = (uint8_t *) (data() + size()) - p;
        size_t loop = len;
        size_t count = s_cleanseCounter;
        while (loop--) {
            *(p++) = (uint8_t) count;
            count += (17u + ((size_t) p & 0x0Fu));
        }
        p = (uint8_t *) memchr((uint8_t *) data(), (uint8_t) count, len);
        if (p) {
            count += (63u + (size_t) p);
        }

        s_cleanseCounter = (uint8_t) count;
        memset((uint8_t *) data(), 0, len);
    }

    void resize(size_t sz) {
        m_data.resize(sz);
    }

    template<typename T>
    T to_num() const {
        return to_num<T>(0);
    }

    template<typename T>
    T to_num(size_t from, size_t to) const {
        auto data = take_range(from, to);

        size_t len = sizeof(T);
        if (len == 1) {
            return data[0];
        } else if (len == 2) {
            return static_cast<uint64_t>(data[0]) << 8u |
                static_cast<uint64_t>(data[1]);
        } else if (len == 4) {
            return static_cast<uint64_t>(data[0]) << 24u |
                static_cast<uint64_t>(data[1]) << 16u |
                static_cast<uint64_t>(data[2]) << 8u |
                static_cast<uint64_t>(data[3]);
        } else if (sizeof(uint64_t) == len) {
            return
                static_cast<uint64_t>(data[0]) << 56u |
                    static_cast<uint64_t>(data[1]) << 48u |
                    static_cast<uint64_t>(data[2]) << 40u |
                    static_cast<uint64_t>(data[3]) << 32u |
                    static_cast<uint64_t>(data[4]) << 24u |
                    static_cast<uint64_t>(data[5]) << 16u |
                    static_cast<uint64_t>(data[6]) << 8u |
                    static_cast<uint64_t>(data[7]);
        }

        return 0;
    }

    template<typename T>
    T to_num(size_t readFrom) const {
        return to_num<T>(readFrom, readFrom + sizeof(T));
    }

    explicit operator uint8_t() const {
        return to_num<uint8_t>();
    }

    explicit operator char() const {
        return to_num<char>();
    }

    explicit operator uint16_t() const {
        return to_num<uint16_t>();
    }

    explicit operator int16_t() const {
        return to_num<int16_t>();
    }

    explicit operator uint32_t() const {
        return to_num<uint32_t>();
    }

    explicit operator int32_t() const {
        return to_num<int32_t>();
    }

    explicit operator uint64_t() const {
        return to_num<uint64_t>();
    }

    explicit operator int64_t() const {
        return to_num<int64_t>();
    }

protected:
    container_t m_data;

};

class bytes_buffer : public bytes_data {
public:
    bytes_buffer() : bytes_data() { }
    virtual ~bytes_buffer() override = default;
    bytes_buffer(const bytes_data &other) : bytes_data(other) { }
    bytes_buffer(bytes_data &&other) : bytes_data(other) { }
    bytes_buffer(size_t size) : bytes_data(size) { }
    bytes_buffer(const char *hexString) : bytes_data(hexString) { }
    bytes_buffer(const std::string &hexString) : bytes_data(hexString) { }
    bytes_buffer(const std::vector<uint8_t> &data) : bytes_data(data) { }
    bytes_buffer(const uint8_t *data, size_t len) : bytes_data(data, len) { }

    size_t pop_front_to(size_t outPosition, bytes_data &out) {
        if (outPosition >= out.size()) {
            throw std::out_of_range("'outPosition' is >= than 'out' size");
        }
        return pop_front_to(out.size() - outPosition, outPosition, out);
    }

    size_t pop_front_to(size_t readLength, size_t position, bytes_data &out) {
        return pop_front_to(readLength, out.begin() + position, out);
    }

    size_t pop_front_to(size_t readLength, iterator position, bytes_data &out) {
        if (readLength == 0 || out.empty() || empty()) {
            return 0;
        }

        size_t s = readLength;
        if (readLength >= size()) {
            s = size();
        }

        if (s > out.size()) {
            s = out.size();
        }

        auto it = std::distance(out.begin(), position);
        for (size_t i = 0; i < s; i++) {
            out[i + it] = m_data.at(i);
        }

        // if we have read full buffer, than cleanup this
        if (s == size()) {
            clear();
            resize(0);
        } else {
            // otherwise, taking begin()+s + end() and rewrite this with new vector
            auto newData = std::vector<uint8_t>(m_data.begin() + s, m_data.end());
            clear();
            resize(newData.size());
            m_data = std::move(newData);
        }

        return s;
    }
};
}

//LCOV_EXCL_START
namespace console {

#ifndef TOOLBOXPP_WIN32

inline ssize_t _password(const std::string &message, char **pw, size_t sz, int mask = 0, FILE *fp = stdin) {
    if (!pw || !sz || !fp) return -1;       /* validate input   */
#ifdef MAXPW
    if (sz > MAXPW) sz = MAXPW;
#endif

    std::cout << message << ": \n";

    if (*pw == nullptr) {              /* reallocate if no address */
        void *tmp = realloc(*pw, sz * sizeof **pw);
        if (!tmp)
            return -1;
        memset(tmp, 0, sz);    /* initialize memory to 0   */
        *pw = static_cast<char *>(tmp);
    }

    size_t idx = 0;         /* index, number of chars in read   */
    int c = 0;

    termios old_kbd_mode;    /* orig keyboard settings   */
    termios new_kbd_mode;

    if (tcgetattr(0, &old_kbd_mode)) { /* save orig settings   */
        fprintf(stderr, "%s() error: tcgetattr failed.\n", __func__);
        return -1;
    }   /* copy old to new */
    memcpy(&new_kbd_mode, &old_kbd_mode, sizeof(struct termios));

    new_kbd_mode.c_lflag &= ~(ICANON | ECHO);  /* new kbd flags */
    new_kbd_mode.c_cc[VTIME] = 0;
    new_kbd_mode.c_cc[VMIN] = 1;
    if (tcsetattr(0, TCSANOW, &new_kbd_mode)) {
        fprintf(stderr, "%s() error: tcsetattr failed.\n", __func__);
        return -1;
    }

    /* read chars from fp, mask if valid char specified */
    while (((c = fgetc(fp)) != '\n' && c != EOF && idx < sz - 1) ||
        (idx == sz - 1 && c == 127)) {
        if (c != 127) {
            if (31 < mask && mask < 127)    /* valid ascii char */
                fputc(mask, stdout);
            (*pw)[idx++] = c;
        } else if (idx > 0) {         /* handle backspace (del)   */
            if (31 < mask && mask < 127) {
                fputc(0x8, stdout);
                fputc(' ', stdout);
                fputc(0x8, stdout);
            }
            (*pw)[--idx] = 0;
        }
    }
    (*pw)[idx] = 0; /* null-terminate   */

    /* reset original keyboard  */
    if (tcsetattr(0, TCSANOW, &old_kbd_mode)) {
        fprintf(stderr, "%s() error: tcsetattr failed.\n", __func__);
        return -1;
    }

    if (idx == sz - 1 && c != '\n') /* warn if pw truncated */
        fprintf(stderr, " (%s() warning: truncated at %zu chars.)\n",
                __func__, sz - 1);

    return idx; /* number of chars in passwd    */
}

inline std::string promptPassword(const std::string &message, const size_t passSize = 32) {
    char *pw = new char[passSize];

    _password(message, &pw, passSize);

    auto out = std::string(pw);
    delete[] pw;
    return out;
}

#endif

inline bool confirm(std::istream &in, std::ostream &out, const std::string &message, bool defValue = false) {
    std::string res;
    const std::string yesNo = defValue ? "yes" : "no";
    do {
        out << message;
        out << " (yes|no) [" << yesNo << "]: ";
        std::getline(in, res);

    } while (
        !res.empty() &&
            !toolboxpp::strings::equalsIgnoreCase(res, "yes") &&
            !toolboxpp::strings::equalsIgnoreCase(res, "no") &&
            !toolboxpp::strings::equalsIgnoreCase(res, "y") &&
            !toolboxpp::strings::equalsIgnoreCase(res, "n")
        );

    if (res.empty()) {
        return defValue;
    }

    return toolboxpp::strings::equalsIgnoreCase(res, "yes") || toolboxpp::strings::equalsIgnoreCase(res, "y");
}

inline bool confirm(const std::string &message, bool defValue = false) {
    return confirm(std::cin, std::cout, message, defValue);
}
inline std::string prompt(std::istream &in, std::ostream &out, const std::string &message,
                          bool required = false, const std::string &defValue = "") {
    std::string res;
    do {
        out << message;
        if (!defValue.empty()) {
            out << " [" << defValue << "]: ";
        } else {
            out << ": ";
        }

        std::getline(in, res);

    } while ((required && res.empty()));

    if (res.empty()) {
        return defValue;
    }
    return res;
}
inline std::string prompt(const std::string &message, bool required = false, const std::string &defValue = "") {
    return prompt(std::cin, std::cout, message, required, defValue);
}
}
//LCOV_EXCL_STOP

using level_t = unsigned short;
constexpr const static level_t LEVEL_DEBUG = (1 << 0);
constexpr const static level_t LEVEL_WARNING = (1 << 1);
constexpr const static level_t LEVEL_INFO = (1 << 2);
constexpr const static level_t LEVEL_ERROR = (1 << 3);
constexpr const static level_t LEVEL_CRITICAL = (1 << 4);
constexpr const static level_t LEVEL_ALL =
    LEVEL_DEBUG
        | LEVEL_WARNING
        | LEVEL_INFO
        | LEVEL_ERROR
        | LEVEL_CRITICAL;

constexpr const static level_t VERBOSITY_0 = LEVEL_CRITICAL | LEVEL_ERROR;
constexpr const static level_t VERBOSITY_1 = VERBOSITY_0 | LEVEL_INFO;
constexpr const static level_t VERBOSITY_2 = LEVEL_ALL;

class Logger {
public:
    Logger(const Logger &copy) = delete;
    Logger(Logger &&copy) = delete;
    Logger &operator=(const Logger &copy) = delete;
    Logger &operator=(Logger &&copy) = delete;
private:
    typedef std::recursive_mutex mutex_t;

    level_t level = LEVEL_ALL;
    std::size_t bufferLimit = 0;
    bool printDateTime = true;

    std::ostream *outStream;
    std::ostream *errStream;

    mutex_t logLock;

    std::unordered_map<level_t, std::queue<std::string>> logs;
    std::unordered_map<level_t, std::string> levelMap = {
        {LEVEL_DEBUG,    "debug"},
        {LEVEL_WARNING,  "warning"},
        {LEVEL_INFO,     "info"},
        {LEVEL_ERROR,    "error"},
        {LEVEL_CRITICAL, "critical"},
    };

    Logger() :
        outStream(&std::cout),
        errStream(&std::cerr) {
    }
    ~Logger() = default;

    std::string levelToString(level_t level) {
        if (!levelMap.count(level)) {
            return "Unknown error!";
        }

        return levelMap[level];
    }
    level_t stringToLevel(const std::string &level) {
        for (auto &t: levelMap) {
            if (toolboxpp::strings::equalsIgnoreCase(t.second, level)) {
                return t.first;
            }
        }

        return LEVEL_ERROR;
    }
    bool canLog(level_t level) {
        return
            ((this->level & level) != 0) &&
                (bufferLimit == 0 || logs[level].size() <= bufferLimit);
    }

public:
    static Logger &get() {
        static Logger logger;
        return logger;
    }

    void setOutStream(std::ostream *out) {
        outStream = out;
    }
    void setErrStream(std::ostream *out) {
        errStream = out;
    }
    void setLevel(level_t level) {
        this->level = level;
    }
    void setLevel(const std::string &stringLevel) {
        this->level = stringToLevel(stringLevel);
    }
    void setVerbosity(level_t verbosity) {
        switch (verbosity) {
            case 0:setLevel(VERBOSITY_0);
                break;
            case 1:setLevel(VERBOSITY_1);
                break;
            case 2:setLevel(VERBOSITY_2);
                break;

            default: setLevel(VERBOSITY_2);
        }
    }
    void setDateTimeEnabled(bool enabled) {
        printDateTime = enabled;
    }

    /**
     * @param limit 0 means infinite
     */
    void setBufferLimit(std::size_t limit) {
        bufferLimit = limit;
    }
    void clear() {
        logs.clear();
    }
    void flush() {
        #ifndef TOOLBOXPP_LOGGER_NO_MUTEX
        std::lock_guard<mutex_t> locker(logLock);
        #endif
        for (auto &levels: logs) {
            while (!levels.second.empty()) {
                if (levels.first > LEVEL_INFO) {
                    *errStream << levels.second.front() << std::endl;
                } else {
                    *outStream << levels.second.front() << std::endl;
                }

                levels.second.pop();
            }
        }
    }

    void log(level_t level, const char *tag, const char *message) {
        log(level, std::string(tag), std::string(message));
    }
    void log(level_t level, const std::string &tag, const std::string &message) {
        if (!canLog(level)) {
            return;
        }
        #ifndef TOOLBOXPP_LOGGER_NO_MUTEX
        std::lock_guard<mutex_t> locker(logLock);
        #endif
        std::string out = tag + ": " + message;
        logs[level].push(out);
        #ifndef TOOLBOXPP_LOGGER_NO_AUTOFLUSH
        flush();
        #endif
    }
    void log(level_t level, const char *file, int line, const char *tag, const char *message) {
        log(level, std::string(file), line, std::string(tag), std::string(message));
    }
    void log(level_t level,
             const std::string &file,
             int line,
             const std::string &tag,
             const std::string &message) {
        if (!canLog(level)) {
            return;
        }

        std::stringstream out;

        if (printDateTime) {
            time_t t = time(nullptr);
            tm *timeinfo = localtime(&t);
            char tbuffer[30];
            strftime(tbuffer, 30, "%Y-%m-%d %T", timeinfo);
            out << "[" << tbuffer << "]";
        }

        out << "[" << std::setw(8) << levelToString(level) << "] ";

        if (level < LEVEL_ERROR) {
            out << tag << ": " << message;
        } else {
            out << tag << ": " << message << "\n\t" << file << ":" << line;
        }

        #ifndef TOOLBOXPP_LOGGER_NO_MUTEX
        std::lock_guard<mutex_t> locker(logLock);
        #endif
        logs[level].push(out.str());

        #ifndef TOOLBOXPP_LOGGER_NO_AUTOFLUSH
        flush();
        #endif
    }

    // ERROR
    void error(const char *file, int line, const char *tag, const char *message) {
        error(std::string(file), line, std::string(tag), std::string(message));
    }
    void error(const std::string &file, int line, const std::string &tag, const std::string &message) {
        log(LEVEL_ERROR, file, line, tag, message);
    }
    void error(const std::string &tag, const std::string &message) {
        log(LEVEL_ERROR, tag, message);
    }
    void error(const char *tag, const char *message) {
        error(std::string(tag), std::string(message));
    }

    // CRITICAL
    void critical(const char *file, int line, const char *tag, const char *message) {
        critical(std::string(file), line, std::string(tag), std::string(message));
    }
    void critical(const std::string &file, int line, const std::string &tag, const std::string &message) {
        log(LEVEL_CRITICAL, file, line, tag, message);
    }
    void critical(const std::string &tag, const std::string &message) {
        log(LEVEL_CRITICAL, tag, message);
    }
    void critical(const char *tag, const char *message) {
        critical(std::string(tag), std::string(message));
    }

    // WARNING
    void warning(const char *file, int line, const char *tag, const char *message) {
        warning(std::string(file), line, std::string(tag), std::string(message));
    }
    void warning(const std::string &file, int line, const std::string &tag, const std::string &message) {
        log(LEVEL_WARNING, file, line, tag, message);
    }
    void warning(const std::string &tag, const std::string &message) {
        log(LEVEL_WARNING, tag, message);
    }
    void warning(const char *tag, const char *message) {
        warning(std::string(tag), std::string(message));
    }

    // INFO
    void info(const char *file, int line, const char *tag, const char *message) {
        info(std::string(file), line, std::string(tag), std::string(message));
    }
    void info(const std::string &file, int line, const std::string &tag, const std::string &message) {
        log(LEVEL_INFO, file, line, tag, message);
    }
    void info(const std::string &tag, const std::string &message) {
        log(LEVEL_INFO, tag, message);
    }
    void info(const char *tag, const char *message) {
        info(std::string(tag), std::string(message));
    }

    // DEBUG
    void debug(const char *file, int line, const char *tag, const char *message) {
        debug(std::string(file), line, std::string(tag), std::string(message));
    }
    void debug(const std::string &file, int line, const std::string &tag, const std::string &message) {
        log(LEVEL_DEBUG, file, line, tag, message);
    }
    void debug(const std::string &tag, const std::string &message) {
        log(LEVEL_DEBUG, tag, message);
    }
    void debug(const char *tag, const char *message) {
        debug(std::string(tag), std::string(message));
    }
};

class Profiler {
private:
    using hires_clock = std::chrono::high_resolution_clock;

    typedef std::chrono::high_resolution_clock::time_point hires_time_t;
    typedef std::mutex mutex_t;
    std::unordered_map<std::string, hires_time_t> timings;
    mutable mutex_t lock;
    Profiler() = default;
public:
    static Profiler &get() {
        static Profiler p;
        return p;
    }

    Profiler(const Profiler &) = delete;
    Profiler(Profiler &&) = delete;
    ~Profiler() {
        timings.clear();
    }

    void clear() {
        std::lock_guard<mutex_t> locker(lock);
        timings.clear();
    }

    std::size_t size() const {
        std::lock_guard<mutex_t> locker(lock);
        std::size_t out = timings.size();

        return out;
    }

    void begin(std::string tag) {
        std::lock_guard<mutex_t> locker(lock);
        if (timings.count(tag)) {
            return;
        }

        timings[tag] = hires_clock::now();
    }

    void end(std::string tag, double *result = nullptr) {
        std::lock_guard<mutex_t> locker(lock);
        if (!timings.count(tag)) {
            return;
        }

        hires_time_t past = timings[tag];
        hires_time_t now = hires_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - past);
        timings.erase(tag);
        if (result != nullptr) {
            *result = time_span.count();
        } else {
            L_DEBUG_F(tag, "Profiling time: %lf ms", time_span.count());
        }
    }
};

}

#endif // TOOLBOXPP_HPP
