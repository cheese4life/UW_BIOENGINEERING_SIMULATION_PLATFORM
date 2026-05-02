/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AsyncActivity.h"
#include "CCPDevice.h"
#include "CachedValue.h"
#include "DeviceException.h"
#include "DeviceResponseType.h"
#include "LoggerMacros.h"
#include "StandardDeviceResponse.h"

namespace imp
{
class DriverResponseType;

/*
 * @brief Helper class to manage the asynchronous retrieval of a property from a hardware device.
 *
 */
template<class TPropertyType>
class AsyncPropertyGet
{
public:
    /*!
     *   @brief  Constructor starts the async activity.
     */
    explicit AsyncPropertyGet( ICCPDevice& device, CachedValue<TPropertyType>& cachedValue )
        : m_device( device )
        , m_cachedValue( cachedValue )
    {
        m_activity.Begin();
    }

    /*!
     *   @brief  Returns the device type - used when throwing a device exception
     */
    std::string
    Type() const
    {
        return m_device.Type();
    }

    /*!
     *   @brief  Returns the device name - used when throwing a device exception
     */
    std::string
    Name() const
    {
        return m_device.Name();
    }

    /*!
     *   @brief  Returns a lambda callback to invoke when the response from the driver is received.
     */
    auto
    GetCallback()
    {
        return [ this ]( DriverResponseType const response, TPropertyType newValue )
        {
            m_deviceResponse = CCPDevice::TranslateDriverResponse( response );
            if ( m_deviceResponse == StandardDeviceResponse::DeviceSuccess )
            {
                m_cachedValue = newValue;
            }
            m_activity.Finished();
        };
    }

    /*!
     *   @brief  Block waiting for the response from the driver to be received.
     */
    void
    Wait()
    {
        // Wait for callback
        auto result = m_activity.Wait( m_device.DefaultTimeout() );
        if ( result == WaitResult::Timeout )
        {
            std::string const errStr = Name() + ": Timeout occurred while waiting for get property response.";
            LOG_ERROR( ComponentName, errStr );
            THROW_DEVICE_EXCEPTION( DeviceResponseType( StandardDeviceResponse::DeviceTimeout ).ToString(), errStr );
        }

        if ( result == WaitResult::Success )
        {
            if ( m_deviceResponse != StandardDeviceResponse::DeviceSuccess )
            {
                std::string const errStr =
                    std::string( Name() + ": Get property failed.  Response: " ) + m_deviceResponse.ToString();
                LOG_ERROR( ComponentName, errStr );
                THROW_DEVICE_EXCEPTION( m_deviceResponse.ToString(), errStr );
            }
        }
    }

private:
    ICCPDevice& m_device;
    CachedValue<TPropertyType>& m_cachedValue;
    AsyncActivity m_activity;
    DeviceResponseType m_deviceResponse;
};

} //  namespace imp
