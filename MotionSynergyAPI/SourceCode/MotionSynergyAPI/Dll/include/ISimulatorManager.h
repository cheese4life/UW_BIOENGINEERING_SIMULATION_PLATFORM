/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <map>
#include <memory>
#include <string>


namespace imp
{
class ISimulator;
using ISimulatorSPtr = std::shared_ptr<ISimulator>;

class IConfiguration;
using IConfigurationSPtr = std::shared_ptr<IConfiguration>;

/*!
 *   @brief  Interface for managing simulators on the instrument
 */
class ISimulatorManager
{
public:
    using NameToSimulatorMap = std::map<std::string, ISimulatorSPtr>;

    ISimulatorManager() = default;
    virtual ~ISimulatorManager() = default;

    /*!
     *   @brief      Creates and configures all simulators defined in the instrument
     *               configuration
     *   @details    If the "Instrument.AllowSimulation" setting is omitted, or explicitly
     *               set to false, simulators are not created/configured
     *   @param[in]  pConfig         Pointer to instance containing configuration
     *   @return     "true" if configuration was successful
     */
    virtual bool Configure( IConfigurationSPtr pConfig ) = 0;

    /*!
     *   @brief      Initialize all simulators and makes them ready for use.
     *   @return     void
     */
    virtual void Initialize() = 0;

    /*!
     *   @brief      Shutdown all simulators
     *   @return     void
     */
    virtual void Shutdown() = 0;

    /*!
     *   @brief      Gets the named simulator from the manager
     *   @details    If the named simulator does not exist, a runtime error is thrown
     *   @param[in]  name        Name of the simulator to retrieve
     *   @return     Pointer to the simulator instance
     *   @throw      std::runtime_error
     */
    virtual ISimulatorSPtr GetSimulator( const std::string& name ) = 0;

    /*!
     *   @brief      Gets the dictionary containing a map of simulator names to instances
     *   @return     A std::map<std::string, ISimulatorSPtr> with all created simulators
     */
    virtual NameToSimulatorMap GetSimulatorMap() = 0;

    /*!
     *   @brief      Registers the named simulator into this manager
     *   @details    If the named simulator already exists, a runtime error is thrown
     *   @param[in]  name        Name of the simulator
     *   @param[in]  pSimulator   Pointer to the simulator to register into this manager
     *   @return     void
     *   @throw      std::runtime_error
     */
    virtual void RegisterSimulator( const std::string& name, ISimulatorSPtr pSimulator ) = 0;
};

} //  namespace imp
