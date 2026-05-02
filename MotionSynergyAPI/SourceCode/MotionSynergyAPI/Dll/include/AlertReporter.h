/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertDeclarations.h"
#include "AsyncCallback.h"
#include <string>


namespace imp
{

/*!
 * @class   AlertReporter
 * @brief   A static interface that allows the raising and clearing of alerts
 */
class AlertReporter
{
public:
    AlertReporter() = delete;

    /*!
     * @brief  Clears the alert specified by the provided report id. This will only have an effect when a
     *         reporter is attached.
     *
     *         If it is called with no reporter it will not fail but only log
     */
    static void ClearAlert( ::Alert::ReportId alertId );

    /*!
     * @brief  Raise an alert with the specified attributes (eg. alert code). By
     *         providing a callback pointer the client declares that they expect that
     *         the alert may be externally cleared AND they wish to be notified
     * @param[in]  sourceId - The system wide unique identifier of the component raising the alert
     * @param[in]  alertCode - The code of the alert to be raised.
     * @param[in]  severity - An informational severity provided by the client, indicates what the client is treating the alert as
     * @param[in]  additionalDetails - free form extra detail added by the 'raiser'
     * @param[in]  contextId - the id that represent the context that the alert occurred in. Eg. the sample id
     * @param[in]  alertClearedCallback - this callback will be invoked if the alert is externally cleared
     * @return the report id, the unique identifier of the raised alert
     */
    static ::Alert::ReportId RaiseAlert(
        std::string const& sourceId,
        RawAlertCode const& alertCode,
        ::Alert::Severity::Enum severity,
        std::string const& additionalDetails = std::string(),
        uint16_t contextId = 0u,
        IPLKeystone::AsyncCallback<::Alert::ReportId>* alertClearedCallback = nullptr );

    /*!
     * @brief  Raise an alert with the specified attributes (eg. alert code). By
     *         providing a callback pointer the client declares that they expect that
     *         the alert may be externally cleared AND they wish to be notified
     * @param[in]  sourceId - The system wide unique identifier of the component raising the alert
     * @param[in]  alertCode - The code of the alert to be raised.
     * @param[in]  additionalDetails - free form extra detail added by the 'raiser'
     * @param[in]  contextId - the id that represent the context that the alert occurred in. Eg. the sample id
     * @param[in]  alertClearedCallback - this callback will be invoked if the alert is externally cleared
     * @return the report id, the unique identifier of the raised alert
     */
    static ::Alert::ReportId RaiseAlert(
        std::string const& sourceId,
        RawAlertCode const& alertCode,
        std::string const& additionalDetails = std::string(),
        uint16_t contextId = 0u,
        IPLKeystone::AsyncCallback<::Alert::ReportId>* alertClearedCallback = nullptr );
};

} //  namespace imp
