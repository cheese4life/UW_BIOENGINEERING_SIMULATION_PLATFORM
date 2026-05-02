/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include "CanBaudRate.h"
#include "SerialBaudRate.h"
#include "SerialProtocol.h"

#include <sstream>

namespace imp
{

/*!
 *  @brief  Defines the Serial and CAN communication settings for an axis.
 */
class CommunicationSettings
{
public:
    /*!
     * @brief Baud rate to use for serial mode (1,200-460,800)
     */
    SerialBaudRate SerialPortBaudRate;

    /*!
     * @brief Protocol to use for serial mode.
     */
    SerialProtocol SerialPortProtocol;

    /*!
     * @brief Multi-drop address for use with RS485 (0-31).
     */
    uint16_t SerialAddress;

    /*!
     * @brief Baud rate to use for CAN mode (0-7).
     */
    CanBaudRate CanPortBaudRate;

    /*!
     * @brief CAN Node ID address (0-127).
     */
    uint16_t CanAddress;


    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "SerialBaudRate=" << SerialBaudRateEnum::ToString( SerialPortBaudRate ) << ", ";
        ss << "SerialProtocol=" << SerialProtocolEnum::ToString( SerialPortProtocol ) << ", ";
        ss << "SerialAddress=" << SerialAddress << ", ";
        ss << "CanBaudRate=" << CanBaudRateEnum::ToString( CanPortBaudRate ) << ", ";
        ss << "CanAddress=" << CanAddress << ", ";

        return ss.str();
    }
};

inline std::ostream&
operator<<( std::ostream& stream, const CommunicationSettings& value )
{
    stream << value.ToString();
    return stream;
}

} //  namespace imp
