/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

/*!
 *  @brief     The following class and macros should not be used directly in code.
 *             Instead use the public macros defined in DiagnosticLog.h.
 */
#pragma once
#include "LoggerMacros.h"
#include "SmartStream.h"
#include "StackTrace.h"

namespace imp
{

/*! @class DiagnosticLog
 *   @brief Class to log function entry and exit points, return values and maintain
 *          thread-specific stack traces.
 *   @details Instances must be created on the stack so the destructor is automatically called
 *           on function exit.
 *   @note   Should not be used directly. Instead use an appropriate macro defined below.
 */
class DiagnosticLog
{
public:
    /*! @fn DiagnosticLog
     *   @brief Creates a diagnostic log object for logging diagnostic info,
     *          and indenting the log output.
     *  @param[in] componentName: name of the component doing the logging.
     *  @param[in] channelName: name of the channel to log to.
     *  @param[in] functionName: function name to log.
     *  @param[in] params: parameters string to be logged, leave empty if none.
     *  @param[in] sourceLocation: the filename and line number of the function entry point.
     *  @param[in] useTraceLevel: Log at Trace level if true, Debug level if false.
     */
    DiagnosticLog(
        const std::string& componentName,
        const std::string& channelName,
        const std::string& functionName,
        const std::string& params,
        const std::string& sourceLocation,
        bool useTraceLevel = false );

    /*! @fn ~DiagnosticLog
     *   @brief Logs an entry indicating function exit and an optional return value.
     */
    virtual ~DiagnosticLog();

    /*! @fn SetReturnValue
     *   @brief Stores a return value for logging on function exit.
     *  @param[in] returnValue: The return value to log on function exit.
     */
    void SetReturnValue( const std::string& returnValue );

private:
    void WriteToLog( const std::string& message ) const;

    StackTrace m_stackTrace;
    const std::string m_componentName;
    const std::string m_channelName;
    const std::string m_sourceLocation;
    const bool m_useTraceLevel;
    std::string m_returnValue;
};

} //  namespace imp


/*! @brief  Little hack to make the __LINE__ macro be a string at compile time
 *   two macros ensures any macro passed will be expanded before being stringified
 */
#define STRINGIZE_SUBCALL( x ) #x
#define STRINGIZE( x )         STRINGIZE_SUBCALL( x )

/*! @brief  This clever thing allows us to overload the number of arguments in a macro. */
#define GET_FUNC_LOGGER_MACRO( _1, _2, _3, _4, _5, _6, _7, _8, NAME, ... ) NAME


/*! @brief  This macro prepends an 'unused' string argument to a variadic argument list
 *  to handle the case of an empty variadic argument list.
 *  TODO REUSE-678 - note that __PRETTY_FUNCTION__ doesn't include the namespace, __FUNCTION__ does
 */
#if defined( _MSC_VER ) // Visual Studio Compilers
    #define _ARGS_AUGMENTER( ... )    "unused", __VA_ARGS__
    #define _CLASS_AND_FUNCTION_NAME_ __FUNCTION__
#else // Other Compilers
    #define _ARGS_AUGMENTER( ... )    "unused", ##__VA_ARGS__
    #define _CLASS_AND_FUNCTION_NAME_ __PRETTY_FUNCTION__
#endif

/*! @brief  This macro creates a local instance of the Diagnostic log on the stack.
 *   @details It should not be used directly but is instead used by other macros.
 */
#define _LOG_LEVEL_FUNC( LEVEL, ARGS )    \
    imp::DiagnosticLog __LOG_FUNC(        \
        ComponentName,                    \
        imp::ILogger::DefaultChannelName, \
        _CLASS_AND_FUNCTION_NAME_,        \
        ARGS,                             \
        std::string( __FILENAMEONLY__ ) + std::string( "," ) + std::string( STRINGIZE( __LINE__ ) ), LEVEL );

/*! @brief  This macro logs 1 function argument.
 *   @details It should not be used directly but is instead used by other macros.
 */
#define _LOG_LEVEL_FUNC1( LEVEL, ARG_1 )  \
    imp::SmartStream __dlArgStream;       \
    __dlArgStream << #ARG_1 "=" << ARG_1; \
    _LOG_LEVEL_FUNC( LEVEL, __dlArgStream.str() )

/*! @brief  This macro logs 2 function arguments.
 *   @details It should not be used directly but is instead used by other macros.
 */
