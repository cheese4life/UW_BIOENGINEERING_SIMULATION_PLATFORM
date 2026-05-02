/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "EnumClass.h"

#include <functional>

namespace imp
{

/*! @brief Enumeration that represent the state of a Digital IO bit */
enum class DigitalIOBitState
{
    LOW = 0,
    HIGH = 1,
    LAST_ENUM
};
ENUMCLASS_FUNCTIONS_DECL( DigitalIOBitState )

/*! @brief Enumeration to represent each bit of a Digital IO register. */
/*! @details Used as zero-based indices into an N-bit-wide register. */
enum class DigitalIOBit
{
    ZERO = 0,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    LAST_ENUM
};
ENUMCLASS_FUNCTIONS_DECL( DigitalIOBit )

using DigitalIORegisterCallback = std::function<void( uint8_t registerValue )>;
using DigitalIOBitCallback = std::function<void( DigitalIOBit bit, DigitalIOBitState bitState )>;


uint8_t BitMask( DigitalIOBit bit );

} //  namespace imp
