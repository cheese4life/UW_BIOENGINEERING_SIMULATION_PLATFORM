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

class ReportedAlert;
using ReportedAlertConstSPtr = std::shared_ptr<const ReportedAlert>;

class IAlertFilter;
using IAlertFilterSPtr = std::shared_ptr<IAlertFilter>;

/*!---------------------------------------------------------------------------
 * @class   IAlertFilter
 * @brief   Provides interface for an 'Alert Filter' which can be registered
 *          with the AlertDistribution singleton in order to filter out or modify specific
 *          alerts prior to being distributed to registered listeners.
 */
class IAlertFilter
{
public:
    IAlertFilter() = default;
    virtual ~IAlertFilter() = default;

    /*!
     * @brief     Method called by AlertDistribution whenever an alert is raised.
     * @details   This method can return pAlert to pass the alert on unchanged, return a
     *            nullptr to filter out the alert or return a copy of pAlert with one or more modified fields (i.e this would
     *            allow the Severity to be changed).
     * @param[in] pAlert - The alert being considered for filtering.
     * @return    The pAlert if the alert is not to be filtered, nullptr if the alert should be filtered.
     */
    virtual ReportedAlertConstSPtr FilterAlert( ReportedAlertConstSPtr pAlert ) = 0;
};


} //  namespace imp
