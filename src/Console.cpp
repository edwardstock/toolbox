/**
 * cpphelpers
 * Console.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include "cpphelpers.h"

bool cpphelpers::console::confirm(std::istream &in, std::ostream &out, const std::string &message, bool defValue) {
	std::string res;
	do {
		out << message;
		out << " (yes|no) [" << (defValue ? "yes" : "no") << "]: ";
		std::getline(in, res);

	} while (
		!res.empty() &&
			!cpphelpers::strings::equalsIgnoreCase(res, "yes") &&
			!cpphelpers::strings::equalsIgnoreCase(res, "no") &&
			!cpphelpers::strings::equalsIgnoreCase(res, "y") &&
			!cpphelpers::strings::equalsIgnoreCase(res, "n")
		);

	if (res.empty()) {
		return defValue;
	}

	return cpphelpers::strings::equalsIgnoreCase(res, "yes") || cpphelpers::strings::equalsIgnoreCase(res, "y");
}

std::string cpphelpers::console::prompt(std::istream &in,
                            std::ostream &out,
                            const std::string &message,
                            bool required,
                            const std::string &defValue) {
	std::string res;
	do {
		out << message;
		if (!defValue.empty()) {
			out << " [" << defValue << "]: ";
		} else {
			out << ": ";
		}

		std::getline(in, res);

	} while ((required && res.empty()));

	if (res.empty()) {
		return defValue;
	}
	return res;
}
