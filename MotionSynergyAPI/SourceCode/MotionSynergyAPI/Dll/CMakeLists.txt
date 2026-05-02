#
# CppMotionSynergyAPIDll library
#
# This file gets copied to the MotionSynergyAPI/dll folder and renamed to CMakeLists.txt by generateRelease.py.
# This project defines the interface to the dll needed by clients (i.e. the header files).
#

add_imp_interface_library(CppMotionSynergyAPIDll "MotionSynergyAPI/Impl/native")

target_include_directories(CppMotionSynergyAPIDll
    INTERFACE include
    )


