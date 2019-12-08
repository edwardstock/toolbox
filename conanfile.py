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


class ToolboxConan(ConanFile):
    name = "toolbox"
    version = get_version()
    license = "MIT"
    author = "Eduard Maximovich edward.vstock@gmail.com"
    url = "https://github.com/edwardstock/toolbox"
    description = "Lightweight everyday C++ helpers"
    topics = ("cpp-helpers", "helpers")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports = "version"
    exports_sources = (
        "cfg/*",
        "modules/*",
        "options.cmake",
        "include/*",
        "tests/*",
        "src/*",
        "CMakeLists.txt",
        "conanfile.py",
        "LICENSE",
        "README.md"
    )
    build_requires = (
        "gtest/1.8.1@bincrafters/stable"
    )

    def source(self):
        if "CONAN_LOCAL" not in os.environ:
            git = tools.Git(folder="toolbox")
            git.clone("https://github.com/edwardstock/toolbox.git", "master")

    def build(self):
        cmake = CMake(self)
        cmake.configure(defs={'CMAKE_BUILD_TYPE': self.options["build_type"]})
        cmake.build(target="toolbox")

    def package(self):
        self.copy("*", dst="include", src="include", keep_path=True)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="lib", keep_path=False)
        self.copy("*.dll.a", dst="lib", keep_path=False)
        self.copy("*.exp", dst="lib", keep_path=False)
        self.copy("*.ilk", dst="lib", keep_path=False)
        self.copy("*.pdb", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = self.collect_libs(folder="lib")

    def test(self):
        cmake = CMake(self)
        cmake.configure([], {'ENABLE_TOOLBOX_TEST': 'On'})
        cmake.build([], None, "toolbox-test")
        self.run("bin/toolbox-test")
