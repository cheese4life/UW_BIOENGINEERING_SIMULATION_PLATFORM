/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "EnumClass.h"

namespace imp
{

/*!
 *  @brief     Defines a set of log levels ranging from the most verbose (Trace)
 *             to the least (Fatal).
 */
enum class LogLevel : int32_t
{
    Trace = 0,   /*!< Trace level for verbose code path evaluation. */
    Debug = 1,   /*!< Debug level for verbose results and values. */
    Info = 2,    /*!< Info level for significant events. */
    Warning = 3, /*!< Warning level for potential/handled error cases. */
    Error = 4,   /*!< Error level for non-fatal error cases. */
    Fatal = 5,   /*!< Fatal level for application closure cases. */
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( LogLevel )

} //  namespace imp
