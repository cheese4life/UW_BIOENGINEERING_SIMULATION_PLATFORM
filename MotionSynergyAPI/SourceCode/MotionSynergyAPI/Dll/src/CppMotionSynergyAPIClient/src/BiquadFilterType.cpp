/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "BiquadFilterType.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::BiquadFilterType

// clang-format off

DECLARE_ENUMCLASS_MAPPER( BiquadFilterType )
{
    ENUM_STRING(None),
    ENUM_STRING(Unknown),
    ENUM_STRING(Notch),
    ENUM_STRING(LowPass),
    ENUM_STRING(Lead),
    ENUM_STRING(Lag)
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( BiquadFilterType )

} //  namespace imp
