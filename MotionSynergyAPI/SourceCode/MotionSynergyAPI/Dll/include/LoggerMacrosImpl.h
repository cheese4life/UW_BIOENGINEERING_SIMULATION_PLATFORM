/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

/*!
 *  @brief     The following macros should not be used directly in code.
 *             Instead use the public macros defined in LoggerMacros.h.
 */
#pragma once

#include "Logger.h"
#include "SmartStream.h"
#include <sstream>


/*! @brief  Extract the filename from the complete pathname at compile time. */
using cstr = const char* const;

static constexpr const char*
past_last_slash( cstr str, cstr last_slash )
{
    return *str == '\0'                      ? last_slash
           : ( *str == '/' || *str == '\\' ) ? past_last_slash( str + 1, str + 1 )
                                             : past_last_slash( str + 1, last_slash );
}

static constexpr const char*
past_last_slash( cstr str )
{
    return past_last_slash( str, str );
}

#define __FILENAMEONLY__ past_last_slash( __FILE__ )


/*! @brief  This is a workaround for MSVC as it does not expand __VA_ARGS__ as per the standard. */
#define EXPAND( x ) x

/*! @brief  This clever thing allows us to overload the number of arguments in a macro. */
#define GET_LOGGER_MACRO( _1, _2, _3, NAME, ... ) NAME

// --------------------------------------------------------------------------

/*!
 *  @brief   Log the supplied message to the Trace level filter,
 *           originating from a specific component,
 *           on the target channel.
 *  @details Trace logging should be used for the most verbose output
 *           useful for evaluating the code path taken during execution.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   channel     The channel to log to.
 *  @param[in]   message     The message to log.
 */
