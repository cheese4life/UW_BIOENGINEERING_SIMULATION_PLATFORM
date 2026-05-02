/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "BiquadFilterDefinition.h"

#include <sstream>

namespace imp
{

/*!
 *  @brief  Defines biquad filter parameters for an axis.
 */
class BiquadFilterSettings
{
public:
    /*!
     * @brief Defines the first Biquad Filter (may be none if Filter 1 is not used)
     */
    BiquadFilterDefinition Filter1;

    /*!
     * @brief Defines the second Biquad Filter (may be none if Filter 2 is not used)
     */
    BiquadFilterDefinition Filter2;

    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "Filter1={" << Filter1 << "}, ";
        ss << "Filter2={" << Filter2 << "}";

        return ss.str();
    }
};

inline std::ostream&
operator<<( std::ostream& stream, const BiquadFilterSettings& value )
{
    stream << value.ToString();
    return stream;
}

} //  namespace imp
