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
 *  @brief   Defines the different UDP modes available.
 */
enum class UDPMode : int32_t
{
    //None,     /*!< The UDP mode is not in use. */ // JS: Removed since, if a UDP profile is loaded, it is always in use.    
    Time,       /*!< The UDP mode is time based. */
    Encoder,    /*!< The UDP mode is encoder based. */
    SineWave,   /*!< The UDP mode is sine wave based. */
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( UDPMode )

} //  namespace imp
