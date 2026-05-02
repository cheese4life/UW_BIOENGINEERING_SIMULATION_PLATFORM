/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <memory>

namespace imp
{

class IDriverManager;
using IDriverManagerSPtr = std::shared_ptr<IDriverManager>;
class IDeviceManager;
using IDeviceManagerSPtr = std::shared_ptr<IDeviceManager>;
class IDeviceFactory;
using IDeviceFactorySPtr = std::shared_ptr<IDeviceFactory>;

/*!
 *  @brief   Factory for creating an IDeviceManager.
 */
class DeviceManagerFactory
{
public:
    /*!
     * @brief   Factory method to create an IDeviceManager.
     * @param[in]   pDriverManager  The driver manager to associate devices to.
     * @param[in]   pDeviceFactory  The factory to pull devices from
     * @returns Shared pointer to the newly created IDeviceManager.
     */
    static IDeviceManagerSPtr Create( IDriverManagerSPtr pDriverManager, IDeviceFactorySPtr pDeviceFactory );
};

} //  namespace imp
