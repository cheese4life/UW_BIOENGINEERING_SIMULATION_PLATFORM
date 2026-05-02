/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonExceptionsImpl.h"

// Throws an exception of the specified class with the given message, appending
// the file, line number and function name.
#define THROW_EXCEPTION( Message, ExceptionClass ) _THROW_EXCEPTION_IMPL( Message, ExceptionClass )

// Throws std::runtime_error with the given message, appending the file, line
// number and function name.
// Note: std::runtime_error derives from std::exception.
// Used where the error is not determinable until runtime, say due to the OS
// reporting an issue, a 3rd party library reporting an issue or unexpected
// lost connection to hardware.
#define THROW_RUNTIME_ERROR( Message ) THROW_EXCEPTION( Message, std::runtime_error )

// Throws std::logic_error with the given message, appending the file, line
// number and function name.
// Note: std::logic_error derives from std::exception.
// Used when the error is logical (developer error) through improper usage,
// assumptions or arguments. Consider using the specialized forms below instead.
#define THROW_LOGIC_ERROR( Message ) THROW_EXCEPTION( Message, std::logic_error )

// Throws std::invalid_argument with the given message, appending the file, line
// number and function name.
// Note: std::invalid_argument derives from std::logic_error.
// Used when the error is due to an invalid argument (e.g. null or out of range)
#define THROW_INVALID_ARGUMENT( Message ) THROW_EXCEPTION( Message, std::invalid_argument )

// Throws std::out_of_range with the given message, appending the file, line
// number and function name.
// Note: std::out_of_range derives from std::logic_error.
// Used when the error is due to an out of range error (e.g. index to a loop has
// exceeded an arrays bounds).
#define THROW_OUT_OF_RANGE( Message ) THROW_EXCEPTION( Message, std::out_of_range )
