/*!
 * toolboxpp. 2017
 *
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_HPP
#define TOOLBOXPP_HPP

#ifdef _WIN32
#include <tchar.h>
#define __CHAR_TO_LOWER(c) _tolower(c)
#define __CHAR_TO_UPPER(C) _toupper(c)
#else
#define __CHAR_TO_LOWER(c) std::tolower(c)
#define __WCHAR_TO_LOWER(c) std::towlower(c)
#define __CHAR_TO_UPPER(c) std::toupper(c)
#define __WCHAR_TO_UPPER(c) std::towupper(c)
#endif

#include "toolboxppconfig.h"

#include <chrono>
#include <string>
#include <iosfwd>
#include <vector>
#include <map>
#include <deque>
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

#ifndef INIT_WCHAR_UNICODE
#if MS_STDLIB_BUGS
#define INIT_WCHAR_UNICODE() \
constexpr char cp_utf16le[] = ".1200"; \
setlocale( LC_ALL, cp_utf16le ); \
_setmode( _fileno(stdout), _O_WTEXT );
#else
#define INIT_WCHAR_UNICODE() \
  constexpr char locale_name[] = ""; \
  setlocale( LC_ALL, locale_name ); \
  std::locale::global(std::locale(locale_name)); \
  std::wcin.imbue(std::locale()); \
  std::wcout.imbue(std::locale());
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
typedef size_t strlen_t;
typedef const std::string &const_string;

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
/**
 * Serch character in string
 * @param source
 * @param substring
 * @return
 */
inline bool hasWSubstring(wchar_t substring, const std::wstring &source) {
    return source.find(substring) != std::string::npos;
}

#ifdef HAVE_REGEX_H
/**
 * Match regex pattern
 * @param pattern
 * @param source
 * @return
 */
inline bool hasRegex(const rxns::regex &pattern, const std::string &source) {
    rxns::smatch match;
    return rxns::regex_search(source, match, pattern);
}
/**
 * Match regex pattern.
 * @param pattern Just string. By default: search with flag std::regex_constants::icase
 * @param source
 * @return
 */
inline bool hasRegex(const std::string &pattern, const std::string &source) {
    return hasRegex(rxns::regex(pattern, rxns::regex_constants::icase), source);
}
/**
 * Returns all found groups in source with pattern
 * @param rx custom regex object
 * @param s
 * @return
 */
inline std::vector<std::vector<std::string>> matchAllRegexp(const rxns::regex &pattern, const std::string &source) {
    std::vector<std::vector<std::string>> capturedGroups;
    std::vector<std::string> capturedSubgroups;
    const rxns::sregex_token_iterator endIterator;
    for (rxns::sregex_token_iterator it(source.cbegin(), source.cend(), pattern); it != endIterator; ++it) {
        capturedSubgroups.clear();
        std::string group = *it;
        rxns::smatch res;
        if (rxns::regex_search(group, res, pattern)) {
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
    return matchAllRegexp(rxns::regex(pattern, rxns::regex_constants::icase), source);
}
/**
 * Take first regex match and return it
 * @param pattern
 * @param source
 * @return
 */
inline std::string matchRegexpFirst(const rxns::regex &pattern, const std::string &source) {
    rxns::smatch results;
    std::string result;
    bool found = rxns::regex_search(source, results, pattern);
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
    return matchRegexpFirst(rxns::regex(pattern, rxns::regex_constants::icase), source);
}

/**
 * Less boilerplate for std::regex
 * Attention! Don't use this method to check if regex pattern has in string, use {@code hasRegex()} instead,
 * because different compilers compiles different results
 * @param rxPattern
 * @param source
 * @return
 */
inline const std::vector<std::string> matchRegexp(const rxns::regex &rxPattern, const std::string &source) {
    rxns::smatch result;
    rxns::regex_search(source, result, rxPattern);

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
    return matchRegexp(rxns::regex(pattern, rxns::regex_constants::icase), source);
}

/**
 * Splits string by delimiter to pair
 * @param source
 * @param delimiter
 * @return
 */
#endif

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

/**
 * String to lower case (wide char)
 * @param s
 * @return
 */
inline std::wstring toWLower(const std::wstring &s) {
    std::wstringstream tmp;
    std::locale loc("");
    for (auto c: s) {
        // This is recommended
        tmp << std::tolower(c, loc);
    }

    return tmp.str();
}

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

/**
 *
 * @param s1
 * @param s2
 * @return
 */
inline bool equalsIgnoreCase(const std::string &s1, const std::string &s2) {
    if (s1.length() != s2.length()) return false;

    return std::equal(
        s2.begin(), s2.end(),
        s1.begin(),
        [](unsigned char a, unsigned char b) {
          return __CHAR_TO_LOWER(a) == __CHAR_TO_LOWER(b);
        }
    );
}

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

#ifdef HAVE_REGEX_H
namespace numbers {
inline bool isInteger(const std::string &input) {
    return rxns::regex_match(input, rxns::regex(R"(^[-]?[0-9eE+]+?$)"));
}
inline bool isReal(const std::string &input) {
    return rxns::regex_match(input, rxns::regex(R"(^[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$)"));
}
}
#endif

namespace console {
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

class Logger {
public:
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

    Logger(const Logger &copy) = delete;
    Logger(Logger &&copy) = delete;
    Logger &operator=(const Logger &copy) = delete;
    Logger &operator=(Logger &&copy) = delete;
private:
    typedef std::recursive_mutex mutex_t;

    level_t level = Logger::LEVEL_ALL;
    std::size_t bufferLimit = 0;
    bool printDateTime = true;

    std::ostream *outStream;
    std::ostream *errStream;

    mutex_t logLock;

    std::unordered_map<level_t, std::queue<std::string>> logs;
    std::unordered_map<level_t, std::string> levelMap = {
        {Logger::LEVEL_DEBUG,    "debug"},
        {Logger::LEVEL_WARNING,  "warning"},
        {Logger::LEVEL_INFO,     "info"},
        {Logger::LEVEL_ERROR,    "error"},
        {Logger::LEVEL_CRITICAL, "critical"},
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
