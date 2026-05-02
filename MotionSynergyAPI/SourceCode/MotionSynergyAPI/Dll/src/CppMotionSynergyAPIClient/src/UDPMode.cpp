/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "UDPMode.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::UDPMode

// clang-format off

DECLARE_ENUMCLASS_MAPPER( UDPMode )
{
    //ENUM_STRING(None),
    ENUM_STRING(Time),
    ENUM_STRING(Encoder),
    ENUM_STRING(SineWave),
};

namespace imp
{

    ENUMCLASS_FUNCTIONS_IMPL( UDPMode )

} //  namespace imp
