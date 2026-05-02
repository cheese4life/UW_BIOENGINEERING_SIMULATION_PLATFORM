/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ILogger.h"
#include "LoggerType.h"

#include <memory>
#include <string>

namespace imp
{

using ILoggerSPtr = std::shared_ptr<ILogger>;

/*!
 *   @brief  Factory for creating various types of logger.
 */
class LoggerFactory
{
public:
    /*!
     *   @brief      Instantiate a logger instance.
     *
     *   @details    The Logger must be cleaned up manually.
     *
     *   @param[in]  type                The type of logger to instantiate.
     *   @param[in]  configurationFile   The file containing the configuration
     *                                   for the logger instance.
     *   @param[in]  logOutputFolder     Folder to write logs to.
     *   @param[in]  openLogCallback     Callback to execute when a new log file is opened.
     *   @param[in]  closeLogCallback    Callback to execute when a log file is being closed.
     *   @return     A shared pointer to the new logger instance.
     */
    static ILoggerSPtr Create(
        LoggerType type,
        const std::string& configurationFile = std::string( "" ),
        const std::string& logOutputFolder = std::string( "" ),
        ILogger::OpenLogCallbackFn openLogCallback = nullptr,
        ILogger::CloseLogCallbackFn closeLogCallback = nullptr );
};

} //  namespace imp
