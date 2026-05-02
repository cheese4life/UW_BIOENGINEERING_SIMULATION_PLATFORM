/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "EnumClass.h"

// clang-format off

/*!
 *  @brief   Invetech Modular Platform namespace
 */
namespace imp
{

/*!
*  @brief   Defines the different types of motion controller configuration settings which can be written to NVRAM
*           using the IMotionControllerConfiguration.WriteToNVRAM method.
 */
enum class MotionControllerConfigurationType : int32_t
{
    None = 0,             /*!< Invalid motion controller configuration type. */
    MotionTracking = 1,   /*!< The motion tracking configuration settings (see MotionTrackingSettings). */
    MotionError    = 2,   /*!< The difference between the actual and commanded position (see MotionErrorSettings). */
    PositionLoop = 3,     /*!< The position loop settings (see PositionLoopSettings). */
    ActiveCommunication  = 4,   /*!< The active communication configuration settings (see CommunicationSettings). This requires the communications link to be re-established (i.e. disconnect / connect). */
    InactiveCommunication = 5, /*!< The inactive communication configuration settings (see CommunicationSettings). Does not affect the current communications link. */
    CurrentLoop = 6,      /*!< The current loop settings (see CurrentLoopSettings). */
    BiquadFilter = 7,      /*!< The biquad filter settings (see BiquadFilterSettings). */

    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( MotionControllerConfigurationType )

} //  namespace imp
