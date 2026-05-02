/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ILouganisDeviceFactory.h"
#include <map>


namespace imp
{

/*!
 * @brief   Builds Louganis devices based on their configuration.
 */
class LouganisDeviceFactory : public ILouganisDeviceFactory
{
public:
    /*! @brief Constructor. */
    explicit LouganisDeviceFactory() = default;
    /*! @brief Destructor. */
    ~LouganisDeviceFactory() override = default;

    //  IDeviceFactory

    ICCPDeviceSPtr
    Create( std::string const& deviceName, IConfigurationSPtr pConfig, IDriverManagerSPtr pDriverManager ) override;

    //  IDeviceFactoryRegistration

    void RegisterDeviceType( std::string const& deviceType, LouganisCreateDeviceFn createDeviceFn ) override;

private:
    using DeviceTypeToDeviceCreateFnsMap = std::map<std::string, LouganisCreateDeviceFn>;
    DeviceTypeToDeviceCreateFnsMap m_deviceTypeToCreateDeviceFns;
};

} //  namespace imp
