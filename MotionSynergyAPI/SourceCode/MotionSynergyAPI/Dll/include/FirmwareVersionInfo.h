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
 *  @brief  Defines firmware version info for an axis.
 */
class FirmwareVersionInfo
{
public:
    /*!
     * @brief   Major version number.
     */
    uint16_t Major = 0;

    /*!
     * @brief   Minor version number.
     */
    uint16_t Minor = 0;

    /*!
     * @brief   Special version number.
     */
    uint16_t Special = 0;

    /*!
     * @brief   Full version number.
     * @details This is all version numbers combined into a single 32-bit number.
     *          It includes additional information such as the product family, motor type and numbers of axes.
     */
    uint32_t Full = 0;

    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "Major=" << Major << ", ";
        ss << "Minor=" << Minor << ", ";
        ss << "Special=" << Special << ", ";
        ss << "Full=0x" << std::uppercase << std::setfill( '0' ) << std::setw( 8 ) << std::hex << Full;

        return ss.str();
    }
};

inline std::ostream&
operator<<( std::ostream& stream, const FirmwareVersionInfo& value )
{
    stream << value.ToString();
    return stream;
}

} //  namespace imp
