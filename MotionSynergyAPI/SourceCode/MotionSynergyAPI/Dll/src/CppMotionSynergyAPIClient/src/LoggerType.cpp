/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "LoggerType.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::LoggerType
DECLARE_ENUMCLASS_MAPPER( LoggerType ){ ENUM_STRING( Unknown ), ENUM_STRING( NullLogger ), ENUM_STRING( BoostLog ) };

namespace imp
{
ENUMCLASS_FUNCTIONS_IMPL( LoggerType )
} //  namespace imp
