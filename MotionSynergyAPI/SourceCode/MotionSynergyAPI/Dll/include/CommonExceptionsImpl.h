/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once


#include "StackTrace.h"

#include <boost/current_function.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <exception>
#include <iostream>
#include <sstream>

#define _PREPARE_EXCEPTION_INFO( Message, ExceptionClass )                                    \
    {                                                                                         \
        boost::posix_time::ptime const now = boost::posix_time::microsec_clock::local_time(); \
        std::string const _currentTime = boost::posix_time::to_simple_string( now );          \
        std::ostringstream _ostr;                                                             \
        _ostr << "Exception: " << Message << std::endl                                        \
              << "Type: " << #ExceptionClass << std::endl                                     \
              << "File: " << std::string( __FILE__ ) << std::endl                             \
              << "Line: " << __LINE__ << std::endl                                            \
              << "Function: " << BOOST_CURRENT_FUNCTION << std::endl                          \
              << "Time: " << _currentTime << std::endl                                        \
              << "StackTrace: " << imp::StackTrace::GetStackTrace();

#define _THROW_EXCEPTION( ExceptionClass ) \
    throw ExceptionClass( _ostr.str() );   \
    }

#define _OUTPUT_EXCEPTION_TO_CONSOLE          std::cerr << _ostr.str() << std::endl << std::endl;
#define _OUTPUT_MESSAGE_TO_CONSOLE( Message ) _ostr << ", Message: " << Message;

// variable args ignored here - detailed messages are only in SCP
#define _THROW_EXCEPTION_IMPL( Message, ExceptionClass ) \
    _PREPARE_EXCEPTION_INFO( Message, ExceptionClass )   \
    _THROW_EXCEPTION( ExceptionClass )
