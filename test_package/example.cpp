#include <iostream>
#include <toolboxpp.hpp>

int main() {
    std::cout << toolboxpp::strings::format("ABC Hello%s", "!") << std::endl;
}
