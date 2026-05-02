/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDeviceFactory.h"
#include "IDeviceFactoryRegistration.h"
#include "SingletonLifetime.h"

namespace imp
{

class ILouganisDeviceFactory;

/*!
 *  @brief   Provides a static interface for retrieving the Device Factory singleton used
 *           for registration of device creation methods and for creating device instances.
 */
class DeviceFactorySingleton
{
public:
    /*!
     * @brief    Has the singleton instance been registered.
     * @returns  true if an instance has been registered, false if not.
     */
    static bool IsRegistered();

    /*!
     * @brief   Retrieve the statically-created device factory instance.
     * @returns Pointer to the device factory.
     */
    static IDeviceFactorySPtr GetDeviceFactory();


    /*!
     * @brief   Retrieve the statically-created device factory instance for
     *          the purpose of device create method registration.
     * @returns Pointer to the device factory.
     */
    static IDeviceFactoryRegistrationSPtr GetDeviceFactoryRegistration();


    /*!
     * @brief   Destroy the currently registered (i.e. static) device factory.
     * @details Intended for unit tests and clean application shutdown.
     */
    static void Destroy();


private:
    static void Create();

    using DeviceFactorySingletonLifetime =
        SingletonLifetime<IDeviceFactory, IDeviceFactoryRegistration, ILouganisDeviceFactory>;
    static DeviceFactorySingletonLifetime s_deviceFactory;
};

} //  namespace imp
