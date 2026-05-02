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

class IDriverManager;
using IDriverManagerSPtr = std::shared_ptr<IDriverManager>;

/*!
 *   @brief  Handles the creation and storage of drivers.
 */
class DriverManagerSingleton
{
public:
    /*!
     * @brief   Register the driver manager singleton instance.
     */
    static void Register( IDriverManagerSPtr pDriverManager );

    /*!
     * @brief    Has the singleton instance been registered.
     * @returns  true if an instance has been registered, false if not.
     */
    static bool IsRegistered();

    /*!
     * @brief   Retrieve the singleton driver manager instance.
     * @returns Pointer to the driver manager.
     */
    static IDriverManagerSPtr Get();

    /*!
     * @brief   Destroy the currently registered (i.e. static) driver manager.
     * @details Intended for unit tests and clean application shutdown.
     */
    static void Destroy();


private:
    static SingletonLifetime<IDriverManager> s_driverManager;
};

} //  namespace imp
