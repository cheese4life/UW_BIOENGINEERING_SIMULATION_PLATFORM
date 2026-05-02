#
# LunaFive library
#
add_library(LunaFive INTERFACE)

target_include_directories(LunaFive
    INTERFACE    include
    )

target_sources(LunaFive
    INTERFACE include/LunaFive.h
    )

