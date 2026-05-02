/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "LogLevel.h"

#include <functional>
#include <string>


namespace MotionSynergyAPI
{
/*!
 *  @brief  Callback function to be invoked when log entries are published
 */
using LogSubscriberCallbackFn = std::function<
    void( const std::string& timeStamp, const std::string& threadID, imp::LogLevel level, const std::string& message )>;

/*!
 *  @brief  Callback function to be invoked when a set of log lines is published.
 */
using LogEntriesSubscriberCallbackFn = std::function<
    void( const std::string& timeStamp, const std::string& threadID, imp::LogLevel level, const std::string& message )>;

} // namespace MotionSynergyAPI
