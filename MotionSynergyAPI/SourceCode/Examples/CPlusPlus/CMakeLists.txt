# This is the top level CMake file for the C++ Examples

cmake_minimum_required(VERSION 3.25)

# Set paths
set(APPLICATION_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
set(SOURCECODE_ROOT "${APPLICATION_ROOT}/SourceCode")
set(APPLICATION_SUPPORT_FOLDER "${APPLICATION_ROOT}/SupportFolder")
set(APPLICATION_CONFIG_FOLDER "${APPLICATION_SUPPORT_FOLDER}/ConfigurationFiles")

set(MOTIONSYNERGYAPI_ROOT "${SOURCECODE_ROOT}/MotionSynergyAPI")
set(MOTIONSYNERGYAPI_NATIVE_IMPL "${MOTIONSYNERGYAPI_ROOT}/Impl")
set(MOTIONSYNERGYAPI_NATIVE_ROOT "${MOTIONSYNERGYAPI_NATIVE_IMPL}/native")
set(MOTIONSYNERGYAPI_COTS "${MOTIONSYNERGYAPI_ROOT}/COTS")
set(MOTIONSYNERGYAPI_DLL_PATH "${MOTIONSYNERGYAPI_ROOT}/Dll")

set(IMP_CCP_COTS_ROOT "${MOTIONSYNERGYAPI_ROOT}/COTS")
set(IMP_CCP_VERSION_ROOT "${MOTIONSYNERGYAPI_NATIVE_IMPL}")

if(MOTIONSYNERGYAPI_MODE STREQUAL "dll")
    set(IMP_CCP_CMAKE "${MOTIONSYNERGYAPI_DLL_PATH}/src/CMake")
else()
    set(IMP_CCP_CMAKE "${MOTIONSYNERGYAPI_NATIVE_ROOT}/CMake")
endif()


# Set output directories - put all outputs into the one folder
set(IMP_CCP_OUTPUT_ROOT "${CMAKE_BINARY_DIR}/bin")

# Enforce out-of-source builds.
include(${IMP_CCP_CMAKE}/PreventInSourceBuilds.cmake)

# Only expose the "debug" and "release" configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(DEFINED CMAKE_BUILD_TYPE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

option(IMP_CCP_ENABLE_ASAN "Turn on AddressSanitizer for all projects" OFF)

# Set details for the CPlusPlusExamples project.
project(CPlusPlusExamples
        VERSION 0.1.0
        DESCRIPTION "CPlusPlusExamples"
        HOMEPAGE_URL "https://dovermotion.com"
        LANGUAGES C CXX)


# include rules for IMP
include("${IMP_CCP_CMAKE}/CPPOptions.cmake")
include("${IMP_CCP_CMAKE}/IMP_COTS.cmake")
include("${IMP_CCP_CMAKE}/IMP_Common.cmake")
include("${IMP_CCP_CMAKE}/IMP_DeployComponent.cmake")
include("${IMP_CCP_CMAKE}/IMP_GenerateGraphVizOptions.cmake")

# Enable generation of folders for IDE generators.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(CppConsoleApplication)

# Set the startup project in Visual Studio to the CppConsoleApplication.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CppConsoleApplication)

set(SUPPORT_FOLDER_PATH "$<TARGET_FILE_DIR:CppConsoleApplication>/SupportFolder")
set(PROGRAMDATA_FOLDER_PATH "$<TARGET_FILE_DIR:CppConsoleApplication>/ProgramDataFolder")

# Check first time startup has been run
if(NOT EXISTS ${APPLICATION_SUPPORT_FOLDER}/Instrument.cfg)
    message(FATAL_ERROR "The MotionSynergyGUI application must be run to select your product and communications settings.")
endif()

add_custom_target(ConfigurationFiles
                  ALL
                  SOURCES   ${APPLICATION_SUPPORT_FOLDER}/Instrument.cfg
                            ${APPLICATION_SUPPORT_FOLDER}/Logger.cfg
                  )

get_target_property(sources ConfigurationFiles SOURCES)
source_group(TREE ${APPLICATION_SUPPORT_FOLDER}
            FILES ${sources}
)

# Copy Configuration Files

postbuild_copy_glob_if_different(ConfigurationFiles
    "${APPLICATION_SUPPORT_FOLDER}/*.cfg"
    "${SUPPORT_FOLDER_PATH}"
    )
