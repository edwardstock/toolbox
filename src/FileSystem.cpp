/**
 * cpphelpers
 * FileSystem.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */

#include <sys/stat.h>
#include "../include/cpphelpers.h"
#include <cerrno>
#include <cstring>
#include <cstdio>

const std::string cpphelpers::fs::readFile(const std::string &path) {
	std::ifstream input(path);

	if (!input.is_open()) {
		perror(strerror(errno));
		return "";
	}

	const std::string out = cpphelpers::strings::toString(input);
	input.close();
	return out;
}

void cpphelpers::fs::writeFile(const std::string &path, const std::string &data) {
	std::ofstream out(path);
	if (!out.is_open()) {
		perror(strerror(errno));
		return;
	}

	out << data;
	out.flush();
	out.close();
}

bool cpphelpers::fs::exists(const std::string &path) {
	struct stat s;
	return stat(path.c_str(), &s) == 0;
}

