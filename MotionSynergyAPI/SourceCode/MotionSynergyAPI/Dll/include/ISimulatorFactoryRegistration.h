/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <functional>
#include <memory>
#include <string>


namespace imp
{

class ISimulator;
using ISimulatorSPtr = std::shared_ptr<ISimulator>;

class IDeviceManager;
using IDeviceManagerSPtr = std::shared_ptr<IDeviceManager>;

class IDriverManager;
using IDriverManagerSPtr = std::shared_ptr<IDriverManager>;

/*!
 *  @brief   Factory interface for registering new simulator types
 */
class ISimulatorFactoryRegistration
{
public:
    using CreateSimulatorFn =
        std::function<ISimulatorSPtr( const std::string&, IDeviceManagerSPtr, IDriverManagerSPtr )>;

    ISimulatorFactoryRegistration() = default;
    virtual ~ISimulatorFactoryRegistration() = default;

    /*!
     *   @brief      Registers a new simulator creation function with the type name
     *   @param[in]  SimulatorType        Name of the simulator type to register
     *   @param[in]  CreateSimulatorFn    Static pointer to the function that creates
     *                                   a new instance of the simulator type
     *   @return     void
     */
    virtual void RegisterSimulatorType( std::string const& SimulatorType, CreateSimulatorFn CreateSimulatorFn ) = 0;
};

} //  namespace imp
