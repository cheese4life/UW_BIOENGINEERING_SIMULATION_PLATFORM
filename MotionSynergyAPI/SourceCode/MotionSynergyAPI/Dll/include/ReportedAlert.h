/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertDeclarations.h"
#include "RawAlertCode.h"
#include <string>


namespace imp
{

/*!
 * @brief   A class representing a reported alert instance.
 */
class ReportedAlert
{
public:
    /*!
     * @brief Default constructor which sets all fields to default (empty) values.
     */
    ReportedAlert()
        : m_reportID()
        , m_timeStamp()
        , m_source()
        , m_code()
        , m_severity()
        , m_description()
        , m_userDescription()
        , m_userTitle()
        , m_contextId()
        , m_expectExternallyCleared()
    {
    }

    /*!
     * @brief Constructor.
     * @param  reportId - the unique identifier for the Alert.
     * @param  timestamp - epoch timestamp in seconds.
     * @param  source - the unique identifier of the component that raised the alert
     * @param  code - the alert code to be raised
     * @param  severity - the alert severity to be raised
     * @param  description - alert description string
     * @param  userDescription - The translation key for the description shown to the user.
     * @param  userTitle - The translation key for the title shown to the user.
     * @param  contextId - the id that represent the context that the alert occurred in. Eg. the sample id
     * @param  expectExternallyCleared - will be cleared by someone other than reporter.
     */
    ReportedAlert(
        ::Alert::ReportId reportId,
        AlertTimestamp timestamp,
        const std::string& source,
        RawAlertCode code,
        ::Alert::Severity::Enum severity,
        const std::string& description,
        const std::string& userDescription,
        const std::string& userTitle,
        uint16_t contextId,
        bool expectExternallyCleared )
        : m_reportID( reportId )
        , m_timeStamp( timestamp )
        , m_source( source )
        , m_code( code )
        , m_severity( severity )
        , m_description( description )
        , m_userDescription( userDescription )
        , m_userTitle( userTitle )
        , m_contextId( contextId )
        , m_expectExternallyCleared( expectExternallyCleared )
    {
    }

    virtual ~ReportedAlert() = default;

    /*!
     * @brief Gets the report ID field.
     * @returns The report ID for this reported alert.
     */
    ::Alert::ReportId
    GetReportId() const
    {
        return m_reportID;
    }

    /*!
     * @brief Gets the timestamp field.
     * @returns The timestamp for this reported alert.
     */
    AlertTimestamp
    GetTimeStamp() const
    {
        return m_timeStamp;
    }

    /*!
     * @brief Gets the source field.
     * @returns The source for this reported alert.
     */
    std::string
    GetSource() const
    {
        return m_source;
    }

    /*!
     * @brief Gets the alert code field.
     * @returns The alert code for this reported alert.
     */
    RawAlertCode
    GetCode() const
    {
        return m_code;
    }

    /*!
     * @brief Gets the severity field.
     * @returns The severity for this reported alert.
     */
    ::Alert::Severity::Enum
    GetSeverity() const
    {
        return m_severity;
    }

    /*!
     * @brief Gets the description field with an optional source placeholder replaced with the actual alert source.
     * @returns The description string for this reported alert.
     */
    std::string
    GetDescription() const
    {
        // Check for a source placeholder and replace it.
        const auto sourceOffsetStart = m_description.find( "[source" );
        if ( sourceOffsetStart != std::string::npos )
        {
            const auto sourceOffsetEnd = m_description.find( ']', sourceOffsetStart );
            if ( sourceOffsetEnd != std::string::npos )
            {
                auto source = m_source;
                if ( source.empty() )
                {
                    // If the source field is empty, try and find a default source in the placeholder and use that
                    const auto defaultSourceStart = m_description.find( '|', sourceOffsetStart );
                    if ( defaultSourceStart != std::string::npos )
                    {
                        source =
                            m_description.substr( defaultSourceStart + 1, sourceOffsetEnd - defaultSourceStart - 1 );
                    }
                    else
                    {
                        source = "[Unknown Source]";
                    }
                }

                // Perform the source substitution.
                return m_description.substr( 0, sourceOffsetStart ) + source +
                       m_description.substr( sourceOffsetEnd + 1 );
            }
        }
        return m_description;
    }

    /*!
     * @brief Gets the translation key for the description shown to the user.
     * @returns The translation key for the description shown to the user.
     */
    std::string
    GetUserDescription() const
    {
        return m_userDescription;
    }

    /*!
     * @brief Gets the translation key for the title shown to the user.
     * @returns The translation key for the title shown to the user.
     */
    std::string
    GetUserTitle() const
    {
        return m_userTitle;
    }

    /*!
     * @brief Gets the context ID field.
     * @returns The context ID for this reported alert.
     */
    uint16_t
    GetContextId() const
    {
        return m_contextId;
    }

    /*!
     * @brief Gets the expected externally cleared field.
     * @returns Whether this reported alert is expected to be externally cleared (true) or not (false).
     */
    bool
    GetExpectExternallyCleared() const
    {
        return m_expectExternallyCleared;
    }

    /*!
     * @brief Gets a string representation of this reported alert.
     * @returns A string representation of this reported alert.
     */
    std::string ToString() const;

private:
    const ::Alert::ReportId m_reportID;
    const AlertTimestamp m_timeStamp;
    const std::string m_source;
    const RawAlertCode m_code;
    const ::Alert::Severity::Enum m_severity;
    const std::string m_description;
    const std::string m_userDescription;
    const std::string m_userTitle;
    const uint16_t m_contextId;
    const bool m_expectExternallyCleared;
};

/*!
 * @brief    Streaming operator to automatically turn any ReportedAlert into a string when output to a stream.
 */
std::ostream& operator<<( std::ostream& stream, const ReportedAlert& reportedAlert );


} //  namespace imp
