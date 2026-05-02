set(CMAKE_SYSTEM_NAME               "Linux")
set(CMAKE_GENERATOR                 "Unix Makefiles")
set(CMAKE_BUILD_TYPE                "Release")
set(CMAKE_AR                        "$ENV{LINARO_ARM64_PATH}/bin/aarch64-linux-gnu-ar")
set(CMAKE_C_COMPILER                "$ENV{LINARO_ARM64_PATH}/bin/aarch64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER              "$ENV{LINARO_ARM64_PATH}/bin/aarch64-linux-gnu-g++")
set(CMAKE_LINKER                    "$ENV{LINARO_ARM64_PATH}/bin/aarch64-linux-gnu-g++")
set(CMAKE_NM                        "$ENV{LINARO_ARM64_PATH}/bin/aarch64-linux-gnu-nm")
set(CMAKE_RANLIB                    "$ENV{LINARO_ARM64_PATH}/bin/aarch64-linux-gnu-ranlib")
set(IMP_CCP_PLATFORM                "arm64")

set(VCPKG_HOST_TRIPLET              "x64-linux-release")
set(VCPKG_TARGET_TRIPLET            "arm64-linux-release")
set(VCPKG_MANIFEST_DIR              "${CMAKE_CURRENT_LIST_DIR}/../vcpkg/")

cmake_policy(SET CMP0167 NEW)   # Do not use deprecated FindBoost module.

include(~/vcpkg/scripts/buildsystems/vcpkg.cmake)