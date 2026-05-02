/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "AxisRecoveryOperation.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::AxisRecoveryOperation

// clang-format off

DECLARE_ENUMCLASS_MAPPER( AxisRecoveryOperation )
{
    ENUM_STRING(NoneRequired),
    ENUM_STRING(ResetPosition),
    ENUM_STRING(ResetPositionAndEnable),
    ENUM_STRING(Initialize)
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( AxisRecoveryOperation )

} //  namespace imp
