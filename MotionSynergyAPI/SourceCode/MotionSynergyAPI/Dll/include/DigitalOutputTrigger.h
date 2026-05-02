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
 *  @brief   Defines the set of triggers which can be used to set the AxisOut1 general purpose output (GPO).
 */
enum class DigitalOutputTrigger : int32_t
{
    Disabled = 0,           /*!< Digital output is set low and triggers are disabled (i.e. the digital output state is not affected by the axis state). */
    InMotion = 1,           /*!< The digital output will be set high whenever the axis is in motion. */
    MotionComplete = 2,     /*!< The digital output will be set high whenever the axis completes a move. */
    AtMaxVelocity = 3,      /*!< The digital output will be set high when the axis reaches maximum velocity during its move. */
    Immediate = 4,          /*!< The digital output will be set high immediately (i.e. the digital output is directly controlled by software). */
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( DigitalOutputTrigger )

} //  namespace imp
