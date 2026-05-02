#
# CppMotionSynergyAPIInterface
#

add_imp_interface_library(CppMotionSynergyAPIInterface "MotionSynergyAPI/Impl/native")

target_include_directories(CppMotionSynergyAPIInterface
    INTERFACE include
    )

deploy_imp_component(
    NAME                CppMotionSynergyAPIInterface
    CMAKE_TARGET        CppMotionSynergyAPIInterface
    HEADER_DIRS         ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
