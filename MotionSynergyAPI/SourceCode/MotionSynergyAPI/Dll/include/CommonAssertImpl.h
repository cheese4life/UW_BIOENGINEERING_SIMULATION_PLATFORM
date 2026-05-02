/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonExceptionsImpl.h"
#include "TypeTraits.h"
#include <cassert>


#define _ABORT_APPLICATION abort();

#define _ASSERT_IMPL( Expression )                                                        \
    do                                                                                    \
    {                                                                                     \
        static_assert(                                                                    \
            !imp::IsStringLiteral<decltype( Expression )>::value,                         \
            "String literals are not valid assertion expressions" );                      \
        if ( !( Expression ) )                                                            \
            _PREPARE_EXCEPTION_INFO( "Assertion failed: " #Expression, std::logic_error ) \
        _OUTPUT_EXCEPTION_TO_CONSOLE                                                      \
        _THROW_EXCEPTION( std::logic_error )                                              \
    }                                                                                     \
    while ( false )

#define _ASSERT_MSG_IMPL( Expression, Message )                                           \
    do                                                                                    \
    {                                                                                     \
        static_assert(                                                                    \
            !imp::IsStringLiteral<decltype( Expression )>::value,                         \
            "String literals are not valid assertion expressions" );                      \
        if ( !( Expression ) )                                                            \
            _PREPARE_EXCEPTION_INFO( "Assertion failed: " #Expression, std::logic_error ) \
        _OUTPUT_MESSAGE_TO_CONSOLE( Message )                                             \
        _OUTPUT_EXCEPTION_TO_CONSOLE                                                      \
        _THROW_EXCEPTION( std::logic_error )                                              \
    }                                                                                     \
    while ( false )

#define _DESTRUCTOR_ASSERT_IMPL( Expression )                                 \
    do                                                                        \
    {                                                                         \
        static_assert(                                                        \
            !imp::IsStringLiteral<decltype( Expression )>::value,             \
            "String literals are not valid assertion expressions" );          \
        if ( !( Expression ) )                                                \
            _PREPARE_EXCEPTION_INFO( "Assertion failed: " #Expression, None ) \
        _OUTPUT_EXCEPTION_TO_CONSOLE                                          \
        assert( Expression );                                                 \
        _ABORT_APPLICATION                                                    \
    }                                                                         \
    }                                                                         \
    while ( false )

#define _DESTRUCTOR_ASSERT_MSG_IMPL( Expression, Message )                    \
    do                                                                        \
    {                                                                         \
        static_assert(                                                        \
            !imp::IsStringLiteral<decltype( Expression )>::value,             \
            "String literals are not valid assertion expressions" );          \
        if ( !( Expression ) )                                                \
            _PREPARE_EXCEPTION_INFO( "Assertion failed: " #Expression, None ) \
        _OUTPUT_MESSAGE_TO_CONSOLE( Message )                                 \
        _OUTPUT_EXCEPTION_TO_CONSOLE                                          \
        assert( Expression );                                                 \
        _ABORT_APPLICATION                                                    \
    }                                                                         \
    }                                                                         \
    while ( false )

#define _ASSERT_ARG_VALID_IMPL( Expression )                                                   \
    do                                                                                         \
    {                                                                                          \
        static_assert(                                                                         \
            !imp::IsStringLiteral<decltype( Expression )>::value,                              \
            "String literals are not valid assertion expressions" );                           \
        if ( !( Expression ) )                                                                 \
            _PREPARE_EXCEPTION_INFO( "Invalid Argument: " #Expression, std::invalid_argument ) \
        _OUTPUT_EXCEPTION_TO_CONSOLE                                                           \
        _THROW_EXCEPTION( std::invalid_argument )                                              \
    }                                                                                          \
    while ( false )

#define _ASSERT_ARG_VALID_MSG_IMPL( Expression, Message )                                      \
    do                                                                                         \
    {                                                                                          \
        static_assert(                                                                         \
            !imp::IsStringLiteral<decltype( Expression )>::value,                              \
            "String literals are not valid assertion expressions" );                           \
        if ( !( Expression ) )                                                                 \
            _PREPARE_EXCEPTION_INFO( "Invalid Argument: " #Expression, std::invalid_argument ) \
        _OUTPUT_MESSAGE_TO_CONSOLE( Message )                                                  \
        _OUTPUT_EXCEPTION_TO_CONSOLE                                                           \
        _THROW_EXCEPTION( std::invalid_argument )                                              \
    }                                                                                          \
    while ( false )
