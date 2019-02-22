from conans import ConanFile, CMake, tools


class ToolboxppConan(ConanFile):
    name = "toolboxpp"
    version = "2.0.0"
    # exports_sources = "include/*"
    license = "MIT"
    author = "Eduard Maximovich edward.vstock@gmail.com"
    url = "https://github.com/edwardstock/toolboxpp"
    description = "Lightweight everyday C++ helpers"
    topics = ("cpp-helpers", "helpers", "header-only", "lightweight")
    generators = "cmake"
    exports_sources = "*"
    no_copy_source = True

    def source(self):
        git = tools.Git(folder="toolboxpp")
        git.clone("https://github.com/edwardstock/toolboxpp.git", "master")

    def build(self):
        self.run("mkdir -p _build && cd _build")
        cmake = CMake(self)
        cmake.configure([], {'WITH_TEST': 'On'})
        cmake.build([], None, "toolboxtest")
        self.run("toolboxtest")

    def package(self):
        self.copy("*.hpp", dst="include", src="include")

    def package_id(self):
        self.info.header_only()
