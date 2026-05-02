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
 *  @brief  Defines position loop parameters for an axis.
 */
class PositionLoopSettings
{
public:
    /*!
     * @brief PID Proportional Gain
     */
    int32_t Kp = 0;

    /*!
     * @brief PID Integrator Gain
     */
    int32_t Ki = 0;

    /*!
     * @brief PID Integrator Limit
     */
    int32_t Ilimit = 0;

    /*!
     * @brief PID Derivative Gain
     */
    int32_t Kd = 0;

    /*!
     * @brief PID Derivative Time
     */
    int32_t DerivativeTime = 0;

    /*!
     * @brief PID Output Gain
     */
    int32_t Kout = 0;

    /*!
     * @brief Velocity Feedforward Gain
     */
    int32_t Kvff = 0;

    /*!
     * @brief Acceleration Feedforward Gain
     */
    int32_t Kaff = 0;

    /*!
     * @brief Whether position loop tuning is available for the current motor type.
     * As a result from the get function, the PID parameters are valid only if this is true.
     * Can be ignored when setting/saving parameters.
     */
    bool IsPositionLoopTuningAvailable = false;

    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "IsPositionLoopTuningAvailable=" << IsPositionLoopTuningAvailable << ", ";
        ss << "Kp=" << Kp << ", ";
        ss << "Ki=" << Ki << ", ";
        ss << "Ilimit=" << Ilimit << ", ";
        ss << "Kd=" << Kd << ", ";
        ss << "DerivativeTime=" << DerivativeTime << ", ";
        ss << "Kout=" << Kout << ", ";
        ss << "Kvff=" << Kvff << ", ";
        ss << "Kaff=" << Kaff;

        return ss.str();
    }
};

inline std::ostream&
operator<<( std::ostream& stream, const PositionLoopSettings& value )
{
    stream << value.ToString();
    return stream;
}

} //  namespace imp
