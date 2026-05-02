/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertSeverity.h"
#include "RawAlertCode.h"

#include <ostream>
#include <string>


namespace imp
{

/*!
 * @brief POD class for holding detailer information parsed from configuration.
 */
template<typename TCode>
struct AlertDetails
{
    /*!
     * @brief Constructor.
     * @param[in] code        The alert code that this AlertDetails describes.
     * @param[in] severity    The severity of the alert.
     * @param[in] description A string describing the above code.
     * @param[in] userDescription The translation key for the description shown to the user.
     * @param[in] userTitle The translation key for the title shown to the user.
     */
    AlertDetails(
        const TCode& code,
        Alert::Severity::Enum severity,
        const std::string& description,
        const std::string& userDescription,
        const std::string& userTitle )
    {
        Code = code;
        Severity = severity;
        Description = description;
        UserDescription = userDescription;
        UserTitle = userTitle;
    }

    /*! @brief The Alert code for the alert. */
    TCode Code;

    /*! @brief The severity of the alert. */
    Alert::Severity::Enum Severity;

    /*! @brief A string describing the above code. */
    std::string Description;

    /*! @brief The translation key for the description shown to the user. */
    std::string UserDescription;

    /*! @brief The translation key for the title shown to the user. */
    std::string UserTitle;
};

/*!
 * @brief    Streaming operator to automatically turn an AlertDetails into a string when output to a stream.
 */
template<typename TCode>
std::ostream&
operator<<( std::ostream& stream, const AlertDetails<TCode>& alertDetails )
{
    stream << " code=" << alertDetails.Code << ", severity=" << ::Alert::Severity::ToString( alertDetails.Severity )
           << ", description = " << alertDetails.Description << ", user description=" << alertDetails.UserDescription
           << ", user title=" << alertDetails.UserTitle;

    return stream;
}

} //  namespace imp
