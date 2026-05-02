/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDriverFactory.h"
#include "IDriverFactoryRegistration.h"
#include <map>


namespace imp
{

/*!
 * @brief   Builds Louganis device drivers based on their configuration.
 */
class LouganisDriverFactory
    : public IDriverFactory
    , public IDriverFactoryRegistration
{
public:
    /*! @brief Constructor. */
    LouganisDriverFactory() = default;
    /*! @brief Default destructor. */
    ~LouganisDriverFactory() override = default;

    IDeviceDriverSPtr Create( const std::string& driverName, IConfigurationSPtr pConfig ) override;

public: //  IDriverFactoryRegistration
    void RegisterDriverType( std::string const& driverType, LouganisCreateDriverFn createDeviceFn ) override;

private:
    using DriverTypeToCreateDriverFnsMap = std::map<std::string, LouganisCreateDriverFn>;
    DriverTypeToCreateDriverFnsMap m_driverTypeToCreateDriverFns;
};

} //  namespace imp
