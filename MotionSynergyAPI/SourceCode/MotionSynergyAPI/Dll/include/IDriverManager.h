/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDeviceDriver.h"
#include <map>
#include <memory>
#include <string>


namespace imp
{

class IDriverFactory;
/*! @brief  Shorthand for creating shared pointers of IDriverFactory. */
typedef std::shared_ptr<IDriverFactory> IDriverFactorySPtr;

class IDriverManager;

/*! @brief  Shorthand for creating shared pointers of IDriverManager. */
typedef std::shared_ptr<IDriverManager> IDriverManagerSPtr;

/*!
 *  @brief   Handles the registration and ownership of Drivers and associated
 *           communication channels.
 */
class IDriverManager
{
public:
    using NameToDriverMap = std::map<std::string, IDeviceDriverSPtr>;

    /*! @brief Default constructor. */
    IDriverManager() = default;

    /*! @brief Default destructor. */
    virtual ~IDriverManager() = default;

    /*!
     * @brief   Create a device driver by device name.
     * @details Currently Louganis-specific as no generic CCP device definition exists.
     * @param[in]    driverName    The name of the driver as configured.
     * @param[in]    pConfig       Configuration.
     * @returns  Shared pointer to the retrieved device.
     * @throws   std::runtime_exception
     */
    virtual IDeviceDriverSPtr CreateDriver( const std::string& driverName, const IConfigurationSPtr& pConfig ) = 0;

    /*!
     * @brief   Retrieve a map of all drivers.
     * @returns A map indexed by driver name.
     */
    virtual NameToDriverMap GetDriverMap() = 0;

    /*!
     * @brief   Register an externally-created driver with the Driver Manager.
     * @param[in]   driverName      The unique name of the driver.
     * @param[in]   driverInstance  The instance of the driver.
     */
    virtual void RegisterDriver( const std::string& driverName, IDeviceDriverSPtr driverInstance ) = 0;

    /*!
     * @brief    Start the driver layer thread.
     * @details  The driver layer thread and dispatcher is used the execute
     *           device to driver and driver to device calls.
     */
    virtual void StartDriverThread() = 0;

    /*!
     * @brief    Stop the driver layer thread.
     * @details  This will send a begin stop and wait for the dispatcher queue to finish.
     *           Blocks the caller till the driver thread joins.
     */
    virtual void StopDriverThread() = 0;

    /*!
     * @brief    Detach all drivers from their associated devices.
     * @details  This will call Detach() on each driver, allowing the driver
     *           to unregister any remaining callbacks its associated device has
     *           registered. The DeviceManager will call this just prior to
     *           deleting all device instances. It's used to ensure there are no
     *           dangling callbacks which may get invoked prior to the drivers
     *           being destroyed.s
     */
    virtual void Detach() = 0;
};

} //  namespace imp
