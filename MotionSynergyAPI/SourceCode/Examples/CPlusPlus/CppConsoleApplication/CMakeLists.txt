#
# CppConsoleApplication executable
#

add_imp_executable(CppConsoleApplication "Applications")

target_include_directories(CppConsoleApplication
    PRIVATE include
    )

target_link_libraries(CppConsoleApplication
    PRIVATE
            Boost::program_options
    )

target_sources(CppConsoleApplication
    PRIVATE include/CppConsoleApplication.h
            include/CmdLineArgumentParser.h
            include/DMCM.h
            include/DOF5.h
            include/MotionSynergyAPIFactory.h
            include/Product.h
            include/SmartStageLinear.h
            include/SmartStageXY.h
            include/SmartStageXYZ.h

            src/CppConsoleApplication.cpp
            src/CmdLineArgumentParser.cpp
            src/DMCM.cpp
            src/DOF5.cpp
            src/MotionSynergyAPIFactory.cpp
            src/Product.cpp
            src/SmartStageLinear.cpp
            src/SmartStageXY.cpp
            src/SmartStageXYZ.cpp
    )

if(${CMAKE_PROJECT_NAME} STREQUAL "CPlusPlusExamples")

    if(NOT DEFINED MOTIONSYNERGYAPI_MODE OR MOTIONSYNERGYAPI_MODE STREQUAL "source")

        message(STATUS "Building MotionSynergyAPI from source.")

        target_link_libraries(CppConsoleApplication
            PRIVATE
                    CppMotionSynergyAPI
            )


        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Alerts ./CCP/Alerts)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Clock ./CCP/Clock)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/CommonServices ./CCP/CommonServices)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Configuration ./CCP/Configuration)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/CppMotionSynergyAPI ./CCP/CppMotionSynergyAPI)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/CppMotionSynergyAPIInterface ./CCP/CppMotionSynergyAPIInterface)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/CustomAlerts ./CCP/CustomAlerts)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Drivers/CommonDrivers ./CCP/Drivers/CommonDrivers)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Devices/CommonDevices ./CCP/Devices/CommonDevices)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Devices/AxisDevice ./CCP/Devices/AxisDevice)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Devices/DigitalInputDevice ./CCP/Devices/DigitalInputDevice)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Emulation/EmulatedDriverCommon ./CCP/Emulation/EmulatedDriverCommon)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Emulation/EmulatedAxisDriver ./CCP/Emulation/EmulatedAxisDriver)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Emulation/EmulatedDigitalInputDriver ./CCP/Emulation/EmulatedDigitalInputDriver)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Infrastructure ./CCP/Infrastructure)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Logging/Logger ./CCP/Logger)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Dispatcher ./CCP/Dispatcher)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/Timer ./CCP/Timer)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/SmartStageAxisDriver ./CCP/Drivers/SmartStageAxisDriver)
        add_subdirectory(${MOTIONSYNERGYAPI_NATIVE_ROOT}/SmartStageAxis ./CCP/SmartStageAxis)
    elseif(MOTIONSYNERGYAPI_MODE STREQUAL "sdk")

        # Ensure path is absolute
        get_filename_component(MOTIONSYNERGYAPI_SDK_PATH "${MOTIONSYNERGYAPI_SDK_PATH}" ABSOLUTE)

        message(STATUS "Using MotionSynergyAPI SDK from path: ${MOTIONSYNERGYAPI_SDK_PATH}")

        #IMP_CCP_DEPLOY_ROOT is used in MOTIONSYNERGYAPI_SDK
        set(IMP_CCP_DEPLOY_ROOT "${MOTIONSYNERGYAPI_SDK_PATH}")

        include("${MOTIONSYNERGYAPI_SDK_PATH}/MotionSynergyAPI_SDK.cmake")

        target_link_libraries(CppConsoleApplication 
            PRIVATE 
                Alerts
                DeviceAxis                  # AxisDevice
                Clock
                DeviceCommon                # CommonDevices
                DriverCommon                # CommonDrivers
                CommonServices
                Configuration
                CppMotionSynergyAPI
                CppMotionSynergyAPIInterface
                CustomAlerts
                DeviceDigitalInput          # DigitalInputDevice
                Dispatcher
                DriverEmulatedAxis          # EmulatedAxisDriver
                DriverEmulatedDigitalInput  # EmulatedDigitalInputDriver
                DriverEmulatedCommon        # EmulatedDriverCommon
                Infrastructure
                Logger
                SmartStageAxisDriver
                Timer

                Boost::log_setup
                Boost::log
        )

        add_custom_command(
            TARGET CppConsoleApplication
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MOTIONSYNERGYAPI_SDK_PATH}/bin/${IMP_CCP_PLATFORM}/$<CONFIG>/${CMAKE_SHARED_LIBRARY_PREFIX}SmartStageAxis${CMAKE_SHARED_LIBRARY_SUFFIX}"
            "$<TARGET_FILE_DIR:CppConsoleApplication>/${CMAKE_SHARED_LIBRARY_PREFIX}SmartStageAxis${CMAKE_SHARED_LIBRARY_SUFFIX}"
        )        

    elseif(MOTIONSYNERGYAPI_MODE STREQUAL "dll")

        message(STATUS "Using MotionSynergyAPI DLL at runtime.")

        add_compile_definitions(
            MOTIONSYNERGYAPI_DLL               # Indicates to the code that we are using the DLL version
        )

        add_subdirectory(${MOTIONSYNERGYAPI_DLL_PATH}/src/CppMotionSynergyAPIInterface ./CCP/CppMotionSynergyAPIInterface)
        add_subdirectory(${MOTIONSYNERGYAPI_DLL_PATH}/src/CppMotionSynergyAPIClient ./CCP/CppMotionSynergyAPIClient)
        add_subdirectory(${MOTIONSYNERGYAPI_DLL_PATH} ./CCP/CppMotionSynergyAPIDll)

        target_link_libraries(CppConsoleApplication
            PUBLIC    
                    Boost::headers
            PRIVATE
                    CppMotionSynergyAPIClient
                    CppMotionSynergyAPIDll

                    Boost::thread
            )

        add_custom_command(
            TARGET CppConsoleApplication
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MOTIONSYNERGYAPI_DLL_PATH}/bin/${IMP_CCP_PLATFORM}/$<CONFIG>/${CMAKE_SHARED_LIBRARY_PREFIX}MotionSynergyAPINative${CMAKE_SHARED_LIBRARY_SUFFIX}"
            "$<TARGET_FILE_DIR:CppConsoleApplication>/${CMAKE_SHARED_LIBRARY_PREFIX}MotionSynergyAPINative${CMAKE_SHARED_LIBRARY_SUFFIX}"
        )            

        add_custom_command(
            TARGET CppConsoleApplication
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MOTIONSYNERGYAPI_DLL_PATH}/bin/${IMP_CCP_PLATFORM}/$<CONFIG>/${CMAKE_SHARED_LIBRARY_PREFIX}SmartStageAxis${CMAKE_SHARED_LIBRARY_SUFFIX}"
            "$<TARGET_FILE_DIR:CppConsoleApplication>/${CMAKE_SHARED_LIBRARY_PREFIX}SmartStageAxis${CMAKE_SHARED_LIBRARY_SUFFIX}"
        )        

    else()
        message(FATAL_ERROR "Unknown MOTIONSYNERGYAPI_MODE: ${MOTIONSYNERGYAPI_MODE}. Valid options are: source, sdk, dll.")
    endif()

    if(MSVC)

        add_custom_command(
            TARGET CppConsoleApplication
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MOTIONSYNERGYAPI_COTS}/IXXAT/bin/x64/release/vciapi.dll"
                "$<TARGET_FILE_DIR:CppConsoleApplication>/vciapi.dll"
            )

        add_custom_command(
            TARGET CppConsoleApplication
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${MOTIONSYNERGYAPI_COTS}/IXXAT/bin/x64/release/vcinpl.dll"
                "$<TARGET_FILE_DIR:CppConsoleApplication>/vcinpl.dll"
            )

    endif()


else()
    message(STATUS "CppConsoleApplication is using the MotionSynergyAPI from the development solution.")

        target_link_libraries(CppConsoleApplication
            PRIVATE
                    CppMotionSynergyAPI
            )    
endif()