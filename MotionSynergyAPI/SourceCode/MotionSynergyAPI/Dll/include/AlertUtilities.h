/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertSeverity.h"
#include "AlertTypes.h"

#include <string>

namespace MotionSynergyAPI
{

/*!
 *  @brief  Class which provides some alert-related utilities.
 */
class AlertUtilities
{
public:
    /*!
     *  @brief          Raise an alert from the central controller.
     *  @param[in]      alertCode   Alert code.
     *  @param[in]      severity    Alert severity.
     *  @param[in]      description An optional string describing the circumstances for the alert.
     */
    static void RaiseAlert(
        ::Alert::AlertCode alertCode,
        ::Alert::Severity::Enum severity,
        const std::string& description = std::string() );
};

} // namespace MotionSynergyAPI
