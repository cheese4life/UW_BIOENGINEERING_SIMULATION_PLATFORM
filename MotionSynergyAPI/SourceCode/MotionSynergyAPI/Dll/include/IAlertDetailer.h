/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertDeclarations.h"
#include "AlertDetails.h"
#include <string>


namespace imp
{

/*!---------------------------------------------------------------------------
 * @class   IAlertDetailer
 * @brief   Provides interface to convert raw Alerts into ReportedAlerts.
 */
class IAlertDetailer
{
public:
    IAlertDetailer() = default;
    virtual ~IAlertDetailer() = default;

    /*!---------------------------------------------------------------------------
     * @brief   Configures the Detailer.
     */
    virtual void Configure() = 0;

    /*!---------------------------------------------------------------------------
     * @brief   Translates a Basic Alert into a ReportedAlert. This is returned as a SharedPtr so it can be expanded.
     * @param  reportId - unique report Id in CCP space.
     * @param  timeStamp - when the report was generated.
     * @param  source - A unique address of the Alert source.
     * @param  code - AlertCode.
     * @param  severity - the severity of the alert
     * @param  additionalDetails - string of additional alert details.
     * @param  contextId - the id that represent the context that the alert occurred in. Eg. the sample id
     * @param  expectExternallyCleared - true if Alert provides callback for when it is cleared.
     * @return  ReportedAlert SharedPointer.
     */
    virtual ReportedAlertConstSPtr GenerateReportedAlert(
        ::Alert::ReportId reportId,
        AlertTimestamp timeStamp,
        const std::string& source,
        const RawAlertCode& code,
        ::Alert::Severity::Enum severity,
        const std::string& additionalDetails,
        uint16_t contextId,
        bool expectExternallyCleared ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Add detail for an alert.
     * @param[in] item - Alert item details to add
     * @return bool - true if the code is added, false if code already exists and cannot be added to the repository.
     */
    virtual bool AddAlertDetail( const AlertDetails<std::string>& item ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Lookup the alert details for an alert by code.
     * @param[in] code - The alert's code.
     * @return A pointer to the alert's details on success, a nullptr if the alert details cannot be found.
     */
    virtual const AlertDetails<RawAlertCode>* GetAlertDetails( const RawAlertCode& code ) = 0;
};

typedef std::shared_ptr<IAlertDetailer> IAlertDetailerSPtr;

} //  namespace imp
