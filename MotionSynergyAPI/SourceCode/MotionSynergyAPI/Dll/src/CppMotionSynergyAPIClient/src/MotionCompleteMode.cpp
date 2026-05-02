/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "MotionCompleteMode.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::MotionCompleteMode

// clang-format off

DECLARE_ENUMCLASS_MAPPER( MotionCompleteMode )
{
    ENUM_STRING(CommandedPositionReached),
    ENUM_STRING(MotionSettles),
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( MotionCompleteMode )

} //  namespace imp
