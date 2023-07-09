import os

from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain, CMakeDeps, CMake


def get_version():
    with open(os.path.join(os.path.dirname(__file__), 'version.info'), 'r') as f:
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
    options = {"shared": [True, False]}
    default_options = {
        "shared": False
    }

    exports = "version.info"
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

    def layout(self):
        cmake_layout(self)

    def generate(self):
        # This generates "conan_toolchain.cmake" in self.generators_folder
        tc = CMakeToolchain(self)
        tc.generate()

        # # This generates "foo-config.cmake" and "bar-config.cmake" in self.generators_folder
        deps = CMakeDeps(self)
        deps.generate()

    def build_requirements(self):
        self.test_requires("gtest/1.13.0")

    def build(self):
        cmake = CMake(self)
        opts = {
            'toolbox_BUILD_SHARED_LIBS': "Off",
        }

        if self.options.shared:
            opts['toolbox_BUILD_SHARED_LIBS'] = "On"

        cmake.configure(variables=opts)
        cmake.build(target="toolbox")

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.includedirs = ['include']
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libsdirs = ["lib", "lib/Debug", "lib/Release"]
        self.cpp_info.libs = ["toolbox"]

    def test(self):
        cmake = CMake(self)
        cmake.configure([], {'toolbox_BUILD_TESTS': 'On'})
        cmake.build(target="toolbox-test")
        self.run("bin/toolbox-test")
