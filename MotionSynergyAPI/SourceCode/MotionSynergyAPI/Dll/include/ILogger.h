/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "LogLevel.h"

#include <functional>
#include <memory>
#include <string>


namespace imp
{

/*!
 *  @brief     Defines the interface a logger must adhere to to handle logging within
 *             the CCP framework.
 */
class ILogger
{
public:
    /*! @brief  The name of the default channel that is logged to. */
    static const std::string DefaultChannelName;

    /*! @brief  The name of the default component that is logged to. */
    static const std::string DefaultComponentName;

    /*! @brief  The function prototype for opening a new log file. */
    using OpenLogCallbackFn =
        std::function<void( const std::string& sinkName, std::basic_ostream<char>& outputStream )>;

    /*! @brief  The function prototype for closing a log file. */
    using CloseLogCallbackFn =
        std::function<void( const std::string& sinkName, std::basic_ostream<char>& outputStream )>;

    /*!
     *   @brief  Constructor (empty).
     */
    ILogger() = default;

    /*!
     *   @brief  Virtual destructor (empty).
     */
    virtual ~ILogger() = default;

public:
    /*!
     *   @brief      Safely and cleanly close the logger, writing all pending
     *               output to target sinks.
     */
    virtual void Destroy() = 0;

    /*!
     *   @brief      Get the current log level.
     *   @return     The current log level.
     */
    virtual LogLevel GetLogLevel() const = 0;

    /*!
     *   @brief      Set the current log level.
     *   @param[in]  level       The log level to set.
     *   @param[in]  persist     Persist the log level to the configuration file if true.
     *
     */
    virtual void SetLogLevel( LogLevel level, bool persist ) = 0;

    /*!
     *   @brief      Register a channel to log to.
     *   @param[in]  channelName     The name of the channel to create.
     *
     *   @details    This method is a no-op if the channel already exists.
     */
    virtual void AddChannel( const std::string& channelName ) = 0;


    /*!
     *   @brief      Rolls over all channels (all text based sinks).
     */
    virtual void Rollover() = 0;


    /*!
     *   @brief      Rolls over the given channel (for a text based sink)
     *   @param[in]  channelName         The channel to roll over (or empty for all sinks).
     *   @attention  The name of the channel must match the name of the sink (case-insensitive) in the logger
     *               configuration file.
     */
    virtual void Rollover( std::string const& channelName ) = 0;


    /*!
     *   @brief      Applies the given predicate for rollover to all (text-based) sinks.
     */
    virtual void SetTriggerBasedRotation( std::function<bool()> pred ) = 0;


    /*!
     *   @brief      Applies the given predicate for rollover to the (text-based) channel
     *   @param[in]  pred            Predicate function to determine whether to trigger rotation
     *   @param[in]  channelName     The channel to apply the rollover trigger to (or empty for all sinks).
     *   @attention  The name of the channel must match the name of the sink (case-insensitive) in the logger
     *               configuration file.
     */
    virtual void SetTriggerBasedRotation( std::function<bool()> pred, std::string const& channelName ) = 0;

    /*!
     *   @brief      Sets the callback when a new log file is opened - this only applies to asynchronous
     *               log sinks
     *   @param[in]  callback        The callback to execute before after a new log file is opened
     */
    virtual void SetOpenLogCallback( OpenLogCallbackFn callback ) = 0;

    /*!
     *   @brief      Sets the callback when a log file is about to be closed - this only applies to
     *               asynchronous log sinks
     *   @param[in]  callback        The callback to execute before a log file is closed
     */
    virtual void SetCloseLogCallback( CloseLogCallbackFn callback ) = 0;

    /*!
     *   @brief  Write a message to the trace log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  message         The message to write to the log.
     */
    virtual void Trace(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) = 0;

    /*!
     *   @brief  Write a message to the debug log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  message         The message to write to the log.
     */
    virtual void Debug(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) = 0;

    /*!
     *   @brief  Write a message to the info log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  message         The message to write to the log.
     */
    virtual void Info(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) = 0;

    /*!
     *   @brief  Write a message to the warning log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  message         The message to write to the log.
     */
    virtual void Warning(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) = 0;

    /*!
     *   @brief  Write a message to the error log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  message         The message to write to the log.
     */
    virtual void Error(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) = 0;

    /*!
     *   @brief  Write a message to the fatal log.
     *   @param[in]  component       The name of the component writing the message.
     *   @param[in]  channel         The channel to write the message to.
     *   @param[in]  sourceLocation  The location of the source code writing the message.
     *   @param[in]  message         The message to write to the log.
     */
    virtual void Fatal(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) = 0;
};

/*! @brief  Shorthand for creating shared pointers of ILogger. */
typedef std::shared_ptr<ILogger> ILoggerSPtr;

} //  namespace imp
