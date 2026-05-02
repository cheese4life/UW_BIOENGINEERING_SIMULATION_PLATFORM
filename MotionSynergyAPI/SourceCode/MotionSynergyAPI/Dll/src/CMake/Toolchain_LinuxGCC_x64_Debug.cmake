set(CMAKE_SYSTEM_NAME               "Linux")
set(CMAKE_GENERATOR                 "Unix Makefiles")
set(CMAKE_BUILD_TYPE                "Debug")
set(CMAKE_AR                        "x86_64-linux-gnu-ar")  # with 32-bit/64-bit multilib support
set(CMAKE_C_COMPILER                "x86_64-linux-gnu-gcc")  # with 32-bit/64-bit multilib support
set(CMAKE_CXX_COMPILER              "x86_64-linux-gnu-g++")  # with 32-bit/64-bit multilib support
set(CMAKE_LINKER                    "x86_64-linux-gnu-g++")  # with 32-bit/64-bit multilib support
set(CMAKE_NM                        "x86_64-linux-gnu-nm")  # with 32-bit/64-bit multilib support
set(CMAKE_RANLIB                    "x86_64-linux-gnu-ranlib")  # with 32-bit/64-bit multilib support
set(CMAKE_C_FLAGS_INIT              "-m64")
set(CMAKE_CXX_FLAGS_INIT            "-m64")
set(CMAKE_EXE_LINKER_FLAGS_INIT     "-m64")
set(CMAKE_SHARED_LINKER_FLAGS_INIT  "-m64")
set(CMAKE_MODULE_LINKER_FLAGS_INIT  "-m64")
set(IMP_CCP_PLATFORM                "x64")

set(VCPKG_HOST_TRIPLET              "x64-linux")
set(VCPKG_TARGET_TRIPLET            "x64-linux")
set(VCPKG_MANIFEST_DIR              "${CMAKE_CURRENT_LIST_DIR}/../vcpkg/")

cmake_policy(SET CMP0167 NEW)   # Do not use deprecated FindBoost module.

include(~/vcpkg/scripts/buildsystems/vcpkg.cmake)