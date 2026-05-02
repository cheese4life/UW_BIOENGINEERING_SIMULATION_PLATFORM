/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <ILogger.h>

#include "DiagnosticsCallback.h"

namespace MotionSynergyAPI
{

class DiagnosticsNative
{
public:
    DiagnosticsNative() = default;
    ~DiagnosticsNative() = default;

    /*!
     *  @brief   Creates the MotionSynergyAPI logger as the program logger instance and
     *           registers a logger callback
     *  @param[in]   logConfigFile  Log configuration filename
     *  @param[in]   logEntrySubscriber  Callback function to invoke when a set of
     *                                   log lines are published
     *  @param[in]   openLogCallback     Callback to execute when a new log file is opened.
     *  @param[in]   closeLogCallback    Callback to execute when a log file is being closed.
     */
    void InitializeLogger(
        const std::string& logConfigFile,
        LogEntriesSubscriberCallbackFn logEntrySubscriber,
        imp::ILogger::OpenLogCallbackFn openLogCallback,
        imp::ILogger::CloseLogCallbackFn closeLogCallback );

    /*!
     *  @brief   Removes logger callback subscription
     */
    void RemoveLoggerCallback();

    /*!
     *  @brief  Returns the location of the log folder.
     */
    std::string GetLogFolder();

    /*!
     *  @brief  Returns the current log level.
     */
    imp::LogLevel GetLogLevel();

    /*!
     *  @brief  Sets the current log level.
     *  @param[in]      newLogLevel  The new log level (Debug, Info, Warning, Error, Fatal).
     *  @param[in]      persist      Persist the log level to the configuration file if true.
     */
    void SetLogLevel( imp::LogLevel newLogLevel, bool persist );

    /*!
     *  @brief  Logs the provided string to the diagnostic log at a specified log level.
     *  @param[in]      level  The log level (Debug, Info, Warning, Error, Fatal).
     *  @param[in]      logline  String to log.
     */
    void LogString( imp::LogLevel level, const std::string& logline );

    /*!
     *  @brief   Performs rollover of all log channels
     */
    void Rollover();

private:
    // Logs are located in the program data folder.
    const std::string m_logFolder = "[ProgramDataFolder]/logs";
    uint32_t m_logSubscriberId = 0u;
};

} // namespace MotionSynergyAPI
