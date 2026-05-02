/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ISimulatorManager.h"


namespace imp
{
class ISimulatorFactory;
using ISimulatorFactorySPtr = std::shared_ptr<ISimulatorFactory>;

class IDeviceManager;
using IDeviceManagerSPtr = std::shared_ptr<IDeviceManager>;

class IDriverManager;
using IDriverManagerSPtr = std::shared_ptr<IDriverManager>;

class LouganisSimulatorManager : public ISimulatorManager
{
public:
    /*!
     *   @brief      Constructor
     *   @param[in]  pSimulatorFactory   Pointer to simulator factory for creating
     *                                   new simulators within this manager
     *   @param[in]  pDeviceManager      Pointer to device manager instance to associate
     *                                   with newly created simulators within this manager
     *   @param[in]  pDriverManager      Pointer to driver manager instance to associate
     *                                   with newly created simulators within this manager
     */
    LouganisSimulatorManager(
        ISimulatorFactorySPtr pSimulatorFactory,
        IDeviceManagerSPtr pDeviceManager,
        IDriverManagerSPtr pDriverManager );

    // overrides from ISimulatorManager

    bool Configure( IConfigurationSPtr pConfig ) override;
    void Initialize() override;
    void Shutdown() override;
    ISimulatorSPtr GetSimulator( const std::string& simulatorName ) override;
    NameToSimulatorMap GetSimulatorMap() override;
    void RegisterSimulator( const std::string& simulatorName, ISimulatorSPtr pSimulator ) override;

private:
    ISimulatorFactorySPtr m_pSimulatorFactory;
    IDeviceManagerSPtr m_pDeviceManager;
    IDriverManagerSPtr m_pDriverManager;
    NameToSimulatorMap m_simulatorMap;
    bool m_allowSimulation;
};

} //  namespace imp
