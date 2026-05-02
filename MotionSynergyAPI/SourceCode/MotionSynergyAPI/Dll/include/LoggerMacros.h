/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

/*!
 *  @brief     Provides simple MACROs for usage and a Logging abstraction,
 *             allowing for alternate implementations of logging.
 *
 *  @details   Clients should use the macros in the following ways:
 *
 *                 LOG_TRACE( message );
 *                 LOG_TRACE( component, message );
 *                 LOG_TRACE( component, channel, message );
 *
 *                 LOG_DEBUG( message );
 *                 LOG_DEBUG( component, message );
 *                 LOG_DEBUG( component, channel, message );
 *
 *                 LOG_INFO( message );
 *                 LOG_INFO( component, message );
 *                 LOG_INFO( component, channel, message );
 *
 *                 LOG_WARNING( message );
 *                 LOG_WARNING( component, message );
 *                 LOG_WARNING( component, channel, message );
 *
 *                 LOG_ERROR( message );
 *                 LOG_ERROR( component, message );
 *                 LOG_ERROR( component, channel, message );
 *
 *                 LOG_FATAL( message );
 *                 LOG_FATAL( component, message );
 *                 LOG_FATAL( component, channel, message );
 */
#pragma once

#include "LoggerMacrosImpl.h"

/*!
 *   @brief  Macro to log an entry at Trace log severity.
 *
 *   @details    Call either:
 *                   LOG_TRACE( message );
 *                   LOG_TRACE( component, message );
 *                   LOG_TRACE( component, channel, message );
 */
#define LOG_TRACE( ... ) \
    EXPAND( GET_LOGGER_MACRO( __VA_ARGS__, LOG_TRACE_CHANNEL, LOG_TRACE_COMPONENT, LOG_TRACE_DEFAULT )( __VA_ARGS__ ) )

// --------------------------------------------------------------------------

/*!
 *   @brief  Macro to log an entry at Debug log severity.
 *
 *   @details    Call either:
 *                   LOG_DEBUG( message );
 *                   LOG_DEBUG( component, message );
 *                   LOG_DEBUG( component, channel, message );
 */
#define LOG_DEBUG( ... ) \
    EXPAND( GET_LOGGER_MACRO( __VA_ARGS__, LOG_DEBUG_CHANNEL, LOG_DEBUG_COMPONENT, LOG_DEBUG_DEFAULT )( __VA_ARGS__ ) )

// --------------------------------------------------------------------------

/*!
 *   @brief  Macro to log an entry at Info log severity.
 *
 *   @details    Call either:
 *                   LOG_INFO( message );
 *                   LOG_INFO( component, message );
 *                   LOG_INFO( component, channel, message );
 */
#define LOG_INFO( ... ) \
    EXPAND( GET_LOGGER_MACRO( __VA_ARGS__, LOG_INFO_CHANNEL, LOG_INFO_COMPONENT, LOG_INFO_DEFAULT )( __VA_ARGS__ ) )

// --------------------------------------------------------------------------

/*!
 *   @brief  Macro to log an entry at Warn log severity.
 *
 *   @details    Call either:
 *                   LOG_WARNING( message );
 *                   LOG_WARNING( component, message );
 *                   LOG_WARNING( component, channel, message );
 */
#define LOG_WARNING( ... ) \
    EXPAND( GET_LOGGER_MACRO( __VA_ARGS__, LOG_WARN_CHANNEL, LOG_WARN_COMPONENT, LOG_WARN_DEFAULT )( __VA_ARGS__ ) )

// --------------------------------------------------------------------------

/*!
 *   @brief  Macro to log an entry at Error log severity.
 *
 *   @details    Call either:
 *                   LOG_ERROR( message );
 *                   LOG_ERROR( component, message );
 *                   LOG_ERROR( component, channel, message );
 */
#define LOG_ERROR( ... ) \
    EXPAND( GET_LOGGER_MACRO( __VA_ARGS__, LOG_ERROR_CHANNEL, LOG_ERROR_COMPONENT, LOG_ERROR_DEFAULT )( __VA_ARGS__ ) )

// --------------------------------------------------------------------------

/*!
 *   @brief  Macro to log an entry at Fatal log severity.
 *
 *   @details    Call either:
 *                   LOG_FATAL( message );
 *                   LOG_FATAL( component, message );
 *                   LOG_FATAL( component, channel, message );
 */
#define LOG_FATAL( ... ) \
    EXPAND( GET_LOGGER_MACRO( __VA_ARGS__, LOG_FATAL_CHANNEL, LOG_FATAL_COMPONENT, LOG_FATAL_DEFAULT )( __VA_ARGS__ ) )

// --------------------------------------------------------------------------

/*!
 *   @brief  Macro to log an entry with the given severity depending on the condition.
 *
 *   @param  condition       The conditional expression to evaluate
 *   @param  level_if_true   The log level when the expression is true (TRACE, DEBUG, INFO, WARNING, ERROR or FATAL)
 *   @param  level_if_false  The log level when the expression is false (TRACE, DEBUG, INFO, WARNING, ERROR or FATAL)
 *   @param  ...             The remaining parameters as if the LOG_TRACE/LOG_DEBUG/etc... macro is called
 */
#define LOG_CONDITION( condition, level_if_true, level_if_false, ... ) \
    if ( ( condition ) )                                               \
    {                                                                  \
        LOG_##level_if_true( __VA_ARGS__ );                            \
    }                                                                  \
    else                                                               \
    {                                                                  \
        LOG_##level_if_false( __VA_ARGS__ );                           \
    }
