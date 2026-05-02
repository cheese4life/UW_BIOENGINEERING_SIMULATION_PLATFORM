/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertSeverity.h"
#include "AlertTypes.h"
#include "IAlertFilter.h"

#include <map>
#include <string>
#include <vector>

namespace imp
{

/*!---------------------------------------------------------------------------
 * @class   AlertFilterChangeSeverity
 * @brief   An alert filter which changes the severity of alerts which match a specified
 *          code or code and source.
 * @details To use this filter, specify one or more alert codes (and optionally sources), their new
 *          severity and then insert it into the list of filters maintained by the alert distribution singleton.
 *          This class could be used as a base class for more sophisticated filters.
 *          For example, one which loads the list of alerts and new severities from a configuration file.
 */
class AlertFilterChangeSeverity : public IAlertFilter
{
public:
    AlertFilterChangeSeverity() = default;
    virtual ~AlertFilterChangeSeverity() = default;

    /*!
     * @brief     See IAlertFilter.
     */
    ReportedAlertConstSPtr FilterAlert( ReportedAlertConstSPtr pAlert ) override;

    /*!
     * @brief     Change the severity of all alerts matching the specified alertCode.
     * @param[in] alertCode - The alert code to filter alerts by.
     * @param[in] newSeverity - The new severity of the updated alert.
     */
    void ChangeSeverityByCode( Alert::AlertCode alertCode, Alert::Severity::Enum newSeverity );

    /*!
     * @brief     Change the severity of all alerts matching the specified alertCode and originating from the specified source.
     * @param[in] alertCode   - The alert code to filter alerts by.
     * @param[in] alertSource - The alert source to filter alerts by.
     * @param[in] newSeverity - The new severity of the updated alert.
     */
    void ChangeSeverityByCodeAndSource(
        Alert::AlertCode alertCode,
        const std::string& alertSource,
        Alert::Severity::Enum newSeverity );

private:
    std::map<Alert::AlertCode, std::vector<std::pair<std::string, Alert::Severity::Enum>>> m_alertCodeFilters;
};


} //  namespace imp
