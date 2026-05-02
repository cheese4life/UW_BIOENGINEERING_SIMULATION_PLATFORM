/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "SerialProtocol.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::SerialProtocol

// clang-format off

DECLARE_ENUMCLASS_MAPPER( SerialProtocol )
{
    ENUM_STRING(Point2Point),
    ENUM_STRING(MultiDropUsingIdleLineDetection)
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( SerialProtocol )

} //  namespace imp
