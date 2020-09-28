/*!
 * toolbox.
 * decimal_formatter.cpp
 *
 * \date 11/30/2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include "toolbox/strings/decimal_formatter.h"

#include "toolbox/strings.hpp"

#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
toolbox::strings::decimal_formatter::decimal_formatter(int num) {
    std::stringstream ss;
    ss << num;
    m_num = ss.str();
}
toolbox::strings::decimal_formatter::decimal_formatter(double num) {
    std::stringstream ss;
    ss << num;
    m_num = ss.str();
}
toolbox::strings::decimal_formatter::decimal_formatter(float num) {
    std::stringstream ss;
    ss << num;
    m_num = ss.str();
}
toolbox::strings::decimal_formatter::decimal_formatter(std::string num)
    : m_num(std::move(num)) {
}
toolbox::strings::decimal_formatter::decimal_formatter(const char* num)
    : m_num(std::string(num)) {
}
std::string toolbox::strings::decimal_formatter::operator()(const std::string& num) {
    m_num = num;
    return format();
}
toolbox::strings::decimal_formatter& toolbox::strings::decimal_formatter::set_delimiter(const std::string& delimiter) {
    m_delimiter = delimiter;
    return *this;
}
toolbox::strings::decimal_formatter& toolbox::strings::decimal_formatter::set_delimiter(char delimiter) {
    std::stringstream ss;
    ss << delimiter;
    m_delimiter = ss.str();
    return *this;
}
toolbox::strings::decimal_formatter& toolbox::strings::decimal_formatter::set_decimals_group(size_t num) {
    m_decimals = num;
    return *this;
}
toolbox::strings::decimal_formatter& toolbox::strings::decimal_formatter::set_max_fractions(size_t max_fractions) {
    m_max_fractions = max_fractions;
    return *this;
}
toolbox::strings::decimal_formatter& toolbox::strings::decimal_formatter::set_min_fractions(size_t min_fractions) {
    m_min_fractions = min_fractions;
    return *this;
}
std::string toolbox::strings::decimal_formatter::format() const {
    if (m_num.empty()) {
        throw std::runtime_error("Empty number passed to decimal formatter");
    }
    std::pair<std::string, std::string> lr = toolbox::strings::split_pair(m_num, '.');

    std::stringstream out;
    std::stack<std::string> parts;

    if (m_min_fractions > 0) {
        size_t fr_size = lr.second.size();
        // if fractial size less than min required fractions, adding zeroes to end
        if (fr_size < m_min_fractions) {
            parts.push(toolbox::strings::repeat('0', m_min_fractions - fr_size));
            parts.push(lr.second);
        } else if (fr_size >= m_min_fractions) {
            if (fr_size > m_max_fractions) {
                parts.push(lr.second.substr(0, m_max_fractions));
            } else {
                parts.push(lr.second);
            }
        }

        parts.push(".");
    }

    size_t prev_i;
    for (size_t i = lr.first.size(); i > 0;) {
        prev_i = i;
        if (m_decimals >= i) {
            i = 0;
        } else {
            i -= m_decimals;
        }

        size_t len = (i == 0) ? prev_i : m_decimals;
        std::string part = lr.first.substr(i, len);

        parts.push(std::move(part));
        if (i != 0) {
            parts.push(std::string(m_delimiter));
        }
    }

    while (!parts.empty()) {
        out << parts.top();
        parts.pop();
    }

    return out.str();
}

std::ostream& operator<<(std::ostream& os, const toolbox::strings::decimal_formatter& formatter) {
    os << formatter.format();
    return os;
}
