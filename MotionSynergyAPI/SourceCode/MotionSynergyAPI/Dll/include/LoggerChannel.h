/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <string>


namespace imp
{

/*!
 *  @brief     Defines the standard set of logger channel names. Each channel is
 *             usually configured to output to a separate file sink.
 *             Applications are free to define additional,
 *             application-specific logging channels.
 */
class LoggerChannel
{
public:
    // ------------------------------------- Core logging channels -------------------------------------
    // - these are automatically added by the Logger and do not require AddChannel() to be called.

    /*! @brief  The primary diagnostics log for use by developers
     *          which should include trace logging, exceptions, alerts, events, etc. */
    static const std::string Diagnostic;

    /*! @brief  Details of all events received and raised. */
    static const std::string Events;

    /*! @brief  Details of all alerts received and raised. */
    static const std::string Alerts;

    /*! @brief  Record of 'chemistry-significant' actions, events, etc. */
    static const std::string RunLog;

    // ------------------------------------ Optional logging channels ----------------------------------
    // - the application must call AddChannel() for each required channel.

    /*! @brief      Details all traffic over a specific 'link', where 'link'
     *              would be a connection to an SCP, a connection to an external system (e.g. LIS), etc.
     *  @details    Consider mapping each comms protocol layer to a different severity level.
     *  @note       The actual channel name should be constructed as CommsPrefix + 'link'.
     */
    static const std::string CommsPrefix;

    /*! @brief      Subsystem specific logs, which can be used to log trace information, device,
     *              driver commands, etc.
     *  @details    Every application should have at least one subsystem log. The subsystem name
     *              'Instrument' should be used for application's with a single subsystem.
     *  @note       The actual channel name should be constructed as SubsystemPrefix + 'subsystem name'.
     */
    static const std::string SubsystemPrefix;

    /*! @brief  Record of auditable events - e.g. user log in/out, setting changes, etc. */
    static const std::string AuditTrail;

    /*! @brief  Log to record scheduler behaviour for application's that require a scheduler. */
    static const std::string Scheduler;
};

} //  namespace imp
