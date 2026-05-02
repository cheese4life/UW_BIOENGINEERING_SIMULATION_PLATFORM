/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include "DiagnosticLogImpl.h"
#include "SmartStream.h"

/*! @brief  Macro to log function entry and exit with 0-7 arguments at Trace level.
 *   @note   This macro requires a const std::string ComponentName to be defined in the current scope.
 */
#define LOG_TRACE_FUNC( ... ) _LOG_TRACE_FUNC( _ARGS_AUGMENTER( __VA_ARGS__ ) )

/*! @brief  Macro to log function entry and exit with 0-7 arguments at Debug level.
 *   @note   This macro requires a const std::string ComponentName to be defined in the current scope.
 */
#define LOG_DEBUG_FUNC( ... ) _LOG_DEBUG_FUNC( _ARGS_AUGMENTER( __VA_ARGS__ ) )

/*! @brief  Macro to log and return a value from a function.
 *   @note   This macro requires LOG_TRACE_FUNC() or LOG_DEBUG_FUNC() to have been used within the current function.
 */
#define LOG_RETURN( VALUE )                                     \
    {                                                           \
        auto const _value = VALUE;                              \
        imp::SmartStream __ReturnValueStream;                   \
        __ReturnValueStream << _value;                          \
        __LOG_FUNC.SetReturnValue( __ReturnValueStream.str() ); \
        return _value;                                          \
    }

/*! @brief  Macro to log a specified string before returning from a function
 *           Useful when SmartStream cannot handle the returned value e.g. a ranged datatype
 *   @note   This macro requires LOG_TRACE_FUNC() or LOG_DEBUG_FUNC() to have been used within the current function.
 */
#define LOG_STRING_BEFORE_RETURN( STRING )                      \
    {                                                           \
        auto const _value = STRING;                             \
        imp::SmartStream __ReturnValueStream;                   \
        __ReturnValueStream << _value;                          \
        __LOG_FUNC.SetReturnValue( __ReturnValueStream.str() ); \
    }
