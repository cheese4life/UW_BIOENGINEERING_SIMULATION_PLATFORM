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
 * @brief   Defines the action performed by the motion controller when a motion error (i.e. position error) is detected.
 */
enum class MotionErrorAction : int32_t
{
    None = 0,                           /*!< Motion errors are ignored and no action is taken. */
    AbruptStop = 1,                     /*!< The axis is brought to an abrupt stop. */
    SmoothStop = 2,                     /*!< The axis is brought to a smooth stop using the current deceleration setting. */
    DisableVelocityLoop = 3,            /*!< Disable velocity loop and higher modules. Note this is currently not supported by SmartStage. */
    DisablePositionLoop = 4,            /*!< Disable position loop and higher modules. */
    DisableCurrentLoop = 5,             /*!< Disable current loop and higher modules. */
    DisableMotor = 6,                   /*!< The motor is disabled (de-energized). */
    AbruptStopWithPosErrorClear = 7,    /*!< Abrupt stop with position error clear. */
    PassiveBraking = 8,                 /*!< Passive braking. Note this is currently not supported by SmartStage.*/
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( MotionErrorAction )

} //  namespace imp
