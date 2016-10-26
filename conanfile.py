from conans import ConanFile, CMake

class FeatureDBConan(ConanFile):
    name = "SenseDev"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    requires = "gtest/1.7.0@lasote/stable", \
            "BoostParts/0.1@ruanjiabin/stable", \
            "Eigen3/3.2.8@bilke/stable", \
            "libmongoc/1.4.0@ruanjiabin/stable", \
            "SenseDev/1.0.0@ruanjiabin/testing"
    options = {"shared": [True, False]}
    default_options = "shared=False"

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake "%s" %s' % (self.conanfile_directory, cmake.command_line))
        self.run('cmake --build . %s' % cmake.build_config)
