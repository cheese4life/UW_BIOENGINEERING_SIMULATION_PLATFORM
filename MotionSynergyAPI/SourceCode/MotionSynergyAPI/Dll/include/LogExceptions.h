/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "DiagnosticLog.h"
#include "LogExceptionsImpl.h"


/*! @brief  Macros to log and throw various exception types at various log levels.
 *           The log information includes a stack trace.
 *   @note   These macros require LOG_TRACE_FUNC() or LOG_DEBUG_FUNC() to have been used within the current function.
 */
#define LOG_TRACE_THROW_EXCEPTION( Message, ExceptionClass ) \
    _LOG_LEVEL_THROW_EXCEPTION( TRACE, Message, ExceptionClass )
#define LOG_DEBUG_THROW_EXCEPTION( Message, ExceptionClass ) \
    _LOG_LEVEL_THROW_EXCEPTION( DEBUG, Message, ExceptionClass )
#define LOG_INFO_THROW_EXCEPTION( Message, ExceptionClass ) _LOG_LEVEL_THROW_EXCEPTION( INFO, Message, ExceptionClass )
#define LOG_WARNING_THROW_EXCEPTION( Message, ExceptionClass ) \
    _LOG_LEVEL_THROW_EXCEPTION( WARNING, Message, ExceptionClass )
#define LOG_ERROR_THROW_EXCEPTION( Message, ExceptionClass ) \
    _LOG_LEVEL_THROW_EXCEPTION( ERROR, Message, ExceptionClass )
#define LOG_FATAL_THROW_EXCEPTION( Message, ExceptionClass ) \
    _LOG_LEVEL_THROW_EXCEPTION( FATAL, Message, ExceptionClass )

#define LOG_TRACE_THROW_RUNTIME_ERROR( Message )   _LOG_LEVEL_THROW_EXCEPTION( TRACE, Message, std::runtime_error )
#define LOG_DEBUG_THROW_RUNTIME_ERROR( Message )   _LOG_LEVEL_THROW_EXCEPTION( DEBUG, Message, std::runtime_error )
#define LOG_INFO_THROW_RUNTIME_ERROR( Message )    _LOG_LEVEL_THROW_EXCEPTION( INFO, Message, std::runtime_error )
#define LOG_WARNING_THROW_RUNTIME_ERROR( Message ) _LOG_LEVEL_THROW_EXCEPTION( WARNING, Message, std::runtime_error )
#define LOG_ERROR_THROW_RUNTIME_ERROR( Message )   _LOG_LEVEL_THROW_EXCEPTION( ERROR, Message, std::runtime_error )
#define LOG_FATAL_THROW_RUNTIME_ERROR( Message )   _LOG_LEVEL_THROW_EXCEPTION( FATAL, Message, std::runtime_error )

#define LOG_TRACE_THROW_LOGIC_ERROR( Message )   _LOG_LEVEL_THROW_EXCEPTION( TRACE, Message, std::logic_error )
#define LOG_DEBUG_THROW_LOGIC_ERROR( Message )   _LOG_LEVEL_THROW_EXCEPTION( DEBUG, Message, std::logic_error )
#define LOG_INFO_THROW_LOGIC_ERROR( Message )    _LOG_LEVEL_THROW_EXCEPTION( INFO, Message, std::logic_error )
#define LOG_WARNING_THROW_LOGIC_ERROR( Message ) _LOG_LEVEL_THROW_EXCEPTION( WARNING, Message, std::logic_error )
#define LOG_ERROR_THROW_LOGIC_ERROR( Message )   _LOG_LEVEL_THROW_EXCEPTION( ERROR, Message, std::logic_error )
#define LOG_FATAL_THROW_LOGIC_ERROR( Message )   _LOG_LEVEL_THROW_EXCEPTION( FATAL, Message, std::logic_error )

#define LOG_TRACE_THROW_INVALID_ARGUMENT( Message ) _LOG_LEVEL_THROW_EXCEPTION( TRACE, Message, std::invalid_argument )
#define LOG_DEBUG_THROW_INVALID_ARGUMENT( Message ) _LOG_LEVEL_THROW_EXCEPTION( DEBUG, Message, std::invalid_argument )
#define LOG_INFO_THROW_INVALID_ARGUMENT( Message )  _LOG_LEVEL_THROW_EXCEPTION( INFO, Message, std::invalid_argument )
#define LOG_WARNING_THROW_INVALID_ARGUMENT( Message ) \
    _LOG_LEVEL_THROW_EXCEPTION( WARNING, Message, std::invalid_argument )
#define LOG_ERROR_THROW_INVALID_ARGUMENT( Message ) _LOG_LEVEL_THROW_EXCEPTION( ERROR, Message, std::invalid_argument )
#define LOG_FATAL_THROW_INVALID_ARGUMENT( Message ) _LOG_LEVEL_THROW_EXCEPTION( FATAL, Message, std::invalid_argument )

#define LOG_TRACE_THROW_OUT_OF_RANGE( Message )   _LOG_LEVEL_THROW_EXCEPTION( TRACE, Message, std::out_of_range )
#define LOG_DEBUG_THROW_OUT_OF_RANGE( Message )   _LOG_LEVEL_THROW_EXCEPTION( DEBUG, Message, std::out_of_range )
#define LOG_INFO_THROW_OUT_OF_RANGE( Message )    _LOG_LEVEL_THROW_EXCEPTION( INFO, Message, std::out_of_range )
#define LOG_WARNING_THROW_OUT_OF_RANGE( Message ) _LOG_LEVEL_THROW_EXCEPTION( WARNING, Message, std::out_of_range )
#define LOG_ERROR_THROW_OUT_OF_RANGE( Message )   _LOG_LEVEL_THROW_EXCEPTION( ERROR, Message, std::out_of_range )
#define LOG_FATAL_THROW_OUT_OF_RANGE( Message )   _LOG_LEVEL_THROW_EXCEPTION( FATAL, Message, std::out_of_range )

