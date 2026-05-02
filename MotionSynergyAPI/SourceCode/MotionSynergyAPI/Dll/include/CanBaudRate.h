/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "EnumClass.h"

namespace imp
{

/*!
 *  @brief   Defines the set of baud rates that can be used for CAN communications.
 */
enum class CanBaudRate : int32_t
{
    Baud1000000,
    Baud800000,
    Baud500000,
    Baud250000,
    Baud125000,
    Baud50000,
    Baud20000,
    Baud10000,
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( CanBaudRate )

} //  namespace imp
