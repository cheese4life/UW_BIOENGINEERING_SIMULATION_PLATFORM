/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <cstdint>
#include <sstream>

namespace imp
{

/*!
 *  @brief  Defines phase angle information
 */
class PhaseAngleInfo
{
public:
    /*!
     * @brief   Raw phase angle - in encoder counts or microsteps.
     */
    uint16_t PhaseAngleRaw = 0;

    /*!
     * @brief   Phase angle in degrees.
     */
    uint16_t PhaseAngleDeg = 0;

    /*!
     * @brief   Whether the phase angle is valid.
     */
    bool IsValid = false;

    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "PhaseAngle(Raw)=" << PhaseAngleRaw << ", ";
        ss << "PhaseAngle(Deg)=" << PhaseAngleDeg << ", ";
        ss << "IsValid=" << IsValid;

        return ss.str();
    }
};

inline std::ostream&
operator<<( std::ostream& stream, const PhaseAngleInfo& value )
{
    stream << value.ToString();
    return stream;
}

} //  namespace imp