/*!
 *  @brief   Assertion which logs and throws an exception.
 *  @details Evaluates the given expression, and if false, outputs to stderr,
 *           logs the exception details including a stack trace to file
 *           and throws a std::logic_error exception
 *           with a message giving the expression, and appending the file,
 *           line number and function name.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *  @note    This macro requires LOG_TRACE_FUNC() or LOG_DEBUG_FUNC() to have been used within the current function.
 *
 *  @param[in]   Expression     The expression to assert true.
 */
#define LOG_ASSERT( Expression )                                            \
    {                                                                       \
        if ( !( Expression ) )                                              \
            LOG_FATAL_THROW_LOGIC_ERROR( "Assertion failed: " #Expression ) \
    }

/*!
 *  @brief   Assertion which logs and throws an exception.
 *  @details Evaluates the given expression, and if false, outputs to stderr,
 *           logs the exception details including a stack trace to file
 *           and throws a std::logic_error exception
 *           with a message giving the expression, and appending the file,
 *           line number and function name.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *  @note    This macro requires LOG_TRACE_FUNC() or LOG_DEBUG_FUNC() to have been used within the current function.
 *
 *  @param[in]   Expression     The expression to assert true.
 *  @param[in]   Message        A message describing the assertion.
 */
#define LOG_ASSERT_MSG( Expression, Message )                                             \
    {                                                                                     \
        if ( !( Expression ) )                                                            \
            _PREPARE_EXCEPTION_INFO( "Assertion failed: " #Expression, std::logic_error ) \
        _ostr << ", Message: " << Message;                                                \
        LOG_FATAL( _ostr.str() )                                                          \
        _THROW_EXCEPTION( std::logic_error )                                              \
    }

/*!
 *  @brief   A special assert for use in destructors which does not throw.
 *  @details Evaluates the given expression, and if false,
 *           logs the details including a stack trace to file and
 *           outputs to stderr with a message giving the expression, and
 *           appending the file, line number and function name.
 *           Will assert() in debug builds and abort() in release builds.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *  @note    This macro requires LOG_TRACE_FUNC() or LOG_DEBUG_FUNC() to have been used within the current function.
 *
 *  @param[in]   Expression     The expression to assert true.
 */
#define LOG_DESTRUCTOR_ASSERT( Expression )                                   \
    {                                                                         \
        if ( !( Expression ) )                                                \
            _PREPARE_EXCEPTION_INFO( "Assertion failed: " #Expression, None ) \
        std::cerr << _ostr.str() << std::endl << std::endl;                   \
        LOG_FATAL( _ostr.str() )                                              \
        assert( Expression );                                                 \
        abort();                                                              \
    }                                                                         \
    }

/*!
 *  @brief   A special assert for use in destructors which does not throw.
 *  @details Evaluates the given expression, and if false,
 *           logs the details including a stack trace to file and
 *           outputs to stderr with a message giving the expression, and
 *           appending the file, line number and function name.
 *           Will assert() in debug builds and abort() in release builds.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *  @note    This macro requires LOG_TRACE_FUNC() or LOG_DEBUG_FUNC() to have been used within the current function.
 *
 *  @param[in]   Expression     The expression to assert true.
 *  @param[in]   Message        A message describing the assertion.
 */
#define LOG_DESTRUCTOR_ASSERT_MSG( Expression, Message )                      \
    {                                                                         \
        if ( !( Expression ) )                                                \
            _PREPARE_EXCEPTION_INFO( "Assertion failed: " #Expression, None ) \
        _ostr << ", Message: " << Message;                                    \
        std::cerr << _ostr.str() << std::endl << std::endl;                   \
        LOG_FATAL( _ostr.str() )                                              \
        assert( Expression );                                                 \
        abort();                                                              \
    }                                                                         \
    }

/*!
 *  @brief   Assertion which logs and throws an exception if a function argument is invalid.
 *  @details Evaluates the given expression, and if false,
 *           logs the exception details including a stack trace to file
 *           outputs to stderr and throws a std::invalid_argument exception
 *           with a message giving the expression, and appending the file,
 *           line number and function name.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *  @note    This macro requires LOG_TRACE_FUNC() or LOG_DEBUG_FUNC() to have been used within the current function.
 *
 *  @param[in]   Expression     The expression to assert true.
 */
#define LOG_ASSERT_ARG_VALID( Expression )                                       \
    {                                                                            \
        if ( !( Expression ) )                                                   \
            LOG_FATAL_THROW_INVALID_ARGUMENT( "Invalid Argument: " #Expression ) \
    }

/*!
 *  @brief   Assertion which logs and throws an exception if a function argument is invalid.
 *  @details Evaluates the given expression, and if false,
 *           logs the exception details including a stack trace to file
 *           outputs to stderr and throws a std::invalid_argument exception
 *           with a message giving the expression, and appending the file,
 *           line number and function name.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *  @note    This macro requires LOG_TRACE_FUNC() or LOG_DEBUG_FUNC() to have been used within the current function.
 *
 *  @param[in]   Expression     The expression to assert true.
 *  @param[in]   Message        A message describing the assertion.
 */
#define LOG_ASSERT_ARG_VALID_MSG( Expression, Message )                                        \
    {                                                                                          \
        if ( !( Expression ) )                                                                 \
            _PREPARE_EXCEPTION_INFO( "Assertion failed: " #Expression, std::invalid_argument ) \
        _ostr << ", Message: " << Message;                                                     \
        LOG_FATAL( _ostr.str() )                                                               \
        _THROW_EXCEPTION( std::invalid_argument )                                              \
    }
