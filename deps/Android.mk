
# auto-generated by cppbuild
PROJECT_ROOT ?= /datavol/home/user/work/multiindex_opq
CPPBUILD_BUILD_TYPE ?= release

# Architecture defines
ifeq ($(TARGET_ARCH_ABI), arm64-v8a)
arch := aarch64
else ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
arch := armv7
else ifeq ($(TARGET_ARCH_ABI), armeabi)
arch := armv7
else
arch := $(TARGET_ARCH_ABI)
endif

