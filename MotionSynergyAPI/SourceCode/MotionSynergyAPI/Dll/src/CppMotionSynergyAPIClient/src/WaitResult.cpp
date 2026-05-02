/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "WaitResult.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::WaitResult
DECLARE_ENUMCLASS_MAPPER( WaitResult ){ ENUM_STRING( Success ), ENUM_STRING( Timeout ), ENUM_STRING( Cancelled ) };

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( WaitResult )

} //  namespace imp
