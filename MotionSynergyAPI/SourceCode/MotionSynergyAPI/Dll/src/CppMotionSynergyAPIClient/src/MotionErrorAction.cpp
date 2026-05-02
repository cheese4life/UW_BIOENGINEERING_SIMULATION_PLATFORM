/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "MotionErrorAction.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::MotionErrorAction

// clang-format off

DECLARE_ENUMCLASS_MAPPER( MotionErrorAction )
{
    ENUM_STRING( None ),
                 ENUM_STRING( AbruptStop ),
                 ENUM_STRING( SmoothStop ),
                 ENUM_STRING( DisableVelocityLoop ),
                 ENUM_STRING( DisablePositionLoop ),
                 ENUM_STRING( DisableCurrentLoop ),
                 ENUM_STRING( DisableMotor ),
                 ENUM_STRING( AbruptStopWithPosErrorClear ),
                 ENUM_STRING( PassiveBraking )
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( MotionErrorAction )

} //  namespace imp
