/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "SingletonLifetime.h"


namespace imp
{
class ISimulatorFactory;
using ISimulatorFactorySPtr = std::shared_ptr<ISimulatorFactory>;

class ISimulatorFactoryRegistration;
using ISimulatorFactoryRegistrationSPtr = std::shared_ptr<ISimulatorFactoryRegistration>;

/*!
 *  @brief   Singleton static class for holding a single ISimulatorFactory instance
 */
class SimulatorFactorySingleton
{
public:
    SimulatorFactorySingleton() = delete;
    ~SimulatorFactorySingleton() = delete;

    /*!
     *   @brief      Registers a new ISimulatorFactory into this singleton
     *   @details    This method can only be called once. If called again a logic error
     *               exception will be thrown
     *   @param[in]  pSimulatorFactory    New instance to register
     *   @return     void
     *   @throw      std::logic_error
     */
    static void Register( ISimulatorFactorySPtr pSimulatorFactory );

    /*!
     * @brief    Has the singleton instance been registered.
     * @returns  true if an instance has been registered, false if not.
     */
    static bool IsRegistered();

    /*!
     *   @brief      Returns the ISimulatorFactory interface associated with this singleton
     *   @details    This method requires that an instance has been registered before
     *               it is called, otherwise a logic error exception is thrown
     *   @return     ISimulatorFactory interface representation of the single instance
     *   @throw      std::logic_error
     */
    static ISimulatorFactorySPtr GetSimulatorFactory();

    /*!
     *   @brief      Returns the ISimulatorFactoryRegistration interface associated with
     *               this singleton
     *   @details    This method requires that an instance has been registered before
     *               it is called, otherwise a logic error exception is thrown
     *   @return     ISimulatorFactoryRegistration interface representation of the single
     *               instance
     *   @throw      std::logic_error
     */
    static ISimulatorFactoryRegistrationSPtr GetSimulatorFactoryRegistration();

    /*!
     *   @brief      Destroys any instance registered inside of this singleton
     *   @return     void
     */
    static void Destroy();

private:
    using SimulatorFactorySingletonLifetime = SingletonLifetime<ISimulatorFactory, ISimulatorFactoryRegistration>;
    static SimulatorFactorySingletonLifetime s_simulatorFactory;
};

} //  namespace imp
