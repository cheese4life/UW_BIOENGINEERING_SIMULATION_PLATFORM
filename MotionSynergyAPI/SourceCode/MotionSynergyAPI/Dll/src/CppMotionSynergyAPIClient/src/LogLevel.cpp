/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "LogLevel.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::LogLevel
DECLARE_ENUMCLASS_MAPPER( LogLevel ){
    ENUM_STRING( Trace ),
    ENUM_STRING( Debug ),
    ENUM_STRING( Info ),
    ENUM_STRING( Warning ),
    ENUM_STRING( Error ),
    ENUM_STRING( Fatal ) };

namespace imp
{
ENUMCLASS_FUNCTIONS_IMPL( LogLevel )
} //  namespace imp
