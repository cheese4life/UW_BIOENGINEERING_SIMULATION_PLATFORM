/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "DigitalOutputTrigger.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::DigitalOutputTrigger

// clang-format off

DECLARE_ENUMCLASS_MAPPER( DigitalOutputTrigger )
{
    ENUM_STRING(Disabled),
    ENUM_STRING(InMotion),
    ENUM_STRING(MotionComplete),
    ENUM_STRING(AtMaxVelocity),
    ENUM_STRING(Immediate),
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( DigitalOutputTrigger )

} //  namespace imp
