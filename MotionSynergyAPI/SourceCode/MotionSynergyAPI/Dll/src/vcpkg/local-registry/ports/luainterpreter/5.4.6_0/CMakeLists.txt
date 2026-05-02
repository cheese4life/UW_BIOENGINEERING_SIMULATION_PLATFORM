# Lua can be compiled as either C or C++.
# Default configuration is C, set COMPILE_AS_CPP to ON to use C++.
# See http://stackoverflow.com/questions/13560945/c-and-c-library-using-longjmp for why would you want to do that.
# Primary differences:
# - Exceptions will be used instead of setjmp/longjmp
# - The name mangling for functions will be C++ instead of C.
#     - This is a source-incompatible change because extern "C" is chosen by the including application.
# - The lua.hpp header will not be available.

CMAKE_MINIMUM_REQUIRED(VERSION 3.18)

PROJECT(luainterpreter)

find_package(unofficial-luafilesystem CONFIG REQUIRED)

FILE(GLOB SRC_LIBLUA "${CMAKE_SOURCE_DIR}/src/*.c")
# Executables luac
LIST(REMOVE_ITEM SRC_LIBLUA "${CMAKE_SOURCE_DIR}/src/luac.c")

IF (WIN32)
    # remove warnings
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS )
ENDIF ()

IF (UNIX)
    ADD_DEFINITIONS (-DLUA_USE_POSIX)
    FIND_LIBRARY (LIB_MATH NAMES m)
ENDIF ()

# C library
SET (CMAKE_C_STANDARD 99)
SET(SRC_LIBLUAC ${SRC_LIBLUA})
SET_SOURCE_FILES_PROPERTIES(${SRC_LIBLUAC} PROPERTIES LANGUAGE CXX)     # Compile as C++
ADD_EXECUTABLE ( luai ${SRC_LIBLUA} )  # interpreter
TARGET_INCLUDE_DIRECTORIES(luai PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

SET_TARGET_PROPERTIES ( luai PROPERTIES OUTPUT_NAME lua PDB_NAME luai )
IF (UNIX)
    IF (CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
        SET (_LIB_READLINE_NAME edit)
    ELSE ()
        SET (_LIB_READLINE_NAME readline)
    ENDIF ()
    FIND_LIBRARY (LIB_READLINE NAMES ${_LIB_READLINE_NAME})
    IF (LIB_READLINE)
        TARGET_COMPILE_DEFINITIONS (luai PUBLIC -DLUA_USE_READLINE)
        TARGET_LINK_LIBRARIES(luai PRIVATE ${LIB_READLINE})
        IF (_LIB_READLINE_NAME STREQUAL edit)
            TARGET_INCLUDE_DIRECTORIES (luai PUBLIC /usr/include/edit)
        ENDIF ()
    ENDIF ()
ENDIF ()
INSTALL ( TARGETS luai RUNTIME DESTINATION tools/luainterpreter )


