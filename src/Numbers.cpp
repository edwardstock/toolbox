/**
 * toolboxpp
 * NumberUtils.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include "toolboxpp.h"
#ifdef HAVE_REGEX_H
bool toolboxpp::numbers::isInteger(const std::string &input) {
    return std::regex_match(input, std::regex(R"(^[-]?[0-9eE+]+?$)"));
}

bool toolboxpp::numbers::isReal(const std::string &input) {
    return std::regex_match(input, std::regex(R"(^[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$)"));
}
#endif