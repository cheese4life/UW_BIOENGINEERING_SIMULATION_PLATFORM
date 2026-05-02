/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ICCPDevice.h"
#include "IDeviceDriver.h"
#include "IDispatcher.h"
#include <string>


namespace imp
{

class IDeviceFactoryRegistration;

/*! @brief  Shorthand for creating shared pointers of IDeviceFactoryRegistration. */
using IDeviceFactoryRegistrationSPtr = std::shared_ptr<IDeviceFactoryRegistration>;

using LouganisCreateDeviceFn = std::function<ICCPDeviceSPtr( const std::string& name, IDeviceDriverSPtr pDriver )>;

/*!
 * @brief Interface for registering device create functions with the device factory.
 */
class IDeviceFactoryRegistration
{
public:
    /*! @brief Default constructor. */
    IDeviceFactoryRegistration() = default;
    /*! @brief Default destructor. */
    virtual ~IDeviceFactoryRegistration() = default;

    /*!
     * @brief   Register a function which will be called to create a device instance of a particular type.
     * @param   [in]    deviceType  The type of the device.
     * @param   [in]    createDeviceFn The function to call to create the device instance.
     * @throws  runtime_error   The deviceType has already been registered.
     */
    virtual void RegisterDeviceType( std::string const& deviceType, LouganisCreateDeviceFn createDeviceFn ) = 0;
};

} //  namespace imp
