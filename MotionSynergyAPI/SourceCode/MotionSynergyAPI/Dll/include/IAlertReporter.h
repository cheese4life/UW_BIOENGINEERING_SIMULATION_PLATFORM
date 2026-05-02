/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertDeclarations.h"
#include "AlertTypes.h"
#include "AsyncCallback.h"


namespace imp
{

class IAlertReporter;
typedef std::shared_ptr<IAlertReporter> IAlertReporterSPtr;

/*!---------------------------------------------------------------------------
 * @class   IAlertReporter
 * @brief   Provides interface to raise and clear alerts
 */
class IAlertReporter
{
public:
    IAlertReporter() = default;
    virtual ~IAlertReporter() = default;


    //----------------------------------------------------------------------------
    // IAlertReporter Interface
public:
    /*!---------------------------------------------------------------------------
     * @brief  Raises the specified alert, from the specified source, with free form additional details
     * @param  sourceId - the unique id used to specify the source of the Alert
     * @param  alertCode - the unique code that identifies the alert to be raised
     * @param  severity - the severity of the alert
     * @param  additionalDetails - a reference to a string containing the additional details
     * @param  contextId - an id that can be added at by the source or system that identifies the context that
     *                     alert was raised in. An example would be a sampleId or workflow step.
     * @param  callback - this callback will be invoked if the alert is externally cleared
     * @return  the id that represents the alert instance id, this can be stored and used to clear the alert
     */
    virtual ::Alert::ReportId RaiseAlert(
        const std::string& sourceId,
        RawAlertCode alertCode,
        ::Alert::Severity::Enum severity,
        const std::string& additionalDetails,
        uint16_t contextId,
        IPLKeystone::AsyncCallback<::Alert::ReportId>* callback ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Raises the specified alert, from the specified source, with free form additional details
     * @param  sourceId - the unique id used to specify the source of the Alert
     * @param  alertCode - the unique code that identifies the alert to be raised
     * @param  additionalDetails - a reference to a string containing the additional details
     * @param  contextId - an id that can be added at by the source or system that identifies the context that
     *                     alert was raised in. An example would be a sampleId or workflow step.
     * @param  callback - this callback will be invoked if the alert is externally cleared
     * @return  the id that represents the alert instance id, this can be stored and used to clear the alert
     */
    virtual ::Alert::ReportId RaiseAlert(
        const std::string& sourceId,
        RawAlertCode alertCode,
        const std::string& additionalDetails,
        uint16_t contextId,
        IPLKeystone::AsyncCallback<::Alert::ReportId>* callback ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief   Clears the specified alert
     * @param  alertId - the unique code that identifies the INSTANCE to be cleared, this is the id returned by the
     *                   raise call
     */
    virtual void ClearAlert( ::Alert::ReportId alertId ) = 0;
};

} //  namespace imp
