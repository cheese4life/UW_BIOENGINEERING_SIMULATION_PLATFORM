/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "TraceTrigger.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::TraceTrigger

// clang-format off

DECLARE_ENUMCLASS_MAPPER( TraceTrigger )
{
    ENUM_STRING(Immediate),
    ENUM_STRING(OnMotionStart),
    ENUM_STRING(OnMotionStartDelayed),
    ENUM_STRING(OnMotionStartNoEnd)
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( TraceTrigger )

} //  namespace imp
