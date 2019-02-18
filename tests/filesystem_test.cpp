/*!
 * toolboxpp. 2017
 *
 * \author Eduard Maximovich <edward.vstock@gmail.com>
 * \link https://github.com/edwardstock
 */

#include <cstdio>
#include "gtest/gtest.h"
#include "toolboxpp.hpp"

using namespace toolboxpp::fs;

TEST(FileSystem, ReadWriteExistCheck) {
    bool written = writeFile("/tmp/test_file.txt", "CONTENT");
    ASSERT_TRUE(written);

    bool exist = exists("/tmp/test_file.txt");
    ASSERT_TRUE(exist);

    std::string content = readFile("/tmp/test_file.txt");
    ASSERT_STREQ(content.c_str(), "CONTENT");

    remove("/tmp/test_file.txt");
    ASSERT_FALSE(exists("/tmp/test_file.txt"));

}
