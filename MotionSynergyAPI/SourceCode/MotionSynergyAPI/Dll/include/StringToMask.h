/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <sstream>
#include <string>

namespace imp
{

/*!
 * @brief Convert a string containing an x-bit hex value (0xnnnnnnFF) to an integer.
 *
 * @tparam     TInteger      The integer type to convert the string to (e.g. uint32_t, uint16_t).
 * @param [in] inputString   The string to convert.
 *
 * @returns An n-bit integer set to the value described in the string.
 */
template<typename TInteger>
TInteger
StringToMask( const std::string& inputString )
{
    std::stringstream inStr( inputString );
    // Note that a uint32_t is used here rather than TInteger because if TInteger is uint8_t, 0xnn will be
    // parsed as 0.
    uint32_t retVal;
    inStr >> std::hex >> retVal;

    return static_cast<TInteger>( retVal );
}

} //  namespace imp
