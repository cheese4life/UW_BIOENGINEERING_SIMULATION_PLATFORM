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
 *  @brief   Defines the recovery operation required return the axis to a state where it can performs moves.
 */
enum class AxisRecoveryOperation : int32_t
{
    NoneRequired,  /*!< The axis is already in a state where it can perform moves and no recovery action is required. */
    ResetPosition, /*!< The axis has encountered a position error and ResetPosition() must be called to reset its commanded position to match its actual position. */
    ResetPositionAndEnable, /*!< The axis has encountered a position error and the motor has been disabled (de-energized). ResetPosition() must be called to reset its commanded position to match its actual position, followed by Enable() to re-energize the motor. */
    Initialize, /*!< The axis has encountered a general error and must be re-initialized by calling Initialize(). */
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( AxisRecoveryOperation )

} //  namespace imp
