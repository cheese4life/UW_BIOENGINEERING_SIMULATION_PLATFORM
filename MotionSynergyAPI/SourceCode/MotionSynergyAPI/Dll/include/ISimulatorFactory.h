/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ISimulatorFactoryRegistration.h"


namespace imp
{
class IConfiguration;
using IConfigurationSPtr = std::shared_ptr<IConfiguration>;

/*!
 *  @brief   Factory interface for creating new simulators
 */
class ISimulatorFactory : public ISimulatorFactoryRegistration
{
public:
    ISimulatorFactory() = default;
    virtual ~ISimulatorFactory() = default;

    /*!
     *   @brief      Creates a new simulator instance based on the named section
     *               in the configuration
     *   @param[in]  name            Name of the new simulator instance
     *   @param[in]  pDeviceManager  Pointer to device manager instance to associate
     *                               with the simulator
     *   @param[in]  pDriverManager  Pointer to driver manager instance to associate
     *                               with the simulator
     *   @param[in]  pConfig         Pointer to instance containing configuration
     *   @return     New instance of an simulator
     */
    virtual ISimulatorSPtr Create(
        const std::string& name,
        IDeviceManagerSPtr pDeviceManager,
        IDriverManagerSPtr pDriverManager,
        IConfigurationSPtr pConfig ) = 0;
};

} //  namespace imp
