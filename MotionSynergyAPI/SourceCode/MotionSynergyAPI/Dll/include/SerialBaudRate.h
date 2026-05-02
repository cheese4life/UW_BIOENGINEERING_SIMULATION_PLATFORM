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
 *  @brief   Defines the set of baud rates that can be used for Serial communications.
 */
enum class SerialBaudRate : int32_t
{
    Baud1200,
    Baud2400,
    Baud9600,
    Baud19200,
    Baud57600,
    Baud115200,
    Baud230400,
    Baud460800,
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( SerialBaudRate )

} //  namespace imp
