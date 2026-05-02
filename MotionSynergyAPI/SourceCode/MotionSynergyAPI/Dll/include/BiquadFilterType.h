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
 *  @brief   Defines the different types of biquad filters available.
 */
enum class BiquadFilterType : int32_t
{
    None,    /*!< The biquad filter is not in use. */
    Unknown, /*!< The type of filter is unknown. The filter is in use on the motion controller but there is no filter in configuration with a matching set of coefficients.*/
    Notch,   /*!< A Notch filter */
    LowPass, /*!< A Low Pass filter */
    Lead,    /*!< The Lead component of a Lead/Lag compensator */
    Lag,     /*!< The Lag component of a Lead/Lag compensator */
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( BiquadFilterType )

} //  namespace imp
