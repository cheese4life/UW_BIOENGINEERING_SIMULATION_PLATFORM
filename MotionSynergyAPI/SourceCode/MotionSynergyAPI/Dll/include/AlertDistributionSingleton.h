/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "SingletonLifetime.h"

#include <memory>

namespace imp
{

class IAlertDistribution;
using IAlertDistributionSPtr = std::shared_ptr<IAlertDistribution>;


/*!
 * @class   AlertDistributionSingleton
 * @brief   Maintains the singleton instance of the IAlertDistribution.
 */
class AlertDistributionSingleton
{
public:
    AlertDistributionSingleton() = delete;

    /*!
     * @brief   Register an instance of an IAlertDistribution.
     *
     * @param [in] pAlertDistribution The instance to register and take ownership of.
     */
    static void Register( IAlertDistributionSPtr pAlertDistribution );

    /*!
     * @brief    Has the singleton instance been registered.
     * @returns  true if an instance has been registered, false if not.
     */
    static bool IsRegistered();

    /*!
     * @brief   Retrieve the singleton IAlertDistribution instance.
     * @returns Pointer to the created IAlertDistribution.
     */
    static IAlertDistributionSPtr Get();

    /*!
     * @brief   Destroy the currently registered (i.e. static) alert distribution.
     * @details Intended for unit tests and clean application shutdown.
     */
    static void Destroy();

private:
    static SingletonLifetime<IAlertDistribution> s_alertDistribution;
};

} //  namespace imp
