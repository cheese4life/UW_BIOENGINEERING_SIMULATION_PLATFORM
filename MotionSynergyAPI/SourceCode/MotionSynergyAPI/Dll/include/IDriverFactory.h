/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IConfiguration.h"
#include "IDeviceDriver.h"
#include <memory>

namespace imp
{

class IDriverFactory;

/*! @brief  Shorthand for creating shared pointers of IDriverFactory. */
typedef std::shared_ptr<IDriverFactory> IDriverFactorySPtr;

/*!
 * @brief Interface for driver factory - builds associated drivers based on
 *        configuration.
 */
class IDriverFactory
{
public:
    /*! @brief Default constructor. */
    IDriverFactory() = default;
    /*! @brief Default destructor. */
    virtual ~IDriverFactory() = default;

    /*!
     * @brief   Creates the driver by name, based on configuration data.
     * @param   [in]    name             The name of the device.
     * @param   [in]    pConfig          The configuration to load.
     * @returns The created driver.
     */
    virtual IDeviceDriverSPtr Create( const std::string& name, IConfigurationSPtr pConfig ) = 0;
};

} //  namespace imp
