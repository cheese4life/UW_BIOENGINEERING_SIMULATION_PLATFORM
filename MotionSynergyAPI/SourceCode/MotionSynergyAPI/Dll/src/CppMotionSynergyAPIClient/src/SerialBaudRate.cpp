/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "SerialBaudRate.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::SerialBaudRate

// clang-format off

DECLARE_ENUMCLASS_MAPPER( SerialBaudRate )
{
    ENUM_STRING(Baud1200),
    ENUM_STRING(Baud2400),
    ENUM_STRING(Baud9600),
    ENUM_STRING(Baud19200),
    ENUM_STRING(Baud57600),
    ENUM_STRING(Baud115200),
    ENUM_STRING(Baud230400),
    ENUM_STRING(Baud460800)
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( SerialBaudRate )

} //  namespace imp
