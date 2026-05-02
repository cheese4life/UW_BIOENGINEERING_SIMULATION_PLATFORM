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
 *  @brief   Defines the axis' active control mode, which determines how the axis is being commanded and the type of move profile it's using.
 */
enum class ActiveControlMode : int32_t
{
    Normal,  /*!< The axis is controlled in the normal manner, using normal move profiles and move commands. */
    UDPTime, /*!< The axis is controlled using a User Defined Profile (UDP) time-based profile. */
    UDPCoordinatedMotion, /*!< The axis is controlled using a User Defined Profile (UDP) master / driven axis profile, where this axis is operating as the driven axis. */
    UDPSineWave,          /*!< The axis is controlled using a User Defined Profile (UDP) sine wave profile. */
    RME,                  /*!< The axis is controlled using a Repetitive Motion Enhancement (RME) profile. */
    ExternalControl,      /*!< The axis is controlled via an external step/direction electronic signal. */
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( ActiveControlMode )

} //  namespace imp
