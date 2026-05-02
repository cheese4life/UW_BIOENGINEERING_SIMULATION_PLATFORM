/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <sstream>

namespace imp
{

/*!
 *  @brief  Defines current loop parameters for an axis.
 */
class CurrentLoopSettings
{
public:
    /*!
     * @brief Proportional Gain.
     */
    uint16_t KpCurrent = 0;

    /*!
     * @brief Integrator Gain.
     */
    uint16_t KiCurrent = 0;

    /*!
     * @brief Integrator Sum Limit.
     */
    uint16_t IlimitCurrent = 0;

    /*!
     * @brief Whether current loop tuning is available for the current motor type.
     * As a result from the get function, the parameters are valid only if this is true.
     * Can be ignored when setting/saving parameters.
     */
    bool IsCurrentLoopTuningAvailable = false;

    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "IsCurrentLoopTuningAvailable" << IsCurrentLoopTuningAvailable << ", ";
        ss << "KpCurrent=" << KpCurrent << ", ";
        ss << "KiCurrent=" << KiCurrent << ", ";
        ss << "IlimitCurrent=" << IlimitCurrent;

        return ss.str();
    }
};

inline std::ostream&
operator<<( std::ostream& stream, const CurrentLoopSettings& value )
{
    stream << value.ToString();
    return stream;
}

} //  namespace imp
