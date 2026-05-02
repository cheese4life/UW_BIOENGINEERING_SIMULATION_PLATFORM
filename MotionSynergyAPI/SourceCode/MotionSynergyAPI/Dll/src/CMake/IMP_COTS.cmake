# check if IMP_CCP_COTS_ROOT is defined first
if(NOT DEFINED IMP_CCP_COTS_ROOT)
    message(FATAL_ERROR "The IMP_CCP_COTS_ROOT variable is not defined")
endif()

# check if IMP_CCP_PLATFORM is defined first
if(NOT DEFINED IMP_CCP_PLATFORM)
    message(FATAL_ERROR "The IMP_CCP_PLATFORM variable is not defined")
endif()

# set COTS variables
set(IMP_CCP_COTS_LUNAFIVE_ROOT  "${IMP_CCP_COTS_ROOT}/LunaFive-5.0.1")
set(IMP_IC_COTS_TINYFSM_ROOT    "${IMP_CCP_COTS_ROOT}/tinyfsm-0.3.2")

string(TOLOWER "${CMAKE_SYSTEM_NAME}" CMAKE_SYSTEM_NAME_LOWER)

find_package(Boost REQUIRED
    COMPONENTS  date_time
                regex
                log
                filesystem
                chrono
                log_setup
                system
                thread
                program_options
                atomic
    )

# add Google Test/Mock libraries
find_package(GTest REQUIRED)

# Add lua library
find_package(Lua REQUIRED)

# Add LunaFive library
add_subdirectory(${IMP_CCP_COTS_LUNAFIVE_ROOT} ./COTS/LunaFive)

# tinyfsm header-only library
add_library(tinyfsm INTERFACE)

target_include_directories(tinyfsm
    INTERFACE 
                ${IMP_IC_COTS_TINYFSM_ROOT}/include
)
