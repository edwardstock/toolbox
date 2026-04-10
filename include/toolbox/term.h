/// @file term.h
/// @brief Interactive terminal helpers: confirm, prompt, password input.

#ifndef TOOLBOXPP_TERM_H
#define TOOLBOXPP_TERM_H

#include "toolbox/toolbox_config.h"

#include <iostream>
#include <string>

// LCOV_EXCL_START
namespace toolbox {
namespace term {

#if defined(HAVE_TERMIOS_H) && defined(HAVE_UNISTD_H)
/// Low-level password reader (POSIX only).
/// @param message prompt shown to the user
/// @param pw pointer to the output buffer (caller-allocated)
/// @param sz buffer size
/// @param mask character used to echo input (0 for no echo)
/// @param fp input stream (default: stdin)
/// @return number of characters read, or -1 on error
TOOLBOX_API ssize_t _password(const std::string& message, char** pw, size_t sz, int mask = 0, FILE* fp = stdin);

/// Prompt the user for a password without echoing it.
/// @param message prompt text
/// @param passSize maximum password length
/// @return entered password
TOOLBOX_API std::string prompt_password(const std::string& message, size_t passSize = 32);
#endif

/// Ask the user a yes/no question.
/// @param in input stream
/// @param out output stream
/// @param message question text
/// @param defValue default answer when the user presses Enter
/// @return true for yes, false for no
TOOLBOX_API bool confirm(std::istream& in, std::ostream& out, const std::string& message, bool defValue = false);

/// Ask the user a yes/no question (uses stdin/stdout).
/// @param message question text
/// @param defValue default answer when the user presses Enter
/// @return true for yes, false for no
TOOLBOX_API bool confirm(const std::string& message, bool defValue = false);

/// Prompt the user for text input.
/// @param in input stream
/// @param out output stream
/// @param message prompt text
/// @param required if true, the user must enter a non-empty value
/// @param defValue default value shown and used if the user presses Enter
/// @return entered text
TOOLBOX_API std::string prompt(
    std::istream& in,
    std::ostream& out,
    const std::string& message,
    bool required = false,
    const std::string& defValue = ""
);

/// Prompt the user for text input (uses stdin/stdout).
/// @param message prompt text
/// @param required if true, the user must enter a non-empty value
/// @param defValue default value shown and used if the user presses Enter
/// @return entered text
TOOLBOX_API std::string prompt(const std::string& message, bool required = false, const std::string& defValue = "");

} // namespace term
} // namespace toolbox
// LCOV_EXCL_STOP

#endif // TOOLBOXPP_TERM_H
