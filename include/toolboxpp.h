/*!
 * toolboxpp. 2017
 *
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#ifndef TOOLBOXPP_H
#define TOOLBOXPP_H

#if !_WIN32 && !_TOOLBOXPP_EXPORT
#define _TOOLBOXPP_EXPORT __attribute__ ((visibility ("default")))
#else
#define _TOOLBOXPP_EXPORT
#endif

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

namespace console {
bool confirm(std::istream &in, std::ostream &out, const std::string &message, bool defValue = false);
bool confirm(const std::string &message, bool defValue = false);
std::string prompt(std::istream &in, std::ostream &out, const std::string &message,
                   bool required = false, const std::string &defValue = "");
std::string prompt(const std::string &message, bool required = false, const std::string &defValue = "");
}

#ifdef HAVE_REGEX_H
namespace numbers {
bool isInteger(const std::string &input);
bool isReal(const std::string &input);
}
#endif

namespace fs {
const std::string readFile(const std::string &path);
bool writeFile(const std::string &path, const std::string &data);
bool exists(const std::string &path);
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
bool hasSubstringIgnoreCase(const std::string &substring, const std::string &source);

/**
 * Search substring in string
 * @param source
 * @param substring
 * @return
 */
bool hasSubstring(const std::string &substring, const std::string &source);
/**
 * Serch character in string
 * @param source
 * @param substring
 * @return
 */
bool hasSubstring(char substring, const std::string &source);
/**
 * Serch character in string
 * @param source
 * @param substring
 * @return
 */
bool hasWSubstring(wchar_t substring, const std::wstring &source);

#ifdef HAVE_REGEX_H
/**
 * Match regex pattern
 * @param pattern
 * @param source
 * @return
 */
bool hasRegex(const rxns::regex &pattern, const std::string &source);
/**
 * Match regex pattern.
 * @param pattern Just string. By default: search with flag std::regex_constants::icase
 * @param source
 * @return
 */
bool hasRegex(const std::string &pattern, const std::string &source);
/**
 * Returns all found groups in source with pattern
 * @param rx custom regex object
 * @param s
 * @return
 */
std::vector<std::vector<std::string>> matchAllRegexp(const rxns::regex &pattern, const std::string &source);
/**
 * Returns all found groups in source with pattern
 * @param pattern
 * @param source
 * @return
 */
std::vector<std::vector<std::string>> matchAllRegexp(const std::string &pattern, const std::string &source);
/**
 * Take first regex match and return it (with custom regex object)
 * @param pattern
 * @param source
 * @return
 */
std::string matchRegexpFirst(const std::string &pattern, const std::string &source);
/**
 * Take first regex match and return it
 * @param pattern
 * @param source
 * @return
 */
std::string matchRegexpFirst(const rxns::regex &pattern, const std::string &source);
/**
 * boilerplate to std::regex
 * @param pattern
 * @param source
 * @return
 */
const std::vector<std::string> matchRegexp(const std::string &pattern, const std::string &source);
/**
 * Less boilerplate to std::regex
 * @param rxPattern
 * @param source
 * @return
 */
const std::vector<std::string> matchRegexp(const rxns::regex &rxPattern, const std::string &source);
/**
 * Splits string by delimiter to pair
 * @param source
 * @param delimiter
 * @return
 */
#endif

std::pair<std::string, std::string> splitPair(const std::string &source, const std::string &delimiter);
/**
 * Splits string by char delimiter to pair
 * @param source
 * @param delimiter
 * @return
 */
std::pair<std::string, std::string> splitPair(const std::string &source, const char &delimiter);
/**
 * Splits string by char delimiter to vector list
 * @param source
 * @param delimiter
 * @return
 */
std::vector<std::string> split(const std::string &source, const std::string &delimiter);
/**
 * Splits string by delimiter to vector list
 * @param source
 * @param delimiter
 * @return
 */
std::vector<std::string> split(const std::string &source, const char &delimiter);

/**
 * @see substringReplace(const std::string &search,
									const std::string &replace,
									const std::string &source) but mutable behavior
 * @param search
 * @param replace
 * @param source Mutable
 * @return
 */
void replace(const std::string &search, const std::string &replace, std::string &source);

/**
 * @see substringReplaceAll(const std::string &search,
									   const std::string &replace,
									   const std::string &source)
 * @param search
 * @param replace
 * @param source Mutable
 */
void replaceAll(const std::string &search, const std::string &replace, std::string &source);

/*!
 *
 * @param search Search vector
 * @param replace Replace vector
 * @param source mutable string
 */
void replaceAll(const std::vector<std::string> &search,
                const std::vector<std::string> &replace,
                std::string &source);

/**
 * Replaces first occurrence "search" and replaces with "replace" parameter in "source" string.
 * @param search
 * @param replace
 * @param source Immutable
 * @return If search string were not found, function will return source string.
 */