#define LOG_TRACE_CHANNEL( component, channel, message )                                \
    {                                                                                   \
        imp::SmartStream __loggerOss;                                                   \
        std::ostringstream __loggerSrc;                                                 \
        __loggerOss << message;                                                         \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                             \
        imp::Logger::Trace( component, channel, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Trace level filter,
 *           originating from a specific component.
 *  @details Trace logging should be used for the most verbose output
 *           useful for evaluating the code path taken during execution.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   message     The message to log.
 */
#define LOG_TRACE_COMPONENT( component, message )                                                                \
    {                                                                                                            \
        imp::SmartStream __loggerOss;                                                                            \
        std::ostringstream __loggerSrc;                                                                          \
        __loggerOss << message;                                                                                  \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                      \
        imp::Logger::Trace( component, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Trace level filter
 *  @details Trace logging should be used for the most verbose output
 *           useful for evaluating the code path taken during execution.
 *
 *  @param[in]   message     The message to log.
 */
#define LOG_TRACE_DEFAULT( message )                                                                                 \
    {                                                                                                                \
        imp::SmartStream __loggerOss;                                                                                \
        std::ostringstream __loggerSrc;                                                                              \
        __loggerOss << message;                                                                                      \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                          \
        imp::Logger::Trace( ComponentName, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

// --------------------------------------------------------------------------

/*!
 *  @brief   Log the supplied message to the Debug level filter,
 *           originating from a specific component,
 *           on the target channel.
 *  @details Debug logging should be used for verbose output of values,
 *           arguments, results, etc.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   channel     The channel to log to.
 *  @param[in]   message     The message to log.
 */
#define LOG_DEBUG_CHANNEL( component, channel, message )                                \
    {                                                                                   \
        imp::SmartStream __loggerOss;                                                   \
        std::ostringstream __loggerSrc;                                                 \
        __loggerOss << message;                                                         \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                             \
        imp::Logger::Debug( component, channel, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Debug level filter,
 *           originating from a specific component.
 *  @details Debug logging should be used for verbose output of values,
 *           arguments, results, etc.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   message     The message to log.
 */
#define LOG_DEBUG_COMPONENT( component, message )                                                                \
    {                                                                                                            \
        imp::SmartStream __loggerOss;                                                                            \
        std::ostringstream __loggerSrc;                                                                          \
        __loggerOss << message;                                                                                  \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                      \
        imp::Logger::Debug( component, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Debug level filter
 *  @details Debug logging should be used for verbose output of values,
 *           arguments, results, etc.
 *
 *  @param[in]   message     The message to log.
 */
#define LOG_DEBUG_DEFAULT( message )                                                                                 \
    {                                                                                                                \
        imp::SmartStream __loggerOss;                                                                                \
        std::ostringstream __loggerSrc;                                                                              \
        __loggerOss << message;                                                                                      \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                          \
        imp::Logger::Debug( ComponentName, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

// --------------------------------------------------------------------------

/*!
 *  @brief   Log the supplied message to the Info level filter,
 *           originating from a specific component,
 *           on the target channel.
 *  @details Info logging should be used for output of high level
 *           component and application normal operating information.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   channel     The channel to log to.
 *  @param[in]   message     The message to log.
 */
#define LOG_INFO_CHANNEL( component, channel, message )                                \
    {                                                                                  \
        imp::SmartStream __loggerOss;                                                  \
        std::ostringstream __loggerSrc;                                                \
        __loggerOss << message;                                                        \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                            \
        imp::Logger::Info( component, channel, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Info level filter,
 *           originating from a specific component.
 *  @details Info logging should be used for output of high level
 *           component and application normal operating information.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   message     The message to log.
 */
#define LOG_INFO_COMPONENT( component, message )                                                                \
    {                                                                                                           \
        imp::SmartStream __loggerOss;                                                                           \
        std::ostringstream __loggerSrc;                                                                         \
        __loggerOss << message;                                                                                 \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                     \
        imp::Logger::Info( component, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Info level filter
 *  @details Info logging should be used for output of high level
 *           component and application normal operating information.
 *
 *  @param[in]   message     The message to log.
 */
#define LOG_INFO_DEFAULT( message )                                                                                 \
    {                                                                                                               \
        imp::SmartStream __loggerOss;                                                                               \
        std::ostringstream __loggerSrc;                                                                             \
        __loggerOss << message;                                                                                     \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                         \
        imp::Logger::Info( ComponentName, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

// --------------------------------------------------------------------------

/*!
 *  @brief   Log the supplied message to the Warning level filter,
 *           originating from a specific component,
 *           on the target channel.
 *  @details Warning logging should be used for problematic events
 *           that do not immediately cause an error, e.g.;
 *           handled/expected exceptions, handled invalid values.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   channel     The channel to log to.
 *  @param[in]   message     The message to log.
 */
#define LOG_WARN_CHANNEL( component, channel, message )                                   \
    {                                                                                     \
        imp::SmartStream __loggerOss;                                                     \
        std::ostringstream __loggerSrc;                                                   \
        __loggerOss << message;                                                           \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                               \
        imp::Logger::Warning( component, channel, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Warning level filter,
 *           originating from a specific component.
 *  @details Warning logging should be used for problematic events
 *           that do not immediately cause an error, e.g.;
 *           handled/expected exceptions, handled invalid values.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   message     The message to log.
 */
#define LOG_WARN_COMPONENT( component, message )                                                                   \
    {                                                                                                              \
        imp::SmartStream __loggerOss;                                                                              \
        std::ostringstream __loggerSrc;                                                                            \
        __loggerOss << message;                                                                                    \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                        \
        imp::Logger::Warning( component, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Warning level filter
 *  @details Warning logging should be used for problematic events
 *           that do not immediately cause an error, e.g.;
 *           handled/expected exceptions, handled invalid values.
 *
 *  @param[in]   message     The message to log.
 */
#define LOG_WARN_DEFAULT( message )                                                                                    \
    {                                                                                                                  \
        imp::SmartStream __loggerOss;                                                                                  \
        std::ostringstream __loggerSrc;                                                                                \
        __loggerOss << message;                                                                                        \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                            \
        imp::Logger::Warning( ComponentName, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

// --------------------------------------------------------------------------

/*!
 *  @brief   Log the supplied message to the Error level filter,
 *           originating from a specific component,
 *           on the target channel.
 *  @details Error logging should be used for events that result in an error,
 *           preventing the execution of a function or requiring recovery.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   channel     The channel to log to.
 *  @param[in]   message     The message to log.
 */
#define LOG_ERROR_CHANNEL( component, channel, message )                                \
    {                                                                                   \
        imp::SmartStream __loggerOss;                                                   \
        std::ostringstream __loggerSrc;                                                 \
        __loggerOss << message;                                                         \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                             \
        imp::Logger::Error( component, channel, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Error level filter,
 *           originating from a specific component.
 *  @details Error logging should be used for events that result in an error,
 *           preventing the execution of a function or requiring recovery.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   message     The message to log.
 */
#define LOG_ERROR_COMPONENT( component, message )                                                                \
    {                                                                                                            \
        imp::SmartStream __loggerOss;                                                                            \
        std::ostringstream __loggerSrc;                                                                          \
        __loggerOss << message;                                                                                  \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                      \
        imp::Logger::Error( component, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Error level filter
 *  @details Error logging should be used for events that result in an error,
 *           preventing the execution of a function or requiring recovery.
 *
 *  @param[in]   message     The message to log.
 */
#define LOG_ERROR_DEFAULT( message )                                                                                 \
    {                                                                                                                \
        imp::SmartStream __loggerOss;                                                                                \
        std::ostringstream __loggerSrc;                                                                              \
        __loggerOss << message;                                                                                      \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                          \
        imp::Logger::Error( ComponentName, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

// --------------------------------------------------------------------------

/*!
 *  @brief   Log the supplied message to the Fatal level filter,
 *           originating from a specific component,
 *           on the target channel.
 *  @details Fatal logging should be used for events that result in application closure,
 *           e.g., unhandled exception, failed assertion.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   channel     The channel to log to.
 *  @param[in]   message     The message to log.
 */
#define LOG_FATAL_CHANNEL( component, channel, message )                                \
    {                                                                                   \
        imp::SmartStream __loggerOss;                                                   \
        std::ostringstream __loggerSrc;                                                 \
        __loggerOss << message;                                                         \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                             \
        imp::Logger::Fatal( component, channel, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Fatal level filter,
 *           originating from a specific component.
 *  @details Fatal logging should be used for events that result in application closure,
 *           e.g., unhandled exception, failed assertion.
 *
 *  @param[in]   component   The originating component.
 *  @param[in]   message     The message to log.
 */
#define LOG_FATAL_COMPONENT( component, message )                                                                \
    {                                                                                                            \
        imp::SmartStream __loggerOss;                                                                            \
        std::ostringstream __loggerSrc;                                                                          \
        __loggerOss << message;                                                                                  \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                      \
        imp::Logger::Fatal( component, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }

/*!
 *  @brief   Log the supplied message to the Fatal level filter
 *  @details Fatal logging should be used for events that result in application closure,
 *           e.g., unhandled exception, failed assertion.
 *
 *  @param[in]   message     The message to log.
 */
#define LOG_FATAL_DEFAULT( message )                                                                                 \
    {                                                                                                                \
        imp::SmartStream __loggerOss;                                                                                \
        std::ostringstream __loggerSrc;                                                                              \
        __loggerOss << message;                                                                                      \
        __loggerSrc << __FILENAMEONLY__ << "," << __LINE__;                                                          \
        imp::Logger::Fatal( ComponentName, imp::ILogger::DefaultChannelName, __loggerSrc.str(), __loggerOss.str() ); \
    }
