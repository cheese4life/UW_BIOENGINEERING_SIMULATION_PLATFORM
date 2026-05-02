/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "SingletonLifetime.h"

#include <memory>

namespace imp
{

class ILogger;
using ILoggerSPtr = std::shared_ptr<ILogger>;


/*!
 * @class   LoggerSingleton
 * @brief   Holds the logger singleton instance
 */
class LoggerSingleton
{
public:
    LoggerSingleton() = delete;

    /*!
     *   @brief      Registers the provided logger as the singleton logger instance
     *               for the program.
     *
     *   @details    This enables the use of LOG_xxx macros.
     *
     *   @param[in]  pLogger    The logger to register.
     *
     *   @returns    The previously registered singleton logger instance (if present).
     */
    static ILoggerSPtr Register( ILoggerSPtr pLogger );

    /*!
     * @brief    Has the singleton instance been registered.
     * @returns  true if an instance has been registered, false if not.
     */
    static bool IsRegistered();

    /*!
     * @brief        Get the logger singleton instance.
     * @returns      The singleton instance on success.
     * @exception   Throws a std::logic_error if there is no registered singleton instance.
     */
    static ILoggerSPtr Get();

    /*!
     * @brief  Prepares for shutdown, releases the logger singleton instance.
     */
    static void Destroy();

private:
    static SingletonLifetime<ILogger> s_logger;
};

} //  namespace imp
