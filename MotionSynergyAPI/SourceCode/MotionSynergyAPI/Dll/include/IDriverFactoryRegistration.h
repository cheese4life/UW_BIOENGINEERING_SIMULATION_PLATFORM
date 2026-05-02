/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDeviceDriver.h"
#include "IDispatcher.h"
#include <memory>
#include <string>


namespace imp
{

class ILouganisSCPBoard;
class IDriverFactoryRegistration;

using LouganisCreateDriverFn =
    std::function<std::shared_ptr<IDeviceDriver>( std::string const& driverName, IConfigurationSPtr pConfig )>;


/*!
 * @brief Interface for registering device create functions with the device factory.
 */
class IDriverFactoryRegistration
{
public:
    /*! @brief Default constructor. */
    IDriverFactoryRegistration() = default;
    /*! @brief Default destructor. */
    virtual ~IDriverFactoryRegistration() = default;

    /*!
     * @brief   Register a function which will be called to create a device driver instance of a particular type.
     * @param   [in]    deviceType  The type of the device.
     * @param   [in]    createDeviceFn The function to call to create the device instance.
     */
    virtual void RegisterDriverType( std::string const& deviceType, LouganisCreateDriverFn createDeviceFn ) = 0;
};

} //  namespace imp
