# function for deploying an IMP component
function(deploy_imp_component)
    # parse arguments
    cmake_parse_arguments(DEPLOY_IMP_COMPONENT "" "NAME;CMAKE_TARGET;MOCK_CMAKE_TARGET" "HEADER_DIRS;ALERT_DIRS;MOCK_DIRS" ${ARGN})
    if(NOT DEFINED DEPLOY_IMP_COMPONENT_NAME)
        message(FATAL_ERROR "NAME argument must be set")
    endif()
    if(NOT DEFINED DEPLOY_IMP_COMPONENT_CMAKE_TARGET)
        message(FATAL_ERROR "CMAKE_TARGET argument must be set")
    endif()
    if(DEFINED DEPLOY_IMP_COMPONENT_MOCK_CMAKE_TARGET)
        if(DEPLOY_IMP_COMPONENT_CMAKE_TARGET STREQUAL DEPLOY_IMP_COMPONENT_MOCK_CMAKE_TARGET)
            message(FATAL_ERROR "CMAKE_TARGET and MOCK_CMAKE_TARGET cannot be the same name")
        endif()
    endif()
    # get target properties
    get_target_property(__target_type ${DEPLOY_IMP_COMPONENT_CMAKE_TARGET} TYPE)
    # copy binary outputs
    install(TARGETS ${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}
            COMPONENT Binaries
            CONFIGURATIONS Debug
            RUNTIME DESTINATION ${IMP_CCP_DEPLOY_BIN_ROOT}/${IMP_CCP_PLATFORM}/Debug
            LIBRARY DESTINATION ${IMP_CCP_DEPLOY_BIN_ROOT}/${IMP_CCP_PLATFORM}/Debug
            ARCHIVE DESTINATION ${IMP_CCP_DEPLOY_LIB_ROOT}/${IMP_CCP_PLATFORM}/Debug
            )
    install(TARGETS ${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}
            COMPONENT Binaries
            CONFIGURATIONS Release
            RUNTIME DESTINATION ${IMP_CCP_DEPLOY_BIN_ROOT}/${IMP_CCP_PLATFORM}/Release
            LIBRARY DESTINATION ${IMP_CCP_DEPLOY_BIN_ROOT}/${IMP_CCP_PLATFORM}/Release
            ARCHIVE DESTINATION ${IMP_CCP_DEPLOY_LIB_ROOT}/${IMP_CCP_PLATFORM}/Release
            )
    # for MSVC, copy the PDB file as well
    if(MSVC)
        if(__target_type STREQUAL "STATIC_LIBRARY" OR __target_type STREQUAL "SHARED_LIBRARY" OR __target_type STREQUAL "EXECUTABLE")
            install(FILES $<TARGET_FILE_DIR:${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}>/${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}.pdb
                    COMPONENT Binaries
                    CONFIGURATIONS Debug
                    DESTINATION ${IMP_CCP_DEPLOY_LIB_ROOT}/${IMP_CCP_PLATFORM}/Debug)
            # no pdb created for release build
            #install(FILES $<TARGET_FILE_DIR:${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}>/${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}.pdb
            #        CONFIGURATIONS Release
            #        COMPONENT Binaries
            #        DESTINATION ${IMP_CCP_DEPLOY_LIB_ROOT}/Release)
        endif()
    endif()
    # clear the deployment file once before appending to it
    if(NOT CLEAR_DEPLOY_FILE_ONCE)
        install(CODE "file(WRITE ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"\")"
                COMPONENT Common)
        set(CLEAR_DEPLOY_FILE_ONCE ON CACHE INTERNAL "" FORCE)
    endif()    
    # copy headers
    if(DEFINED DEPLOY_IMP_COMPONENT_HEADER_DIRS)
        set(__has_headers OFF)

        set(HEADER_FILES)
        foreach(__header_dir ${DEPLOY_IMP_COMPONENT_HEADER_DIRS})
            if(NOT EXISTS "${__header_dir}")
                message(FATAL_ERROR "Header folder '${__header_dir}' for target '${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}' does not exist")
            endif()
            set(__has_headers ON)
            file(GLOB DIR_HEADER_FILES CONFIGURE_DEPENDS "${__header_dir}/*.h")
            list(APPEND HEADER_FILES ${DIR_HEADER_FILES})
        endforeach()
        
        message(VERBOSE "Found header files: ${HEADER_FILES}")
        install(FILES ${HEADER_FILES} 
                COMPONENT Common
                DESTINATION ${IMP_CCP_DEPLOY_INCLUDE_ROOT}/${DEPLOY_IMP_COMPONENT_NAME}
                )

        if(__has_headers)
            install(CODE "file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"#\\n\") \n\
                          file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"# ${DEPLOY_IMP_COMPONENT_CMAKE_TARGET} IMP interface library\\n\") \n\
                          file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"#\\n\") \n\
                          file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"add_library(${DEPLOY_IMP_COMPONENT_CMAKE_TARGET} INTERFACE IMPORTED GLOBAL)\\n\") \n\
                          file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"add_library(imp::${DEPLOY_IMP_COMPONENT_CMAKE_TARGET} ALIAS ${DEPLOY_IMP_COMPONENT_CMAKE_TARGET})\\n\") \n\
                          file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"target_include_directories(${DEPLOY_IMP_COMPONENT_CMAKE_TARGET} SYSTEM INTERFACE \\\${IMP_CCP_DEPLOY_ROOT}/include/${DEPLOY_IMP_COMPONENT_NAME})\\n\") \n"
                         COMPONENT Common
                         )
            if(__target_type STREQUAL "STATIC_LIBRARY" OR __target_type STREQUAL "SHARED_LIBRARY")
                install(CODE "file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"target_link_libraries(${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}\\n\") \n"
                             COMPONENT Common
                             )
                if(WIN32 AND MSVC)
                    install(CODE "file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"    INTERFACE optimized \\\"\\\${IMP_CCP_DEPLOY_ROOT}/lib/\\\${IMP_CCP_PLATFORM}/Release/${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}.lib\\\"\\n\") \n\
                                  file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"    INTERFACE debug \\\"\\\${IMP_CCP_DEPLOY_ROOT}/lib/\\\${IMP_CCP_PLATFORM}/Debug/${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}.lib\\\"\\n\") \n"
                                 COMPONENT Common
                                 )
                else()
                    install(CODE "file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"    INTERFACE optimized \\\"\\\${IMP_CCP_DEPLOY_ROOT}/lib/\\\${IMP_CCP_PLATFORM}/Release/lib${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}.a\\\"\\n\") \n\
                                  file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"    INTERFACE debug \\\"\\\${IMP_CCP_DEPLOY_ROOT}/lib/\\\${IMP_CCP_PLATFORM}/Debug/lib${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}.a\\\"\\n\") \n"
                                 COMPONENT Common
                                 )
                endif()
                install(CODE "file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"    )\\n\")"
                             COMPONENT Common
                             )
            endif()
            install(CODE "file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"\\n\")"
                         COMPONENT Common
                         )
        endif()
    endif()
    # copy mocks
    if(DEFINED DEPLOY_IMP_COMPONENT_MOCK_DIRS)
        set(__has_mocks OFF)
        set(__mock_sources "")
        foreach(__mock_dir ${DEPLOY_IMP_COMPONENT_MOCK_DIRS})
            if(NOT EXISTS "${__mock_dir}")
                message(FATAL_ERROR "Mock folder '${__mock_dir}' for target '${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}' does not exist")
            endif()
            set(__has_mocks ON)
            install(DIRECTORY ${__mock_dir}/
                    COMPONENT Common
                    DESTINATION ${IMP_CCP_DEPLOY_MOCKS_ROOT}/${DEPLOY_IMP_COMPONENT_NAME}
                    USE_SOURCE_PERMISSIONS
                    )
            file(GLOB __mock_dir_sources "${__mock_dir}/src/*")
            foreach(__mock_dir_source ${__mock_dir_sources})
                get_filename_component(__mock_source_name "${__mock_dir_source}" NAME)
                list(APPEND __mock_sources "${__mock_source_name}")
            endforeach()
        endforeach()
        if(__has_mocks)
            if(NOT DEFINED DEPLOY_IMP_COMPONENT_MOCK_CMAKE_TARGET)
                message(FATAL_ERROR "Component has mock headers, but MOCK_CMAKE_TARGET is not set")
            endif()

            install(CODE "file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"#\\n\") \n\
                            file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"# ${DEPLOY_IMP_COMPONENT_MOCK_CMAKE_TARGET} IMP mock interface library\\n\") \n\
                            file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"#\\n\") \n\
                            file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"add_library(${DEPLOY_IMP_COMPONENT_MOCK_CMAKE_TARGET} INTERFACE)\\n\") \n\
                            file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"add_library(imp::${DEPLOY_IMP_COMPONENT_MOCK_CMAKE_TARGET} ALIAS ${DEPLOY_IMP_COMPONENT_MOCK_CMAKE_TARGET})\\n\") \n\
                            file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"target_include_directories(${DEPLOY_IMP_COMPONENT_MOCK_CMAKE_TARGET} SYSTEM INTERFACE \\\${IMP_CCP_DEPLOY_ROOT}/mocks/${DEPLOY_IMP_COMPONENT_NAME}/include)\\n\") \n\
                            file(APPEND ${IMP_CCP_DEPLOY_CMAKE_DEFS} \"\\n\")"
                            COMPONENT Common
                            )
        endif()
    endif()
    # copy alerts
    if(DEFINED DEPLOY_IMP_COMPONENT_ALERT_DIRS)
        foreach(__alert_dir ${DEPLOY_IMP_COMPONENT_ALERT_DIRS})
            if(NOT EXISTS "${__alert_dir}")
                message(FATAL_ERROR "Alerts folder '${__alert_dir}' for target '${DEPLOY_IMP_COMPONENT_CMAKE_TARGET}' does not exist")
            endif()
            install(DIRECTORY ${__alert_dir}/
                    COMPONENT Common
                    DESTINATION ${IMP_CCP_DEPLOY_ALERTS_ROOT}/${DEPLOY_IMP_COMPONENT_NAME}
                    USE_SOURCE_PERMISSIONS
                    )
        endforeach()
    endif()
    # copy docs (assumed to be in the "docs" folder relative to the root) - cannot use on interface libraries
    if(NOT __target_type STREQUAL "INTERFACE_LIBRARY")
        get_target_property(__target_source_dir ${DEPLOY_IMP_COMPONENT_CMAKE_TARGET} SOURCE_DIR)
        if(EXISTS ${__target_source_dir}/docs)
            install(DIRECTORY ${__target_source_dir}/docs/
                    COMPONENT Common
                    DESTINATION ${IMP_CCP_DEPLOY_DOCS_ROOT}/${DEPLOY_IMP_COMPONENT_NAME}
                    USE_SOURCE_PERMISSIONS
                    )
        endif()
    endif()
endfunction()
