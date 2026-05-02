/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "BiquadFilterType.h"

#include <sstream>


namespace imp
{

/*!
 *  @brief  Defines a single Biquad Filter
 */
class BiquadFilterDefinition
{
public:
    /*!
     * @brief The type of filter.
     */
    BiquadFilterType FilterType = BiquadFilterType::None;

    /*!
     * @brief The frequency parameter used to generate the filter coefficients. Must be greater than zero.
     */
    double Frequency = 0.0;

    /*!
     * @brief The quality factor parameter used to generate the filter coefficients. Must be greater than zero.
     */
    double QualityFactor = 1.0;

    /*!
     * @brief The alpha or beta parameter used with Lead/Lag filters. Lead Alpha must be 0.1 <= Alpha <= 0.9, and Lag Beta must be .
     */
    double AlphaOrBeta = 1.0;

    /*!
     * @brief B0 Coefficient
     */
    int32_t B0Coefficient = 0;

    /*!
     * @brief B1 Coefficient
     */
    int32_t B1Coefficient = 0;

    /*!
     * @brief B2 Coefficient
     */
    int32_t B2Coefficient = 0;

    /*!
     * @brief A1 Coefficient
     */
    int32_t A1Coefficient = 0;

    /*!
     * @brief A2 Coefficient
     */
    int32_t A2Coefficient = 0;

    /*!
     * @brief K Coefficient
     */
    int32_t KCoefficient = 0;


    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "FilterType=" << BiquadFilterTypeEnum::ToString( FilterType ) << ", ";
        ss << "Frequency=" << Frequency << ", ";
        ss << "QualityFactor=" << QualityFactor << ", ";
        ss << "AlphaOrBeta=" << AlphaOrBeta << ", ";
        ss << "B0=" << B0Coefficient << ", ";
        ss << "B1=" << B1Coefficient << ", ";
        ss << "B2=" << B2Coefficient << ", ";
        ss << "A1=" << A1Coefficient << ", ";
        ss << "A2=" << A2Coefficient << ", ";
        ss << "K=" << KCoefficient;

        return ss.str();
    }
};

inline std::ostream&
operator<<( std::ostream& stream, const BiquadFilterDefinition& value )
{
    stream << value.ToString();
    return stream;
}

} //  namespace imp
