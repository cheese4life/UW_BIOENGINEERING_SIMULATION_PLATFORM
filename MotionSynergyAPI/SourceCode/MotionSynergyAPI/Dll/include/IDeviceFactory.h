/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ICCPDevice.h"
#include "IDriverManager.h"
#include <memory>
#include <string>


namespace imp
{

class IDeviceFactory;
/*! @brief  Shorthand for creating shared pointers of IDeviceFactory. */
using IDeviceFactorySPtr = std::shared_ptr<IDeviceFactory>;

/*!
 * @brief   Interface for device factory - builds associated devices based on configuration
 * @details Defines the interface used to configure, store, retrieve and run the
 *          device drivers in the system. At the moment this is Louganis - specific;
 *          there is no generic non-louganis driver to account for.
 */
class IDeviceFactory
{
public:
    /*! @brief Default constructor. */
    IDeviceFactory() = default;
    /*! @brief Default destructor. */
    virtual ~IDeviceFactory() = default;

    /*!
     * @brief   Creates the device by name, based on configuration data.
     * @param   [in]    name            The name of the device.
     * @param   [in]    pConfig         The configuration to load.
                                        Must be non-null.
     * @param   [in]    pDriverManager  The driver manager used to retrieve an appropriate driver for the device being created.
                                        Must be non-null.
     * @returns The created device.
     * @throws  runtime_error           The device name doesn't exist, or the driver for that device is not
     *                                  defined in config.
     */
    virtual ICCPDeviceSPtr
    Create( std::string const& name, IConfigurationSPtr pConfig, IDriverManagerSPtr pDriverManager ) = 0;
};

} //  namespace imp
