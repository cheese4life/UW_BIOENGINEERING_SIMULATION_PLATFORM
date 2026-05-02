/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <memory>

namespace imp
{

class IDispatcher;
typedef std::shared_ptr<IDispatcher> IDispatcherSPtr;

class IAlertReporter;
typedef std::shared_ptr<IAlertReporter> IAlertReporterSPtr;

class IAlertDistribution;
typedef std::shared_ptr<IAlertDistribution> IAlertDistributionSPtr;

class IAlertDetailer;
typedef std::shared_ptr<IAlertDetailer> IAlertDetailerSPtr;


/*!
 *   @brief      Factory for creating Alert Classes.
 *   @details    Ensures that the implementation is not exposed
 *               outside of this library.
 *
 */
class AlertsFactory
{
public:
    AlertsFactory() = delete;

    /*!
    *   @brief  Create an AlertDistribution
                  NOTE: AlertDistribution needs to be configured before usage.
    *   @param  [in] dispatcher                 The dispatcher for performing callbacks on.
    *   @param  [in] alertDetailer              AlertDetailer for extending Alert descriptions.
    *   @return Shared pointer to a new AlertDistribution
    */
    static IAlertDistributionSPtr
    CreateAlertDistribution( IDispatcherSPtr dispatcher, IAlertDetailerSPtr alertDetailer );

    /*!
    *   @brief  Create CCPAlertReporter.
                 NOTE: Caller needs to install AlertReporter into the Global AlertReporter.
    *   @param  [in] alertDistribution          The AlertDistribution system for CCPAlertReport to use.
    *   @return Shared Pointer to a new AlertReporter.
    */
    static IAlertReporterSPtr CreateCCPAlertReporter( IAlertDistributionSPtr alertDistribution );
};

} //  namespace imp
