# First pull luafilesystem from github

# To update to a later version of the luafilesystem port, update the following lines from the later version of the luafilesystem portfile.cmake: 
set(LUAFILESYSTEM_VERSION 1.8.0)
set(LUAFILESYSTEM_REVISION v1_8_0)
set(LUAFILESYSTEM_HASH 79d964f13ae43716281dc8521d2f128b22f2261234c443e242b857cfdf621e208bdf4512f8ba710baa113e9b3b71e2544609de65e2c483f569c243a5cf058247)

# Note the patch fix-luafilesystem-warnings.patch is applied to fix some compiler warnings in luafilesystem. This may not be needed in later releases.

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO keplerproject/luafilesystem
    REF ${LUAFILESYSTEM_REVISION}
    SHA512 ${LUAFILESYSTEM_HASH}
    HEAD_REF master
    PATCHES fix-luafilesystem-warnings.patch
)

set(LUAFILESYSTEM_SOURCE_PATH "${SOURCE_PATH}/src")

# Now pull lua from lua.org

vcpkg_download_distfile(ARCHIVE
    URLS "https://www.lua.org/ftp/lua-${VERSION}.tar.gz"
    FILENAME "lua-${VERSION}.tar.gz"
    SHA512 d90c6903355ee1309cb0d92a8a024522ff049091a117ea21efb585b5de35776191cd67d17a65b18c2f9d374795b7c944f047576f0e3fe818d094b26f0e4845c5
)

# Note the additional patch add-luafilesystem-support.patch is applied to linit.c to add the lfs library to the standard set of lua libraries used by lua.exe.

vcpkg_extract_source_archive(
    SOURCE_PATH
    ARCHIVE "${ARCHIVE}"
    PATCHES
        vs2015-impl-c99.patch
        fix-ios-system.patch
        add-luafilesystem-support.patch
)

# Copy the luafilesystem sources files from the luafilesystem port source directory to this source directory

file(COPY "${LUAFILESYSTEM_SOURCE_PATH}/lfs.h" DESTINATION "${SOURCE_PATH}/src")
file(COPY "${LUAFILESYSTEM_SOURCE_PATH}/lfs.c" DESTINATION "${SOURCE_PATH}/src")

file(COPY "${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt" DESTINATION "${SOURCE_PATH}")

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
         ${FEATURE_OPTIONS}
)

set(VCPKG_POLICY_EMPTY_INCLUDE_FOLDER enabled)  # Only installing the executable, no headers.

vcpkg_cmake_install()

vcpkg_copy_pdbs()

vcpkg_copy_tools(TOOL_NAMES lua SEARCH_DIR "${CURRENT_PACKAGES_DIR}/tools/${PORT}")

# Suitable for old version
configure_file("${CMAKE_CURRENT_LIST_DIR}/vcpkg-cmake-wrapper.cmake.in"  "${CURRENT_PACKAGES_DIR}/share/${PORT}/vcpkg-cmake-wrapper.cmake" @ONLY)
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")

file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/COPYRIGHT" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
