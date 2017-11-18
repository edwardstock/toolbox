/**
 * uedit
 * toolboxpp.h
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
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
#define __CHAR_TO_UPPER(c) std::toupper(c)
#endif

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
#include <regex>
#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>
#include <mutex>

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
template<typename K, typename V>
std::vector<K> mapKeysToVector(const std::map<K, V> &map) {
    std::vector<K> v(map.size());
    for (auto &it = map.begin(); it != map.end(); ++it) {
        v.push_back(it->first);
    }

    return v;
}

template<typename K, typename V>
std::vector<V> mapValuesToVector(const std::map<K, V> &map) {
    std::vector<V> v(map.size());

    for (auto &it = map.begin(); it != map.end(); ++it) {
        v.push_back(it->first);
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

namespace numbers {
bool isInteger(const std::string &input);
bool isReal(const std::string &input);
}

namespace fs {
const std::string readFile(const std::string &path);
void writeFile(const std::string &path, const std::string &data);
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
bool hasSubstringIgnoreCase(const_string source, const_string substring);

/**
 * Search substring in string
 * @param source
 * @param substring
 * @return
 */
bool hasSubstring(const_string source, const_string substring);
/**
 * Serch character in string
 * @param source
 * @param substring
 * @return
 */
bool hasSubstring(const_string source, char substring);
/**
 * Match regex pattern
 * @param pattern
 * @param source
 * @return
 */
bool hasRegex(const std::regex &pattern, const_string source);
/**
 * Match regex pattern.
 * @param pattern Just string. By default: search with flag std::regex_constants::icase
 * @param source
 * @return
 */
bool hasRegex(const_string pattern, const_string source);
/**
 * Returns all found groups in source with pattern
 * @param rx custom regex object
 * @param s
 * @return
 */
std::vector<std::vector<std::string>> matchAllRegexp(const std::regex &pattern, const_string source);
/**
 * Returns all found groups in source with pattern
 * @param pattern
 * @param source
 * @return
 */
std::vector<std::vector<std::string>> matchAllRegexp(const_string pattern, const_string source);
/**
 * Take first regex match and return it (with custom regex object)
 * @param pattern
 * @param source
 * @return
 */
std::string matchRegexpFirst(const_string pattern, const_string source);
/**
 * Take first regex match and return it
 * @param pattern
 * @param source
 * @return
 */
std::string matchRegexpFirst(const std::regex &pattern, const_string source);
/**
 * boilerplate to std::regex
 * @param pattern
 * @param source
 * @return
 */
std::smatch matchRegexp(const_string pattern, const_string source);
/**
 * Less boilerplate to std::regex
 * @param rxPattern
 * @param source
 * @return
 */
std::smatch matchRegexp(const std::regex &rxPattern, const_string source);

/**
 * Splits string by delimiter to pair
 * @param source
 * @param delimiter
 * @return
 */
std::pair<std::string, std::string> splitPair(const_string source, const_string delimiter);
/**
 * Splits string by char delimiter to pair
 * @param source
 * @param delimiter
 * @return
 */
std::pair<std::string, std::string> splitPair(const_string source, const char &delimiter);
/**
 * Splits string by char delimiter to vector list
 * @param source
 * @param delimiter
 * @return
 */
std::vector<std::string> split(const_string source, const_string delimiter);
/**
 * Splits string by delimiter to vector list
 * @param source
 * @param delimiter
 * @return
 */
std::vector<std::string> split(const_string source, const char &delimiter);

/**
 * @see substringReplace(const_string search,
									const_string replace,
									const_string source) but mutable behavior
 * @param search
 * @param replace
 * @param source Mutable
 * @return
 */
void replace(const_string search, const_string replace, std::string &source);

/**
 * @see substringReplaceAll(const_string search,
									   const_string replace,
									   const_string source)
 * @param search
 * @param replace
 * @param source Mutable
 */
void replaceAll(const_string search, const_string replace, std::string &source);

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
std::string substringReplace(const_string search,
                             const_string replace,
                             const_string source);
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
                                const_string source);
/**
 * Works like std::string substringReplace(const_string search, const_string replace, const_string source);
 * but replaces all "search" occurrences with "replace" string
 * @param search
 * @param replace
 * @param source
 * @return
 */
std::string substringReplaceAll(const_string search,
                                const_string replace,
                                const_string source);
/**
 * Find "removable" string in "source" and remove it
 * @param source Mutable value!
 * @param removable
 */
