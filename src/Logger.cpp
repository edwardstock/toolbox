/**
 * toolboxpp
 * Logger.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 * @copyright 2017 GNU General Public License v3
 */
#include "toolboxpp.h"

constexpr const toolboxpp::Logger::level_t toolboxpp::Logger::LEVEL_DEBUG;
constexpr const toolboxpp::Logger::level_t toolboxpp::Logger::LEVEL_WARNING;
constexpr const toolboxpp::Logger::level_t toolboxpp::Logger::LEVEL_INFO;
constexpr const toolboxpp::Logger::level_t toolboxpp::Logger::LEVEL_ERROR;
constexpr const toolboxpp::Logger::level_t toolboxpp::Logger::LEVEL_CRITICAL;
constexpr const toolboxpp::Logger::level_t toolboxpp::Logger::LEVEL_ALL;
constexpr const toolboxpp::Logger::level_t toolboxpp::Logger::VERBOSITY_0;
constexpr const toolboxpp::Logger::level_t toolboxpp::Logger::VERBOSITY_1;
constexpr const toolboxpp::Logger::level_t toolboxpp::Logger::VERBOSITY_2;

toolboxpp::Logger::Logger() :
    outStream(&std::cout),
    errStream(&std::cerr) {
}
std::string toolboxpp::Logger::levelToString(level_t level) {
    if (!levelMap.count(level)) {
        return "Unknown error!";
    }

    return levelMap[level];
}
toolboxpp::Logger::level_t toolboxpp::Logger::stringToLevel(const std::string &level) {
    for (auto &t: levelMap) {
        if (toolboxpp::strings::equalsIgnoreCase(t.second, level)) {
            return t.first;
        }
    }

    return LEVEL_ERROR;
}
bool toolboxpp::Logger::canLog(level_t level) {
    return
        (this->level & level) == 0
            || bufferLimit == 0
            || logs[level].size() <= bufferLimit;
}
toolboxpp::Logger &toolboxpp::Logger::get() {
    static Logger logger;
    return logger;
}
void toolboxpp::Logger::setOutStream(std::ostream *out) {
    outStream = out;
}
void toolboxpp::Logger::setErrStream(std::ostream *out) {
    errStream = out;
}
void toolboxpp::Logger::setLevel(level_t level) {
    this->level = level;
}
void toolboxpp::Logger::setLevel(const std::string &stringLevel) {
    this->level = stringToLevel(stringLevel);
}
void toolboxpp::Logger::setVerbosity(toolboxpp::Logger::level_t verbosity) {
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
void toolboxpp::Logger::setBufferLimit(std::size_t limit) {
    bufferLimit = limit;
}
void toolboxpp::Logger::clear() {
    logs.clear();
}
void toolboxpp::Logger::flush() {
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
void toolboxpp::Logger::log(level_t level, const char *tag, const char *message) {
    log(level, std::string(tag), std::string(message));
}
void toolboxpp::Logger::log(level_t level, const std::string &tag, const std::string &message) {
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
void toolboxpp::Logger::log(level_t level, const char *file, int line, const char *tag, const char *message) {
    log(level, std::string(file), line, std::string(tag), std::string(message));
}
void toolboxpp::Logger::log(level_t level,
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
void toolboxpp::Logger::error(const char *file, int line, const char *tag, const char *message) {
    error(std::string(file), line, std::string(tag), std::string(message));
}
void toolboxpp::Logger::error(const std::string &file, int line, const std::string &tag, const std::string &message) {
    log(LEVEL_ERROR, file, line, tag, message);
}
void toolboxpp::Logger::error(const std::string &tag, const std::string &message) {
    log(LEVEL_ERROR, tag, message);
}
void toolboxpp::Logger::error(const char *tag, const char *message) {
    error(std::string(tag), std::string(message));
}
void toolboxpp::Logger::critical(const char *file, int line, const char *tag, const char *message) {
    critical(std::string(file), line, std::string(tag), std::string(message));
}
void toolboxpp::Logger::critical(const std::string &file,
                                 int line,
                                 const std::string &tag,
                                 const std::string &message) {
    log(LEVEL_CRITICAL, file, line, tag, message);
}
void toolboxpp::Logger::critical(const std::string &tag, const std::string &message) {
    log(LEVEL_CRITICAL, tag, message);
}
void toolboxpp::Logger::critical(const char *tag, const char *message) {
    critical(std::string(tag), std::string(message));
}
void toolboxpp::Logger::warning(const char *file, int line, const char *tag, const char *message) {
    warning(std::string(file), line, std::string(tag), std::string(message));
}
void toolboxpp::Logger::warning(const std::string &file, int line, const std::string &tag, const std::string &message) {
    log(LEVEL_WARNING, file, line, tag, message);
}
void toolboxpp::Logger::warning(const std::string &tag, const std::string &message) {
    log(LEVEL_WARNING, tag, message);
}
void toolboxpp::Logger::warning(const char *tag, const char *message) {
    warning(std::string(tag), std::string(message));
}
void toolboxpp::Logger::info(const char *file, int line, const char *tag, const char *message) {
    info(std::string(file), line, std::string(tag), std::string(message));
}
void toolboxpp::Logger::info(const std::string &file, int line, const std::string &tag, const std::string &message) {
    log(LEVEL_INFO, file, line, tag, message);
}
void toolboxpp::Logger::info(const std::string &tag, const std::string &message) {
    log(LEVEL_INFO, tag, message);
}
void toolboxpp::Logger::info(const char *tag, const char *message) {
    info(std::string(tag), std::string(message));
}
void toolboxpp::Logger::debug(const char *file, int line, const char *tag, const char *message) {
    debug(std::string(file), line, std::string(tag), std::string(message));
}
void toolboxpp::Logger::debug(const std::string &file, int line, const std::string &tag, const std::string &message) {
    log(LEVEL_DEBUG, file, line, tag, message);
}
void toolboxpp::Logger::debug(const std::string &tag, const std::string &message) {
    log(LEVEL_DEBUG, tag, message);
}
void toolboxpp::Logger::debug(const char *tag, const char *message) {
    debug(std::string(tag), std::string(message));
}
void toolboxpp::Logger::setDateTimeEnabled(bool enabled) {
    printDateTime = enabled;
}


