/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "SingletonLifetime.h"

namespace imp
{

class IDeviceManager;
using IDeviceManagerSPtr = std::shared_ptr<IDeviceManager>;

/*!
 *  @brief   Maintains the singleton instance of the IDeviceManager.
 */
class DeviceManagerSingleton
{
public:
    /*!
     * @brief   Register an instance of an IDeviceManager.
     *
     * @param [in] pDeviceManager The instance to register and take ownership of.
     */
    static void Register( IDeviceManagerSPtr pDeviceManager );

    /*!
     * @brief    Has the singleton instance been registered.
     * @returns  true if an instance has been registered, false if not.
     */
    static bool IsRegistered();

    /*!
     * @brief   Retrieve the singleton IDeviceManager instance.
     * @returns Pointer to the created IDeviceManager.
     */
    static IDeviceManagerSPtr Get();

    /*!
     * @brief   Destroy the currently registered (i.e. static) device manager.
     * @details Intended for unit tests and clean application shutdown.
     */
    static void Destroy();


private:
    static SingletonLifetime<IDeviceManager> s_deviceManager;
};

} //  namespace imp
