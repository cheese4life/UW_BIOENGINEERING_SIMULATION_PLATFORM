
# function for copying files based on a pattern to a destination folder
function(postbuild_copy_glob_if_different target_name source_file_pattern destination_dir)
    file(GLOB source_files ${source_file_pattern})
    foreach(source_file ${source_files})
        get_filename_component(source_filename ${source_file} NAME)
        add_custom_command(
            TARGET ${target_name}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${source_file}
                "${destination_dir}/${source_filename}"
            )
    endforeach()
endfunction()

# function for declaring an IMP static library
function(add_imp_static_library target_name ide_folder)
    add_library(${target_name})
    add_library(imp::${target_name} ALIAS ${target_name})

    # set IDE target folder
    set_target_properties(${target_name} PROPERTIES FOLDER "${ide_folder}")

    # set output folder
    if(DEFINED IMP_CCP_OUTPUT_ROOT)
        set_target_properties(${target_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${IMP_CCP_OUTPUT_ROOT}")
        set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${IMP_CCP_OUTPUT_ROOT}")
        set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${IMP_CCP_OUTPUT_ROOT}")
    else()
        message(FATAL_ERROR "IMP_CCP_OUTPUT_ROOT is not set")
    endif()
endfunction()

# function for declaring a shared library
function(add_shared_library target_name ide_folder)
    add_library(${target_name} SHARED)

    # set IDE target folder
    set_target_properties(${target_name} PROPERTIES FOLDER "${ide_folder}")

    # set output folder
    if(DEFINED IMP_CCP_OUTPUT_ROOT)
        set_target_properties(${target_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${IMP_CCP_OUTPUT_ROOT}")
        set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${IMP_CCP_OUTPUT_ROOT}")
        set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${IMP_CCP_OUTPUT_ROOT}")
    else()
        message(FATAL_ERROR "IMP_CCP_OUTPUT_ROOT is not set")
    endif()
endfunction()

# function for declaring an IMP interface library
function(add_imp_interface_library target_name ide_folder)
    add_library(${target_name} INTERFACE)
    add_library(imp::${target_name} ALIAS ${target_name})
    
    # NOTE: IDE target folder is ignored for interface libraries
endfunction()

# function for declaring an IMP executable
function(add_imp_executable target_name ide_folder)
    add_executable(${target_name})
    add_executable(imp::${target_name} ALIAS ${target_name})

    # TODO (REUSE-956) - on GCC, force a link group for imported libraries - having issues trying to link without it
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(${target_name} BEFORE PRIVATE "-Wl,--start-group")
    endif()

    # setup project settings
    # NOTE: using VC++ macros, not CMake macros - this is deliberate
    set_target_properties(${target_name}
        PROPERTIES  VS_DEBUGGER_WORKING_DIRECTORY   "$(OutDir)"
                    VS_DEBUGGER_COMMAND             "$(TargetFileName)"
    )

    # set IDE target folder
    set_target_properties(${target_name} PROPERTIES FOLDER "${ide_folder}")

    # set output folder
    if(DEFINED IMP_CCP_OUTPUT_ROOT)
        set_target_properties(${target_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${IMP_CCP_OUTPUT_ROOT}")
        set_target_properties(${target_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${IMP_CCP_OUTPUT_ROOT}")
        set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${IMP_CCP_OUTPUT_ROOT}")
    else()
        message(FATAL_ERROR "IMP_CCP_OUTPUT_ROOT is not set")
    endif()
endfunction()
