#include <cassert>
#include <toolbox/strings.hpp>

int main() {
    bool ret = toolbox::strings::has_substring_icase("w", "hello world");
    assert(ret == true);
}
