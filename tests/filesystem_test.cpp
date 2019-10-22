/*!
 * toolboxpp. 2017
 *
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#include <cstdio>
#include "gtest/gtest.h"
#include "toolboxpp.hpp"
#include <stdlib.h>

using namespace toolboxpp::fs;

TEST(FileSystem, ReadWriteExistCheck) {
    char *tmpPath = std::getenv("TMP");
    std::string tmp(tmpPath == nullptr ? "" : tmpPath);
    if (tmp.empty()) {
        tmp = "/tmp";
    }
    #ifdef _MSC_VER
    const std::string ds = "\\";
    #else
    const std::string ds = "/";
    #endif

    std::string targetPath = tmp + ds + "test_file.txt";

    bool written = writeFile(targetPath, "CONTENT");
    ASSERT_TRUE(written);

    bool exist = exists(targetPath);
    ASSERT_TRUE(exist);

    std::string content = readFile(targetPath);
    ASSERT_STREQ(content.c_str(), "CONTENT");

    remove(targetPath.c_str());
    ASSERT_FALSE(exists(targetPath));

}
