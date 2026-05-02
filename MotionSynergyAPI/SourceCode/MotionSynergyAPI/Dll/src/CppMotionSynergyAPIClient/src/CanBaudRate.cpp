/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "CanBaudRate.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::CanBaudRate

// clang-format off

DECLARE_ENUMCLASS_MAPPER( CanBaudRate )
{
    ENUM_STRING(Baud1000000),
    ENUM_STRING(Baud800000),
    ENUM_STRING(Baud500000),
    ENUM_STRING(Baud250000),
    ENUM_STRING(Baud125000),
    ENUM_STRING(Baud50000),
    ENUM_STRING(Baud20000),
    ENUM_STRING(Baud10000)
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( CanBaudRate )

} //  namespace imp
