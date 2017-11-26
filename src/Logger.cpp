/**
 * toolboxpp
 * Logger.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 * @copyright 2017 GNU General Public License v3
 */
#include "toolboxpp.h"

constexpr const unsigned short toolboxpp::Logger::LEVEL_DEBUG;
constexpr const unsigned short toolboxpp::Logger::LEVEL_WARNING;
constexpr const unsigned short toolboxpp::Logger::LEVEL_INFO;
constexpr const unsigned short toolboxpp::Logger::LEVEL_ERROR;
constexpr const unsigned short toolboxpp::Logger::LEVEL_CRITICAL;
constexpr const unsigned short toolboxpp::Logger::LEVEL_ALL;

toolboxpp::Logger::Logger() :
    outStream(&std::cout),
    errStream(&std::cerr) {
}
std::string toolboxpp::Logger::levelToString(int level) {
    if (!levelMap.count(level)) {
        return "Unknown error!";
    }

    return levelMap[level];
}
bool toolboxpp::Logger::canLog(int level) {
    return not((this->level & level) == 0 || (bufferLimit > -1 && logs[level].size() >= bufferLimit));
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
void toolboxpp::Logger::setLevel(int level) {
    this->level = level;
}
void toolboxpp::Logger::setBufferLimit(int limit) {
    bufferLimit = limit;
}
void toolboxpp::Logger::clear() {
    logs.clear();
}
void toolboxpp::Logger::flush() {
    logLock.lock();
    for (auto &levels: logs) {
        while (!levels.second.empty()) {
            if (levels.first > LEVEL_INFO) {
                *errStream << "[" << levelToString(levels.first) << "] " << levels.second.front() << std::endl;
            } else {
                *outStream << "[" << levelToString(levels.first) << "] " << levels.second.front() << std::endl;
            }

            levels.second.pop();
        }
    }
    logLock.unlock();
}
void toolboxpp::Logger::log(int level, const char *tag, const char *message) {
    log(level, std::string(tag), std::string(message));
}
void toolboxpp::Logger::log(int level, const std::string &tag, const std::string &message) {
    if (!canLog(level)) {
        return;
    }
    std::lock_guard<mutex_t> locker(logLock);
    std::string out = tag + ": " + message;
    logs[level].push(out);
    #ifndef L_DISABLE_AUTO_FLUSH
    flush();
    #endif
}
void toolboxpp::Logger::log(int level, const char *file, int line, const char *tag, const char *message) {
    log(level, std::string(file), line, std::string(tag), std::string(message));
}
void toolboxpp::Logger::log(int level,
                            const std::string &file,
                            int line,
                            const std::string &tag,
                            const std::string &message) {

    if (!canLog(level)) {
        return;
    }

    const size_t bufferSize = file.length() + sizeof(line) + tag.length() + message.length() + 512;
    char buffer[bufferSize];

    if (level < LEVEL_ERROR) {
        sprintf(buffer, "%s: %s", tag.c_str(), message.c_str());
    } else {
        sprintf(buffer, "%s: %s \n\tSource: %s:%d", tag.c_str(), message.c_str(), file.c_str(), line);
    }

    const std::string out(buffer);

    std::lock_guard<mutex_t> locker(logLock);
    logs[level].push(std::string(buffer));

    #ifndef L_DISABLE_AUTO_FLUSH
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


