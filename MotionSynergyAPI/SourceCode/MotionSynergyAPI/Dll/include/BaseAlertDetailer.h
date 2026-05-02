/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IAlertDetailer.h"
#include "RawAlertCode.h"
#include <map>

namespace imp
{

/*!---------------------------------------------------------------------------
 * @class   BaseAlertDetailer
 * @brief   Generates ReportedAlert from Raw Alert.
 */
class BaseAlertDetailer : public IAlertDetailer
{
public:
    BaseAlertDetailer() = default;
    ~BaseAlertDetailer() override = default;

    /*!---------------------------------------------------------------------------
     * @brief  Translates a Basic Alert into a ReportedAlert. This is returned as a SharedPtr so it can be expanded.
     * @param  reportId - the unique identifier for the Alert.
     * @param  timeStamp - epoch timestamp in seconds.
     * @param  source - the unique identifier of the component that raised the alert
     * @param  code - the alert code to be raised
     * @param  severity - the alert severity to be raised
     * @param  additionalDetails - free form extra detail added by the 'raiser'
     * @param  contextId - the id that represent the context that the alert occurred in. Eg. the sample id
     * @param  expectExternallyCleared - will be cleared by someone other than reporter.
     * @return  ReportedAlert SharedPointer.
     * @see IAlertDetailer
     */
    ReportedAlertConstSPtr GenerateReportedAlert(
        ::Alert::ReportId reportId,
        AlertTimestamp timeStamp,
        const std::string& source,
        const RawAlertCode& code,
        ::Alert::Severity::Enum severity,
        const std::string& additionalDetails,
        uint16_t contextId,
        bool expectExternallyCleared ) override;

    /*!---------------------------------------------------------------------------
     * @brief  Add detail for an alert.
     * @param  item - An AlertDetails describing the alert.
     * @return bool - true if the code is added, false if code already exists and cannot be added to the repository.
     * @see IAlertDetailer
     */
    bool AddAlertDetail( const AlertDetails<std::string>& item ) override;

    const AlertDetails<RawAlertCode>* GetAlertDetails( const RawAlertCode& code ) override;

private:
    void ConvertCachedAlertDetails();

    typedef std::map<RawAlertCode, AlertDetails<RawAlertCode>> AlertMapType;
    AlertMapType m_alertDetails; // Map of alert codes to configured reported details.

    typedef std::map<std::string, AlertDetails<std::string>> StringAlertMapType;
    StringAlertMapType m_cachedAlertDetails; // Temporary cache of alert details used at startup
};

typedef std::shared_ptr<BaseAlertDetailer> BaseAlertDetailerSPtr;

} //  namespace imp
