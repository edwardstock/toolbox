/**
 * toolboxpp
 * FileSystem.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#include <sys/stat.h>
#include "toolboxpp.h"
#include <cerrno>
#include <cstring>
#include <cstdio>

const std::string toolboxpp::fs::readFile(const std::string &path) {
    std::ifstream input(path);

    if (!input.is_open()) {
        perror(strerror(errno));
        return "";
    }

    const std::string out = toolboxpp::strings::toString(input);
    input.close();
    return out;
}

bool toolboxpp::fs::writeFile(const std::string &path, const std::string &data) {
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

bool toolboxpp::fs::exists(const std::string &path) {
    struct stat s;
    return stat(path.c_str(), &s) == 0;
}