#define _LOG_LEVEL_FUNC2( LEVEL, ARG_1, ARG_2 )  \
    imp::SmartStream __dlArgStream;              \
    __dlArgStream << #ARG_1 "=" << ARG_1;        \
    __dlArgStream << "," << #ARG_2 "=" << ARG_2; \
    _LOG_LEVEL_FUNC( LEVEL, __dlArgStream.str() )

/*! @brief  This macro logs 3 function arguments.
 *   @details It should not be used directly but is instead used by other macros.
 */
#define _LOG_LEVEL_FUNC3( LEVEL, ARG_1, ARG_2, ARG_3 ) \
    imp::SmartStream __dlArgStream;                    \
    __dlArgStream << #ARG_1 "=" << ARG_1;              \
    __dlArgStream << "," << #ARG_2 "=" << ARG_2;       \
    __dlArgStream << "," << #ARG_3 "=" << ARG_3;       \
    _LOG_LEVEL_FUNC( LEVEL, __dlArgStream.str() )

/*! @brief  This macro logs 4 function arguments.
 *   @details It should not be used directly but is instead used by other macros.
 */
#define _LOG_LEVEL_FUNC4( LEVEL, ARG_1, ARG_2, ARG_3, ARG_4 ) \
    imp::SmartStream __dlArgStream;                           \
    __dlArgStream << #ARG_1 "=" << ARG_1;                     \
    __dlArgStream << "," << #ARG_2 "=" << ARG_2;              \
    __dlArgStream << "," << #ARG_3 "=" << ARG_3;              \
    __dlArgStream << "," << #ARG_4 "=" << ARG_4;              \
    _LOG_LEVEL_FUNC( LEVEL, __dlArgStream.str() )

/*! @brief  This macro logs 5 function arguments.
 *   @details It should not be used directly but is instead used by other macros.
 */
#define _LOG_LEVEL_FUNC5( LEVEL, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5 ) \
    imp::SmartStream __dlArgStream;                                  \
    __dlArgStream << #ARG_1 "=" << ARG_1;                            \
    __dlArgStream << "," << #ARG_2 "=" << ARG_2;                     \
    __dlArgStream << "," << #ARG_3 "=" << ARG_3;                     \
    __dlArgStream << "," << #ARG_4 "=" << ARG_4;                     \
    __dlArgStream << "," << #ARG_5 "=" << ARG_5;                     \
    _LOG_LEVEL_FUNC( LEVEL, __dlArgStream.str() )

/*! @brief  This macro logs 6 function arguments.
 *   @details It should not be used directly but is instead used by other macros.
 */
#define _LOG_LEVEL_FUNC6( LEVEL, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ARG_6 ) \
    imp::SmartStream __dlArgStream;                                         \
    __dlArgStream << #ARG_1 "=" << ARG_1;                                   \
    __dlArgStream << "," << #ARG_2 "=" << ARG_2;                            \
    __dlArgStream << "," << #ARG_3 "=" << ARG_3;                            \
    __dlArgStream << "," << #ARG_4 "=" << ARG_4;                            \
    __dlArgStream << "," << #ARG_5 "=" << ARG_5;                            \
    __dlArgStream << "," << #ARG_6 "=" << ARG_6;                            \
    _LOG_LEVEL_FUNC( LEVEL, __dlArgStream.str() )

/*! @brief  This macro logs 7 function arguments.
 *   @details It should not be used directly but is instead used by other macros.
 */
#define _LOG_LEVEL_FUNC7( LEVEL, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ARG_6, ARG_7 ) \
    imp::SmartStream __dlArgStream;                                                \
    __dlArgStream << #ARG_1 "=" << ARG_1;                                          \
    __dlArgStream << "," << #ARG_2 "=" << ARG_2;                                   \
    __dlArgStream << "," << #ARG_3 "=" << ARG_3;                                   \
    __dlArgStream << "," << #ARG_4 "=" << ARG_4;                                   \
    __dlArgStream << "," << #ARG_5 "=" << ARG_5;                                   \
    __dlArgStream << "," << #ARG_6 "=" << ARG_6;                                   \
    __dlArgStream << "," << #ARG_7 "=" << ARG_7;                                   \
    _LOG_LEVEL_FUNC( LEVEL, __dlArgStream.str() )

