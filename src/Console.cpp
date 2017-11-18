/**
 * toolboxpp
 * Console.cpp
 *
 * @author Eduard Maximovich <edward.vstock@gmail.com>
 * @link https://github.com/edwardstock
 */
#include "toolboxpp.h"

bool toolboxpp::console::confirm(std::istream &in, std::ostream &out, const std::string &message, bool defValue) {
	std::string res;
	do {
		out << message;
		out << " (yes|no) [" << (defValue ? "yes" : "no") << "]: ";
		std::getline(in, res);

	} while (
		!res.empty() &&
			!toolboxpp::strings::equalsIgnoreCase(res, "yes") &&
			!toolboxpp::strings::equalsIgnoreCase(res, "no") &&
			!toolboxpp::strings::equalsIgnoreCase(res, "y") &&
			!toolboxpp::strings::equalsIgnoreCase(res, "n")
		);

	if (res.empty()) {
		return defValue;
	}

	return toolboxpp::strings::equalsIgnoreCase(res, "yes") || toolboxpp::strings::equalsIgnoreCase(res, "y");
}

bool toolboxpp::console::confirm(const std::string &message, bool defValue) {
	return confirm(std::cin, std::cout, message, defValue);
}

std::string toolboxpp::console::prompt(std::istream &in,
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
std::string toolboxpp::console::prompt(const std::string &message, bool required, const std::string &defValue) {
	return prompt(std::cin, std::cout, message, required, defValue);
}

