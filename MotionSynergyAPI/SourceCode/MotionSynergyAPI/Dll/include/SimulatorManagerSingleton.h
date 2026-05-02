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
class ISimulatorManager;
using ISimulatorManagerSPtr = std::shared_ptr<ISimulatorManager>;

/*!
 *  @brief   Singleton static class for holding a single ISimulatorManager instance
 */
class SimulatorManagerSingleton
{
public:
    SimulatorManagerSingleton() = delete;
    ~SimulatorManagerSingleton() = delete;

    /*!
     *   @brief      Registers a new ISimulatorManager into this singleton
     *   @details    This method can only be called once. If called again a logic error
     *               exception will be thrown
     *   @param[in]  pSimulatorManager    New instance to register
     *   @return     void
     *   @throw      std::logic_error
     */
    static void Register( ISimulatorManagerSPtr pSimulatorManager );

    /*!
     *   @brief      Is there a registered instance inside this singleton?
     *   @return     "true" if there is a registered instance inside of this singleton
     */
    static bool IsRegistered();

    /*!
     *   @brief      Returns the ISimulatorManager interface associated with this singleton
     *   @details    This method requires that an instance has been registered before
     *               it is called, otherwise a logic error exception is thrown
     *   @return     ISimulatorManager interface representation of the single instance
     *   @throw      std::logic_error
     */
    static ISimulatorManagerSPtr Get();

    /*!
     *   @brief      Destroys any instance registered inside of this singleton
     *   @throw      void
     */
    static void Destroy();

private:
    static SingletonLifetime<ISimulatorManager> s_simulatorManager;
};

} //  namespace imp
