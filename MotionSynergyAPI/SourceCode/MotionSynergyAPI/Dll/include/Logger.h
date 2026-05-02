/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ILogger.h"
#include "LoggerType.h"


namespace imp
{

/*!
 *   @brief  Provides a simple static interface to the logging infrastructure.
 */
class Logger
{
public:
    /*!
     *   @brief      Instantiate a logger instance.
     *
     *   @details    The Logger instance is maintained as a singleton.
     *
     *   @param[in]  type                The type of logger to instantiate.
     *   @param[in]  configurationFile   The file containing the configuration
     *                                   for the logger instance.
     *   @param[in]  logOutputFolder     Folder to write logs to.
     *   @param[in]  openLogCallback     Callback to execute when a new log file is opened.
     *   @param[in]  closeLogCallback    Callback to execute when a log file is being closed.
     */
    static void Create(
        LoggerType type,
        const std::string& configurationFile = std::string( "" ),
        const std::string& logOutputFolder = std::string( "" ),
        ILogger::OpenLogCallbackFn openLogCallback = nullptr,
        ILogger::CloseLogCallbackFn closeLogCallback = nullptr );


    /*!
     *   @brief      Safely and cleanly close the logger, writing all pending
     *               output to target sinks.
     */
    static void Destroy();


    /*!
     *   @brief      Will perform rollover on all channels.
     */
    static void Rollover();


    /*!
     *   @brief      Will perform rollover on the given channel.
     */
    static void Rollover( const std::string& channel );


    /*!
     *   @brief      Will perform trigger based rollover on all channels.
     */
    static void SetTriggerBasedRotation( std::function<bool()> pred );


    /*!
     *   @brief      Will perform trigger based rollover on the given channel.
     */
    static void SetTriggerBasedRotation( std::function<bool()> pred, const std::string& channel );

    /*!
     *   @brief      Sets the callback when a new log file is opened - this only applies to asynchronous
     *               log sinks
     *   @param[in]  callback        The callback to execute after a new log file is opened
     */
    static void SetOpenLogCallback( ILogger::OpenLogCallbackFn callback );

    /*!
     *   @brief      Sets the callback when a log file is about to be closed - this only applies to
     *               asynchronous log sinks
     *   @param[in]  callback        The callback to execute before a log file is closed
     */
    static void SetCloseLogCallback( ILogger::CloseLogCallbackFn callback );

    /*!
     *   @brief      Register a channel with the logger, allowing filtering.
     */
    static void AddChannel( const std::string& channel );

    /*!
     *   @brief      Get the current log level.
     *   @return     The current log level.
     */
    static LogLevel GetLogLevel();

    /*!
     *   @brief      Set the current log level.
     *   @param[in]  level       The log level to set.
     *   @param[in]  persist     Persist the log level to the configuration file if true.
     *
     */
    static void SetLogLevel( LogLevel level, bool persist );

    /*!
     *   @brief  Write a message to the trace log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  rhs             The message to write to the log.
     */
    static void Trace(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& rhs );


    /*!
     *   @brief  Write a message to the debug log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  rhs             The message to write to the log.
     */
    static void Debug(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& rhs );

    /*!
     *   @brief  Write a message to the info log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  rhs             The message to write to the log.
     */
    static void Info(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& rhs );

    /*!
     *   @brief  Write a message to the warning log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  rhs             The message to write to the log.
     */
    static void Warning(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& rhs );

    /*!
     *   @brief  Write a message to the error log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  rhs             The message to write to the log.
     */
    static void Error(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& rhs );

    /*!
     *   @brief  Write a message to the fatal log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  rhs             The message to write to the log.
     */
    static void Fatal(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& rhs );
};

} //  namespace imp