void removeSubstrings(std::string &source, const_string removable);
/**
 * Find all "removables" strings in "source" and remove it
 * @param source
 * @param removables
 */
void removeSubstrings(std::string &source, std::vector<std::string> removables);

/**
 *
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
std::string substrInverse(const_string source, char whence);
std::string substrInverse(const_string source, char begin, char end, long offset = 0);
std::string substrInverse(const_string source, const_string begin);
std::string substrInverse(const_string source, const_string begin, const_string end, long offset = 0);

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
std::string clipSubstring(const_string source, const_string search, size_t width, bool icase = false);

/**
 * Concatenates strings by glue
 * @param glue
 * @param strings
 * @return
 */
std::string glue(const_string glue, const std::vector<std::string> &strings);

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
std::string toLower(const_string s);

/**
 * String to upper case
 * @param s
 * @return
 */
std::string toUpper(const_string s);

/**
 *
 * @param s1
 * @param s2
 * @return
 */
bool equalsIgnoreCase(const_string s1, const_string s2);

/**
 * Works like sprintf but with std::string and returns std::string
 * @tparam Args any type
 * @param format format string
 * @param args
 * @return
 */
template<typename... Args>
std::string format(const_string format, Args ...args) {
    auto *tmp = new char[format.length()];

    sprintf(tmp, format.c_str(), args...);

    std::string out = std::string(tmp);
    delete[] tmp;

    return out;
}
}

class _TOOLBOXPP_EXPORT Logger {
 public:
    constexpr const static unsigned short LEVEL_DEBUG = (1 << 0);
    constexpr const static unsigned short LEVEL_INFO = (1 << 1);
    constexpr const static unsigned short LEVEL_WARNING = (1 << 2);
    constexpr const static unsigned short LEVEL_ERROR = (1 << 3);
    constexpr const static unsigned short LEVEL_CRITICAL = (1 << 4);
    constexpr const static unsigned short LEVEL_ALL =
        LEVEL_DEBUG
            | LEVEL_INFO
            | LEVEL_WARNING
            | LEVEL_ERROR
            | LEVEL_CRITICAL;

    Logger(const Logger &copy) = delete;
    Logger(Logger &&copy) = delete;
    Logger &operator=(const Logger &copy) = delete;
    Logger &operator=(Logger &&copy) = delete;
 private:
    typedef std::mutex mutex_t;

    int level = Logger::LEVEL_ALL;
    int bufferLimit = -1;

    std::ostream *outStream;
    std::ostream *errStream;

    mutex_t logLock;

    std::unordered_map<int, std::queue<std::string>> logs;
    std::unordered_map<int, std::string> levelMap = {
        {Logger::LEVEL_DEBUG,    "debug"},
        {Logger::LEVEL_INFO,     "info"},
        {Logger::LEVEL_WARNING,  "warning"},
        {Logger::LEVEL_ERROR,    "error"},
        {Logger::LEVEL_CRITICAL, "critical"},
    };

    Logger();
    ~Logger() = default;

    std::string levelToString(int level);
    bool canLog(int level);

 public:
    static Logger &get();

    void setOutStream(std::ostream *out);
    void setErrStream(std::ostream *out);
    void setLevel(int level);

    /**
     * @param limit -1 means infinite
     */
    void setBufferLimit(int limit);
    void clear();
    void flush();

    void log(int level, const char *tag, const char *message);
    void log(int level, const std::string &tag, const std::string &message);
    void log(int level, const char *file, int line, const char *tag, const char *message);
    void log(int level,
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
    mutex_t lock;

 public:
    static Profiler &get() {
        static Profiler p;
        return p;
    }

    void clear() {
        timings.clear();
    }

    std::size_t size() const {
        return timings.size();
    }

    void begin(const std::string &tag) {
        if (timings.count(tag)) {
            return;
        }

        lock.lock();
        timings[tag] = hires_clock::now();
        lock.unlock();
    }

    void end(const std::string &tag, double *result = nullptr) {
        if (!timings.count(tag)) {
            return;
        }

        lock.lock();
        hires_time_t past = timings[tag];
        hires_time_t now = hires_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(now - past);
        timings.erase(tag);
        lock.unlock();
        if (result != nullptr) {
            *result = time_span.count();
        } else {
            L_DEBUG_F(tag, "Profiling time: %lf ms", time_span.count());
        }

    }
};

}

#endif // TOOLBOXPP_H
