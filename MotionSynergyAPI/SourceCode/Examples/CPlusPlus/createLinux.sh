#!/bin/bash
#   The copyright to the computer program(s) herein is the property of
#   Dover Motion, Inc. This file is subject to the terms and
#   conditions found in the 'DoverLicense.txt' file included with this
#   source code package.

set -e

__COVERAGE=0
__PLATFORM=x64
__CONFIG=Debug
__ASAN=0
__SDK=0
__DLL=1 # Default to DLL mode

while [ $# -ne 0 ]
do
    arg="$1"
    case "$arg" in
        --coverage)
        __COVERAGE=1
        shift  # move past arg
        ;;
        --platform)
        __PLATFORM="$2"
        shift  # move past arg
        shift  # move past value
        ;;
        --config)
        __CONFIG="$2"
        shift  # move past arg
        shift  # move past value
        ;;
        --asan)
        __ASAN=1
        shift  # move past arg
        ;;
        --sdk)
        __SDK=1
        __DLL=0
        shift  # move past arg
        ;;
        --source)
        __DLL=0
        __SDK=0
        shift  # move past arg
        ;;
        *)
        echo "Unknown option: $1"
        exit 1
        ;;
    esac
done

if [[ ("${__PLATFORM}" != "x64") && ("${__PLATFORM}" != "arm64") ]]; then
    echo "Unknown platform: ${__PLATFORM}"
    exit 1
fi
if [[ ("${__CONFIG}" != "Debug") && ("${__CONFIG}" != "Release") ]]; then
    echo "Unknown configuration: ${__CONFIG}"
    exit 1
fi

__CONFIG_LOWER=`echo "${__CONFIG}" | tr '[:upper:]' '[:lower:]'`
mkdir -p ./build_linux_${__PLATFORM}_${__CONFIG_LOWER}
pushd ./build_linux_${__PLATFORM}_${__CONFIG_LOWER}


__CMAKE_TOOLCHAIN_FOLDER="../../MotionSynergyAPI/Impl/native/CMake"
__CMAKE_ARGS=""

if [[ ${__ASAN} -eq 1 ]]; then
    __CMAKE_ARGS="${__CMAKE_ARGS} -DIMP_CCP_ENABLE_ASAN:bool=ON"
else
    __CMAKE_ARGS="${__CMAKE_ARGS} -DIMP_CCP_ENABLE_ASAN:bool=OFF"
fi

if [[ ${__SDK} -eq 1 ]]; then
    __CMAKE_ARGS="${__CMAKE_ARGS} -DMOTIONSYNERGYAPI_MODE=sdk -DMOTIONSYNERGYAPI_SDK_PATH=../../../MotionSynergyAPI_SDK"
fi

if [[ ${__DLL} -eq 1 ]]; then
    __CMAKE_ARGS="${__CMAKE_ARGS} -DMOTIONSYNERGYAPI_MODE=dll"
    __CMAKE_TOOLCHAIN_FOLDER="../../MotionSynergyAPI/Dll/src/CMake"
fi

__CMAKE_ARGS="${__CMAKE_ARGS} .."
cmake --fresh -DCMAKE_TOOLCHAIN_FILE=${__CMAKE_TOOLCHAIN_FOLDER}/Toolchain_LinuxGCC_${__PLATFORM}_${__CONFIG}.cmake ${__CMAKE_ARGS}
popd

