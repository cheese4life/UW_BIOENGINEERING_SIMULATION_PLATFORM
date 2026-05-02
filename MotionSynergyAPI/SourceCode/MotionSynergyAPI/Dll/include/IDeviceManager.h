/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ICCPDevice.h"
#include "IConfiguration.h"
#include "IDeviceFactory.h"
#include <memory>


namespace imp
{

class IDeviceManager;

/*! @brief  Shorthand for creating shared pointers of IDeviceManager. */
typedef std::shared_ptr<IDeviceManager> IDeviceManagerSPtr;

/*!
 *  @brief   Handles the registration and ownership of Devices and associated
 *           communication channels.
 */
class IDeviceManager
{
protected:
    /*! @brief Default constructor. */
    IDeviceManager() = default;

    /*! @brief Default destructor. */
    virtual ~IDeviceManager() = default;

public:
    using NameToDeviceMap = std::map<std::string, ICCPDeviceSPtr>;

    /*!
     *  @brief  Configure the Device manager, building all Devices and connections
     *          specified in configuration.
     *  @param[in]  pConfig     The configuration file to read from.
     *  @returns    True if configuration was successful; False otherwise.
     */
    virtual bool Configure( const IConfigurationSPtr& pConfig ) = 0;

    /*!
     * @brief   Retrieve a device Device by name.
     * @param[in]    name    The name of the device as configured.
     * @returns  Shared pointer to the retrieved device.
     * @throws   std::runtime_exception
     */
    virtual ICCPDeviceSPtr GetDevice( const std::string& name ) = 0;

    /*!
     * @brief   Retrieve a map of all devices.
     * @returns A map indexed by device name.
     */
    virtual NameToDeviceMap GetDeviceMap() = 0;

    /*!
     * @brief   Register an externally-created Device with the Device Manager.
     * @param[in]   deviceName       The unique name of the Device.
     * @param[in]   pDeviceInstance  The instance of the Device.
     */
    virtual void RegisterDevice( const std::string& deviceName, ICCPDeviceSPtr pDeviceInstance ) = 0;
};

} //  namespace imp
