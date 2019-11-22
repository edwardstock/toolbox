import os
from conans import ConanFile, CMake, tools

def get_version():
    with open(os.path.join(os.path.dirname(__file__), 'version'), 'r') as f:
        content = f.read()
        try:
            content = content.decode()
        except AttributeError:
            pass
        return content.strip()

class ToolboxppConan(ConanFile):
    name = "toolboxpp"
    version = get_version()
    license = "MIT"
    author = "Eduard Maximovich edward.vstock@gmail.com"
    url = "https://github.com/edwardstock/toolboxpp"
    description = "Lightweight everyday C++ helpers"
    topics = ("cpp-helpers", "helpers", "header-only", "lightweight")
    generators = "cmake"
    exports_sources = (
        "cmakes/*",
        "options.cmake",
        "include/*",
        "tests/*",
        "src/*",
        "CMakeLists.txt",
        "conanfile.py",
        "conanfile.txt",
        "LICENSE.md",
        "README.md"
    )
    no_copy_source = True
    build_requires = (
        "gtest/1.8.1@bincrafters/stable"
    )

    def source(self):
        if "CONAN_LOCAL" not in os.environ:
            git = tools.Git(folder="toolboxpp")
            git.clone("https://github.com/edwardstock/toolboxpp.git", "master")

    def package(self):
        self.copy("*.hpp", dst="include", src="include")

    def package_id(self):
        self.info.header_only()

    def test(self):
        cmake = CMake(self)
        cmake.configure([], {'WITH_TEST': 'On'})
        cmake.build([], None, "toolboxtest")
        self.run("bin/toolboxtest")
