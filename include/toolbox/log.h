
/// @file log.h
/// @brief Thread-safe singleton logger with severity levels and macro shortcuts.

#ifndef TOOLBOXPP_LOG_H
#define TOOLBOXPP_LOG_H

#include "toolbox/toolbox_config.h"

#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <cstdio>
#include <unordered_map>

namespace toolbox {

/// Bitmask type for log severity levels.
using level_t = uint16_t;
constexpr const static level_t LEVEL_DEBUG = (1u << 0u);
constexpr const static level_t LEVEL_WARNING = (1u << 1u);
constexpr const static level_t LEVEL_INFO = (1u << 2u);
constexpr const static level_t LEVEL_ERROR = (1u << 3u);
constexpr const static level_t LEVEL_CRITICAL = (1u << 4u);
constexpr const static level_t LEVEL_ALL = LEVEL_DEBUG | LEVEL_WARNING | LEVEL_INFO | LEVEL_ERROR | LEVEL_CRITICAL;

/// Verbosity presets: VERBOSITY_0 shows only errors/critical,
/// VERBOSITY_1 adds info, VERBOSITY_2 shows everything.
constexpr const static level_t VERBOSITY_0 = LEVEL_CRITICAL | LEVEL_ERROR;
constexpr const static level_t VERBOSITY_1 = VERBOSITY_0 | LEVEL_INFO;
constexpr const static level_t VERBOSITY_2 = LEVEL_ALL;

/// Thread-safe singleton logger.
///
/// Messages are written to configurable output streams (stdout/stderr by default) and can optionally be buffered.
/// Use the L_DEBUG, L_INFO, L_WARN, L_ERR, L_CRIT macros for convenience.
class TOOLBOX_API tlog {
public:
    tlog(const tlog& copy) = delete;
    tlog(tlog&& copy) = delete;
    tlog& operator=(const tlog& copy) = delete;
    tlog& operator=(tlog&& copy) = delete;

private:
    typedef std::recursive_mutex mutex_t;

    level_t m_level = LEVEL_ALL;
    std::size_t m_buffer_limit = 0;
    bool m_print_datetime = true;

    std::ostream* out_stream;
    std::ostream* err_stream;
    mutex_t log_mutex;

    std::unordered_map<level_t, std::queue<std::string>> logs;
    std::unordered_map<level_t, std::string> levelMap = {
        {LEVEL_DEBUG, "debug"},
        {LEVEL_WARNING, "warning"},
        {LEVEL_INFO, "info"},
        {LEVEL_ERROR, "error"},
        {LEVEL_CRITICAL, "critical"},
    };

    tlog();
    ~tlog() = default;

    std::string level_to_string(level_t level);
    level_t string_to_level(const std::string& level);
    bool can_log(level_t level);

public:
    /// Return the global logger instance.
    static tlog& get() {
        static tlog logger;
        return logger;
    }

    /// Set the output stream for info/debug/warning messages (default: std::cout).
    void set_out_stream(std::ostream* out);
    /// Set the output stream for error/critical messages (default: std::cerr).
    void set_err_stream(std::ostream* out);
    /// Set allowed severity levels as a bitmask (e.g. LEVEL_DEBUG | LEVEL_INFO).
    void set_level(level_t level);
    /// Set allowed severity level by name ("debug", "info", "warning", "error", "critical").
    void set_level(const std::string& level_s);
    /// Set a verbosity preset (VERBOSITY_0, VERBOSITY_1, VERBOSITY_2).
    void set_verbosity(level_t verbosity);
    /// Enable or disable the datetime prefix on each log line.
    void set_datetime_enable(bool enabled);

    /// Set the maximum number of buffered messages per level.
    /// @param limit 0 means unlimited
    void set_buffer_limit(std::size_t limit);
    /// Clear all buffered messages.
    void clear();
    /// Flush all buffered messages to their respective output streams.
    void flush();

    /// @name Generic log methods
    /// @{
    void log(level_t level, const char* tag, const char* message);
    void log(level_t level, const std::string& tag, const std::string& message);
    void log(level_t level, const char* file, int line, const char* tag, const char* message);
    void log(level_t level, const std::string& file, int line, const std::string& tag, const std::string& message);
    /// @}

