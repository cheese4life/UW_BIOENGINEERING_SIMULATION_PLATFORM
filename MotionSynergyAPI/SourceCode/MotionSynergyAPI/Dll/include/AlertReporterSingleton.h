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

class IAlertReporter;
using IAlertReporterSPtr = std::shared_ptr<IAlertReporter>;


/*!
 * @class   AlertReporterSingleton
 * @brief   Maintains the singleton instance of the IAlertReporter.
 */
class AlertReporterSingleton
{
public:
    AlertReporterSingleton() = delete;

    /*!
     * @brief   Register an instance of an IAlertReporter.
     *
     * @param [in] pReporter The instance to register and take ownership of.
     */
    static void Register( IAlertReporterSPtr pReporter );

    /*!
     * @brief    Has the singleton instance been registered.
     * @returns  true if an instance has been registered, false if not.
     */
    static bool IsRegistered();

    /*!
     * @brief   Retrieve the singleton IAlertReporter instance.
     * @returns Pointer to the created IAlertReporter.
     */
    static IAlertReporterSPtr Get();

    /*!
     * @brief  Prepares for shutdown, releases the alert reporter pointer.
     */
    static void Destroy();

private:
    static SingletonLifetime<IAlertReporter> s_alertReporter;
};

} //  namespace imp
