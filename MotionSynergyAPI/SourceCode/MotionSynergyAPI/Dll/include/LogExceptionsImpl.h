/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

/*!
 *  @brief     The following macros should not be used directly in code.
 *             Instead use the public macros defined in LogExceptions.h.
 */
#pragma once

#include "CommonExceptionsImpl.h"
#include "DiagnosticLogImpl.h"

/*! @brief  This macro logs and throws an exception. The log information includes
 *           a stack trace.
 *   @details It should not be used directly but is instead used by other macros.
 */
#define _LOG_LEVEL_THROW_EXCEPTION( LogLevel, Message, ExceptionClass ) \
    _PREPARE_EXCEPTION_INFO( Message, ExceptionClass )                  \
    LOG_##LogLevel( _ostr.str() ) _THROW_EXCEPTION( ExceptionClass )
