#
# CppMotionSynergyAPIClient library
#
# This library provides a mechanism for C++ clients to access the Motion Synergy API.
# It uses the DllLoader to load the MotionSynergyAPINative dll at runtime (via the MotionSynergyAPISingleton).
# It provides an alternative implementation of the static Logger class which directs logging calls through
# the Motion Synergy API logging interface. This static class allows the logging macros to be used in client code
# without modification.
# There are also a number of source files that are automatically added to the target_sources when the 
# release package is created. These provide implementations for the enum string mappers which provide to/from string
# conversions for the various Motion Synergy API enums.
# There is also an implementation of the StackTrace class which is also used by the logging macros.
#

add_imp_static_library(CppMotionSynergyAPIClient "MotionSynergyAPI/Impl/native")

target_include_directories(CppMotionSynergyAPIClient
    PUBLIC include
    )

target_link_libraries(CppMotionSynergyAPIClient
    PUBLIC 
            imp::CppMotionSynergyAPIInterface

    PRIVATE
            Boost::filesystem
    )

if(MOTIONSYNERGYAPI_MODE STREQUAL "dll")
    target_link_libraries(CppMotionSynergyAPIClient
        PRIVATE
                CppMotionSynergyAPIDll
        )
else()
    target_link_libraries(CppMotionSynergyAPIClient
        PRIVATE
                imp::Logger
        )
endif()

target_sources(CppMotionSynergyAPIClient
    PRIVATE 
            include/MotionSynergyAPISingleton.h

            src/DllLoader.h
            src/DllLoader.cpp
            src/Logger.cpp
            src/MotionSynergyAPISingleton.cpp
    )

if(MOTIONSYNERGYAPI_MODE STREQUAL "dll")
        target_sources(CppMotionSynergyAPIClient
        PRIVATE 
                ### Additional source files for DLL mode, automatically added by generateRelease.py ###
                src/StackTrace.cpp
                src/DigitalIOBasicTypes.cpp
                src/MotionCompleteMode.cpp
                src/AxisRecoveryOperation.cpp
                src/DigitalOutputTrigger.cpp
                src/MotionControllerConfigurationType.cpp
                src/MotionErrorAction.cpp
                src/SerialBaudRate.cpp
                src/TraceTrigger.cpp
                src/UDPMode.cpp
                src/CanBaudRate.cpp
                src/SerialProtocol.cpp
                src/BiquadFilterType.cpp
                src/TraceVariable.cpp
                src/ActiveControlMode.cpp
                src/WaitResult.cpp
                src/LogLevel.cpp
                src/LoggerType.cpp
        )
endif()

deploy_imp_component(
    NAME                CppMotionSynergyAPIClient
    CMAKE_TARGET        CppMotionSynergyAPIClient
    HEADER_DIRS         ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
