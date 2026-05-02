/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <stdexcept>
#include <string>


namespace imp
{

/*!
 *  @brief   Exception thrown when a device encounters an error.
 */
class DeviceException : public std::runtime_error
{
public:
    explicit DeviceException(
        const std::string& deviceType,
        const std::string& deviceName,
        const std::string& responseCode,
        std::string const& what )
        : runtime_error( what )
        , m_deviceType( deviceType )
        , m_deviceName( deviceName )
        , m_responseCode( responseCode )
    {
    }

    virtual ~DeviceException() = default;

    /*!
     *  @brief Returns the type of device which generated the exception.
     */
    std::string
    GetDeviceType() const
    {
        return m_deviceType;
    }

    /*!
     *  @brief Returns the name of device instance which generated the exception.
     */
    std::string
    GetDeviceName() const
    {
        return m_deviceName;
    }

    /*!
     *  @brief Returns the response code (in string form) indicating the type of error.
     */
    std::string
    GetResponseCode() const
    {
        return m_responseCode;
    }

private:
    std::string m_deviceType;
    std::string m_deviceName;
    std::string m_responseCode;
};

#define THROW_DEVICE_EXCEPTION( DeviceResponseCode, MessageString )                             \
    {                                                                                           \
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();         \
        std::string _currentTime = boost::posix_time::to_simple_string( now );                  \
        std::ostringstream _ostr;                                                               \
        _ostr << "Exception: " << MessageString << ", Type: " << Type() << ", Name: " << Name() \
              << ", File: " << std::string( __FILE__ ) << ", Line: " << __LINE__                \
              << ", Function: " << BOOST_CURRENT_FUNCTION << ", Time: " << _currentTime;        \
        throw DeviceException( Type(), Name(), DeviceResponseCode, _ostr.str() );               \
    }


} //  namespace imp
