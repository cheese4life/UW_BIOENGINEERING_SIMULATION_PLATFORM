/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "MotionControllerConfigurationType.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::MotionControllerConfigurationType

// clang-format off

DECLARE_ENUMCLASS_MAPPER( MotionControllerConfigurationType )
{
    ENUM_STRING( None ),
                 ENUM_STRING( MotionTracking ),
                 ENUM_STRING( MotionError ),
                 ENUM_STRING( PositionLoop ),
                 ENUM_STRING( ActiveCommunication ),
                 ENUM_STRING( InactiveCommunication ),
                 ENUM_STRING( CurrentLoop ),
                 ENUM_STRING( BiquadFilter ),
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( MotionControllerConfigurationType )

} //  namespace imp
