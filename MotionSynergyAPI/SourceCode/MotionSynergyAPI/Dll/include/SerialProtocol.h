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
 *  @brief   Defines the protocol to use for serial communications.
 */
enum class SerialProtocol : int32_t
{
    Point2Point,                     /*!< RS232 point-to-point */
    MultiDropUsingIdleLineDetection, /*!< RS485 Multi-drop using idle-line detection */
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( SerialProtocol )

} //  namespace imp
