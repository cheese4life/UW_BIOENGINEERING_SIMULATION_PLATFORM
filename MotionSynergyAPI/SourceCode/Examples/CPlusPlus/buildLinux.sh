#!/bin/bash
#   The copyright to the computer program(s) herein is the property of
#   Dover Motion, Inc. This file is subject to the terms and
#   conditions found in the 'DoverLicense.txt' file included with this
#   source code package.

set -e

__PLATFORM=x64
__CONFIG=Debug

while [ $# -ne 0 ]
do
    arg="$1"
    case "$arg" in
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
        *)
        echo "Unknown option: $1"
        exit 1
        ;;
    esac
done

__CONFIG_LOWER=`echo "${__CONFIG}" | tr '[:upper:]' '[:lower:]'`
__MAKE_ARGS="-j`nproc`"
__MAKE_DIR=./build_linux_${__PLATFORM}_${__CONFIG_LOWER}

if [[ ("${__PLATFORM}" != "x64") && ("${__PLATFORM}" != "arm64") ]]; then
    echo "Unknown platform: ${__PLATFORM}"
    exit 1
fi
if [[ ("${__CONFIG}" != "Debug") && ("${__CONFIG}" != "Release") ]]; then
    echo "Unknown configuration: ${__CONFIG}"
    exit 1
fi

pushd ${__MAKE_DIR}
make ${__MAKE_ARGS}
popd
