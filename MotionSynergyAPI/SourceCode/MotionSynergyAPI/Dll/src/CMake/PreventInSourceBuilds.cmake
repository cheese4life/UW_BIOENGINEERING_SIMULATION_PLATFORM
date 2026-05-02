# Taken from https://github.com/lefticus/cpp_starter_project/blob/master/cmake/PreventInSourceBuilds.cmake
#
# This function will prevent in-source builds
function(PreventInSourceBuilds)
    # make sure the user doesn't play dirty with symlinks
    get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
    get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

    # disallow in-source builds
    if("${srcdir}" STREQUAL "${bindir}")
        message(STATUS "Please remove the following files and directories from ${CMAKE_BINARY_DIR}:")
        message(STATUS " * CMakeCache.txt")
        message(STATUS " * CMakeFiles")
        message(FATAL_ERROR "In-source builds are not supported")
    endif()
endfunction()

PreventInSourceBuilds()
