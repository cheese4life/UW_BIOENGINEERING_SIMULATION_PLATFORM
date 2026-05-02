/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <iomanip>
#include <sstream>

namespace imp
{

/*!
 *  @brief  Electrical properties retrieved from the PMD controller.
 */
class ElectricalProperties
{
public:
    /*!
     * @brief   Voltage provided by the power supply being used.
     */
    double BusVoltageV = 0;

    /*!
     * @brief   The encoder resolution for the axis.
     */
    double EncoderResolutionNm = 0;

    /*!
     * @brief   Sampling clock frequency based on the Servo Rate (20kHz = 51.2, 4kHz = 256).
     */
    double SampleTimeUs = 0;

    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "BusVoltageV=" << BusVoltageV << ", ";
        ss << "EncoderResolutionNm=" << EncoderResolutionNm << ", ";
        ss << "SampleTimeUs=" << SampleTimeUs;

        return ss.str();
    }
};

inline std::ostream&
operator<<( std::ostream& stream, const ElectricalProperties& value )
{
    stream << value.ToString();
    return stream;
}

} //  namespace imp
