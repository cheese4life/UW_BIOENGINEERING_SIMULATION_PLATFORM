/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "DiagnosticsCallback.h"

#include <Logger.h>

#include <boost/core/noncopyable.hpp>
#include <mutex>

namespace MotionSynergyAPI
{
class CallbackSubscriber;

/*!
 * @class   MotionSynergyAPILogger
 * @brief   This is a wrapper for the Logger class. It allows registration to receive log lines and will synchronously call callbacks with log lines
 * NB: Due to technical limitations in this design the timestamp and thread ID may not always accurately match the lines in the log file.
 */
class MotionSynergyAPILogger
    : public imp::ILogger
    , private boost::noncopyable
{
public:
    MotionSynergyAPILogger( imp::ILoggerSPtr pBaseLogger );

    /*!
     * @brief    Register a callback to receive log lines
     * @param[in]    callback    A callback to call with log lines
     * @return   Unique ID for this subscription
     */
    uint32_t AddLogLineCallback( LogSubscriberCallbackFn callback );

    /*!
     * @brief    Remove a callback for log lines
     * @param    subscriptionID  subscription ID to unsubscribe
     */
    void RemoveLogLineCallback( uint32_t subscriptionID );

    // overridden from imp::ILogger
    void Destroy() override;
    imp::LogLevel GetLogLevel() const override;
    void SetLogLevel( imp::LogLevel level, bool persist ) override;
    void AddChannel( const std::string& channelName ) override;
    void Rollover() override;
    void Rollover( std::string const& channelName ) override;
    void SetTriggerBasedRotation( std::function<bool()> pred ) override;
    void SetTriggerBasedRotation( std::function<bool()> pred, std::string const& channelName ) override;
    void SetOpenLogCallback( OpenLogCallbackFn callback ) override;
    void SetCloseLogCallback( CloseLogCallbackFn callback ) override;
    void Trace(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) override;
    void Debug(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) override;
    void Info(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) override;
    void Warning(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) override;
    void Error(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) override;
    void Fatal(
        const std::string& component,
        const std::string& channel,
        const std::string& sourceLocation,
        const std::string& message ) override;

private:
    void NotifySubscribers( imp::LogLevel level, const std::string& message, const std::string& channel );
    imp::ILoggerSPtr m_pBaseLogger;
    std::vector<CallbackSubscriber> m_logLineSubscribers;
    uint32_t m_nextSubscriberNumber;
    std::mutex m_mutex;
};
} // namespace MotionSynergyAPI
