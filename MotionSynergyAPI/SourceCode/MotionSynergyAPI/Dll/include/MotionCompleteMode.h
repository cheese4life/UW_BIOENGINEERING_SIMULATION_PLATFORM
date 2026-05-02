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
*  @brief   Defines when the motion controller determines a move has completed.
 */
enum class MotionCompleteMode : int32_t
{
    CommandedPositionReached    = 0,   /*!< A move is complete as soon as the commanded position has been reached. The axis settle time and window is ignored. */
    MotionSettles               = 1,   /*!< A move is completed once the axis has reached the commanded position and has maintained position within the settle window for the length of the settle time. */

    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( MotionCompleteMode )

} //  namespace imp
