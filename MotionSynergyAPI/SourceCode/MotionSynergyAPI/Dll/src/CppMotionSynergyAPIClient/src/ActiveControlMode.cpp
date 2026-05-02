/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "EnumClassMapper.h"
#include "ActiveControlMode.h"

#define ENUM_NAMESPACE imp::ActiveControlMode

// clang-format off

DECLARE_ENUMCLASS_MAPPER( ActiveControlMode )
{
    ENUM_STRING(Normal),
    ENUM_STRING(UDPTime),
    ENUM_STRING(UDPCoordinatedMotion),
    ENUM_STRING(UDPSineWave),
    ENUM_STRING(RME),
    ENUM_STRING(ExternalControl)
};

namespace imp
{

    ENUMCLASS_FUNCTIONS_IMPL( ActiveControlMode )

} //  namespace imp