    /// @name Shorthand log methods by severity
    /// @{

    // ERROR
    void e(const char* file, int line, const char* tag, const char* message);
    void e(const std::string& file, int line, const std::string& tag, const std::string& message);
    void e(const std::string& tag, const std::string& message);
    void e(const char* tag, const char* message);

    // CRITICAL
    void crit(const char* file, int line, const char* tag, const char* message);
    void crit(const std::string& file, int line, const std::string& tag, const std::string& message);
    void crit(const std::string& tag, const std::string& message);
    void crit(const char* tag, const char* message);

    // WARNING
    void w(const char* file, int line, const char* tag, const char* message);
    void w(const std::string& file, int line, const std::string& tag, const std::string& message);
    void w(const std::string& tag, const std::string& message);
    void w(const char* tag, const char* message);

    // INFO
    void i(const char* file, int line, const char* tag, const char* message);
    void i(const std::string& file, int line, const std::string& tag, const std::string& message);
    void i(const std::string& tag, const std::string& message);
    void i(const char* tag, const char* message);

    // DEBUG
    void d(const char* file, int line, const char* tag, const char* message);
    void d(const std::string& file, int line, const std::string& tag, const std::string& message);
    void d(const std::string& tag, const std::string& message);
    void d(const char* tag, const char* message);
    /// @}
};

} // namespace toolbox

/// @name Logging macros
/// Each macro captures __FILE__ and __LINE__ automatically.
/// @{

#define L_LEVEL(level) toolbox::tlog::get().set_level(level)

#define L_DEBUG(tag, message) toolbox::tlog::get().d(__FILE__, __LINE__, tag, message);
#define L_DEBUG_F(tag, message, ...)                                 \
    {                                                                \
        char __tmp_buff[512];                                        \
        std::snprintf(__tmp_buff, 512, message, __VA_ARGS__);                   \
        toolbox::tlog::get().d(__FILE__, __LINE__, tag, __tmp_buff); \
    }

#define L_INFO(tag, message) toolbox::tlog::get().i(__FILE__, __LINE__, tag, message);
#define L_INFO_F(tag, message, ...)                                  \
    {                                                                \
        char __tmp_buff[512];                                        \
        std::snprintf(__tmp_buff, 512, message, __VA_ARGS__);                   \
        toolbox::tlog::get().i(__FILE__, __LINE__, tag, __tmp_buff); \
    }

#define L_WARN(tag, message) toolbox::tlog::get().w(__FILE__, __LINE__, tag, message);
#define L_WARN_F(tag, message, ...)                                  \
    {                                                                \
        char __tmp_buff[512];                                        \
        std::snprintf(__tmp_buff, 512, message, __VA_ARGS__);                   \
        toolbox::tlog::get().w(__FILE__, __LINE__, tag, __tmp_buff); \
    }
#define L_ERR(tag, message) toolbox::tlog::get().e(__FILE__, __LINE__, tag, message);
#define L_ERR_F(tag, message, ...)                                   \
    {                                                                \
        char __tmp_buff[512];                                        \
        std::snprintf(__tmp_buff, 512, message, __VA_ARGS__);                   \
        toolbox::tlog::get().e(__FILE__, __LINE__, tag, __tmp_buff); \
    }
#define L_CRIT(tag, message) toolbox::tlog::get().crit(__FILE__, __LINE__, tag, message);
#define L_CRIT_F(tag, message, ...)                                     \
    {                                                                   \
        char __tmp_buff[512];                                           \
        std::snprintf(__tmp_buff, 512, message, __VA_ARGS__);                      \
        toolbox::tlog::get().crit(__FILE__, __LINE__, tag, __tmp_buff); \
    }

#ifdef _MSC_VER
#define L_TRAP(tag, message) \
    L_CRIT(tag, message)     \
    __debugbreak()
#else
#include <csignal>
#define L_TRAP(tag, message) \
    L_CRIT(tag, message);    \
    raise(SIGABRT)
#endif

#define L_FLUSH() toolbox::tlog::get().flush()

#endif // TOOLBOXPP_LOG_H
