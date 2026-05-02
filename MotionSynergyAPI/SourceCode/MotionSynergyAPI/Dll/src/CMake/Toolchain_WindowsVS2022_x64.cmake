set(CMAKE_SYSTEM_NAME                           "Windows")
set(CMAKE_SYSTEM_VERSION                        "10.0.22000.0" CACHE STRING INTERNAL FORCE)
set(CMAKE_GENERATOR                             "Visual Studio 17 2022")
set(CMAKE_GENERATOR_PLATFORM                    "x64")
set(IMP_CCP_PLATFORM                            "x64")

set(VCPKG_HOST_TRIPLET                          "x64-windows-static-md")
set(VCPKG_TARGET_TRIPLET                        "x64-windows-static-md")
set(VCPKG_MANIFEST_DIR                          "${CMAKE_CURRENT_LIST_DIR}/../vcpkg/")

cmake_policy(SET CMP0167 NEW)   # Do not use deprecated FindBoost module.

include($ENV{PROGRAMDATA}/vcpkg/scripts/buildsystems/vcpkg.cmake)