std::string substringReplace(const std::string &search,
                             const std::string &replace,
                             const std::string &source);
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
std::string substringReplaceAll(const std::vector<std::string> &search,
                                const std::vector<std::string> &replace,
                                const std::string &source);
/**
 * Works like std::string substringReplace(const std::string &search, const std::string &replace, const std::string &source);
 * but replaces all "search" occurrences with "replace" string
 * @param search
 * @param replace
 * @param source
 * @return
 */
std::string substringReplaceAll(const std::string &search,
                                const std::string &replace,
                                const std::string &source);
/**
 * Find "removable" string in "source" and remove it
 * @param source Mutable value!
 * @param removable
 */
void removeSubstrings(std::string &source, const std::string &removable);
/**
 * Find all "removables" strings in "source" and remove it
 * @param source
 * @param removables
 */
void removeSubstrings(std::string &source, std::vector<std::string> removables);

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
std::string substrInverse(const std::string &source, char whence);
std::string substrInverse(const std::string &source, char begin, char end, long offset = 0);
std::string substrInverse(const std::string &source, const std::string &begin);
std::string substrInverse(const std::string &source, const std::string &begin, const std::string &end, long offset = 0);

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
std::string clipSubstring(const std::string &source, const std::string &search, size_t width, bool icase = false);

/**
 * Concatenates strings by glue
 * @param glue
 * @param strings
 * @return
 */
std::string glue(const std::string &glue, const std::vector<std::string> &strings);

/**
 * Input file stream to string
 * @param inputStream
 * @return
 */
std::string toString(std::ifstream &inputStream);

/**
 * String to lower case
 * @param s
 * @return
 */
std::string toLower(const std::string &s);

/**
 * String to lower case (wide char)
 * @param s
 * @return
 */
std::wstring toWLower(const std::wstring &s);

/**
 * String to upper case
 * @param s
 * @return
 */
std::string toUpper(const std::string &s);

/**
 * String to upper case
 * @param s
 * @return
 */
std::wstring toWUpper(const std::wstring &s);

/**
 *
 * @param s1
 * @param s2
 * @return
 */
bool equalsIgnoreCase(const std::string &s1, const std::string &s2);

/**
 * Wide version
 * @param s1
 * @param s2
 * @return
 */
bool equalsIgnoreWCase(const std::wstring &s1, const std::wstring &s2);

/**
 * Works like sprintf but with std::string and returns std::string
 * @tparam Args any type
 * @param format format string
 * @param args
 * @return
 */
template<typename... Args>
std::string format(const std::string &format, Args ...args) {
    auto *tmp = new char[format.length()];

    sprintf(tmp, format.c_str(), args...);

    std::string out = std::string(tmp);
    delete[] tmp;

    return out;
}
}

class _TOOLBOXPP_EXPORT Logger {
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

    Logger();
    ~Logger() = default;

    std::string levelToString(level_t level);
    level_t stringToLevel(const std::string &level);
    bool canLog(level_t level);

 public:
    static Logger &get();

    void setOutStream(std::ostream *out);
    void setErrStream(std::ostream *out);
    void setLevel(level_t level);
    void setLevel(const std::string &stringLevel);
    void setVerbosity(level_t verbosity);
    void setDateTimeEnabled(bool enabled);

    /**
     * @param limit 0 means infinite
     */
    void setBufferLimit(std::size_t limit);
    void clear();
    void flush();

    void log(level_t level, const char *tag, const char *message);
    void log(level_t level, const std::string &tag, const std::string &message);
    void log(level_t level, const char *file, int line, const char *tag, const char *message);
    void log(level_t level,
             const std::string &file,
             int line,
             const std::string &tag,
             const std::string &message);

    // ERROR
    void error(const char *file, int line, const char *tag, const char *message);
    void error(const std::string &file, int line, const std::string &tag, const std::string &message);
    void error(const std::string &tag, const std::string &message);
    void error(const char *tag, const char *message);

    // CRITICAL
    void critical(const char *file, int line, const char *tag, const char *message);
    void critical(const std::string &file, int line, const std::string &tag, const std::string &message);
    void critical(const std::string &tag, const std::string &message);
    void critical(const char *tag, const char *message);

    // WARNING
    void warning(const char *file, int line, const char *tag, const char *message);
    void warning(const std::string &file, int line, const std::string &tag, const std::string &message);
    void warning(const std::string &tag, const std::string &message);
    void warning(const char *tag, const char *message);

    // INFO
    void info(const char *file, int line, const char *tag, const char *message);
    void info(const std::string &file, int line, const std::string &tag, const std::string &message);
    void info(const std::string &tag, const std::string &message);
    void info(const char *tag, const char *message);

    // DEBUG
    void debug(const char *file, int line, const char *tag, const char *message);
    void debug(const std::string &file, int line, const std::string &tag, const std::string &message);
    void debug(const std::string &tag, const std::string &message);
    void debug(const char *tag, const char *message);
};

class _TOOLBOXPP_EXPORT Profiler {
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

#endif // TOOLBOXPP_H