/*! @brief  Macros to log functions with 0-7 arguments at either Trace or Debug level.
 *   @details They should not be used directly but are instead used by other macros.
 */
#define LOG_TRACE_FUNC0( ... )                                     _LOG_LEVEL_FUNC( true, "" )
#define LOG_DEBUG_FUNC0( ... )                                     _LOG_LEVEL_FUNC( false, "" )
#define LOG_TRACE_FUNC1( UNUSED, ARG_1, ... )                      _LOG_LEVEL_FUNC1( true, ARG_1 )
#define LOG_DEBUG_FUNC1( UNUSED, ARG_1, ... )                      _LOG_LEVEL_FUNC1( false, ARG_1 )
#define LOG_TRACE_FUNC2( UNUSED, ARG_1, ARG_2, ... )               _LOG_LEVEL_FUNC2( true, ARG_1, ARG_2 )
#define LOG_DEBUG_FUNC2( UNUSED, ARG_1, ARG_2, ... )               _LOG_LEVEL_FUNC2( false, ARG_1, ARG_2 )
#define LOG_TRACE_FUNC3( UNUSED, ARG_1, ARG_2, ARG_3, ... )        _LOG_LEVEL_FUNC3( true, ARG_1, ARG_2, ARG_3 )
#define LOG_DEBUG_FUNC3( UNUSED, ARG_1, ARG_2, ARG_3, ... )        _LOG_LEVEL_FUNC3( false, ARG_1, ARG_2, ARG_3 )
#define LOG_TRACE_FUNC4( UNUSED, ARG_1, ARG_2, ARG_3, ARG_4, ... ) _LOG_LEVEL_FUNC4( true, ARG_1, ARG_2, ARG_3, ARG_4 )
#define LOG_DEBUG_FUNC4( UNUSED, ARG_1, ARG_2, ARG_3, ARG_4, ... ) _LOG_LEVEL_FUNC4( false, ARG_1, ARG_2, ARG_3, ARG_4 )
#define LOG_TRACE_FUNC5( UNUSED, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ... ) \
    _LOG_LEVEL_FUNC5( true, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5 )
#define LOG_DEBUG_FUNC5( UNUSED, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ... ) \
    _LOG_LEVEL_FUNC5( false, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5 )
#define LOG_TRACE_FUNC6( UNUSED, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ARG_6, ... ) \
    _LOG_LEVEL_FUNC6( true, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ARG_6 )
#define LOG_DEBUG_FUNC6( UNUSED, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ARG_6, ... ) \
    _LOG_LEVEL_FUNC6( false, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ARG_6 )
#define LOG_TRACE_FUNC7( UNUSED, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ARG_6, ARG_7, ... ) \
    _LOG_LEVEL_FUNC7( true, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ARG_6, ARG_7 )
#define LOG_DEBUG_FUNC7( UNUSED, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ARG_6, ARG_7, ... ) \
    _LOG_LEVEL_FUNC7( false, ARG_1, ARG_2, ARG_3, ARG_4, ARG_5, ARG_6, ARG_7 )

/*! @brief  Macros to select the appropriate macro based on the argument count.
 *   @details They should not be used directly but are instead used by other macros.
 */
#define _LOG_TRACE_FUNC( ... )     \
    EXPAND( GET_FUNC_LOGGER_MACRO( \
        __VA_ARGS__,               \
        LOG_TRACE_FUNC7,           \
        LOG_TRACE_FUNC6,           \
        LOG_TRACE_FUNC5,           \
        LOG_TRACE_FUNC4,           \
        LOG_TRACE_FUNC3,           \
        LOG_TRACE_FUNC2,           \
        LOG_TRACE_FUNC1,           \
        LOG_TRACE_FUNC0 )( __VA_ARGS__ ) )
#define _LOG_DEBUG_FUNC( ... )     \
    EXPAND( GET_FUNC_LOGGER_MACRO( \
        __VA_ARGS__,               \
        LOG_DEBUG_FUNC7,           \
        LOG_DEBUG_FUNC6,           \
        LOG_DEBUG_FUNC5,           \
        LOG_DEBUG_FUNC4,           \
        LOG_DEBUG_FUNC3,           \
        LOG_DEBUG_FUNC2,           \
        LOG_DEBUG_FUNC1,           \
        LOG_DEBUG_FUNC0 )( __VA_ARGS__ ) )
