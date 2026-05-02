/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ISimulatorFactory.h"
#include <map>


namespace imp
{
using SimulatorTypeToCreateSimulatorFnsMap = std::map<std::string, ISimulatorFactoryRegistration::CreateSimulatorFn>;

/*!
 *  @brief   Factory implementation for creating new simulators
 */
class LouganisSimulatorFactory : public ISimulatorFactory
{
public:
    /*!
     *   @brief  Constructor
     */
    explicit LouganisSimulatorFactory() = default;

    // overrides from ISimulatorFactory

    ISimulatorSPtr Create(
        const std::string& simulatorName,
        IDeviceManagerSPtr pDeviceManager,
        IDriverManagerSPtr pDriverManager,
        IConfigurationSPtr pConfig ) override;

    // overrides from ISimulatorFactoryRegistration

    void RegisterSimulatorType( std::string const& simulatorType, CreateSimulatorFn createSimulatorFn ) override;

private:
    SimulatorTypeToCreateSimulatorFnsMap m_simulatorTypeToCreateSimulatorFns;
};

} //  namespace imp
