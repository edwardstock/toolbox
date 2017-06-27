/**
 * cpphelpers
 * Console.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include "cpphelpers.h"

bool Console::confirm(std::istream &in, std::ostream &out, const std::string &message, bool defValue) {
	std::string res;
	do {
		out << message;
		out << " (yes|no) [" << (defValue ? "yes" : "no") << "]: ";
		std::getline(in, res);

	} while (
		!res.empty() &&
			!Strings::equalsIgnoreCase(res, "yes") &&
			!Strings::equalsIgnoreCase(res, "no") &&
			!Strings::equalsIgnoreCase(res, "y") &&
			!Strings::equalsIgnoreCase(res, "n")
		);

	if (res.empty()) {
		return defValue;
	}

	return Strings::equalsIgnoreCase(res, "yes") || Strings::equalsIgnoreCase(res, "y");
}

std::string Console::prompt(std::istream &in,
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
