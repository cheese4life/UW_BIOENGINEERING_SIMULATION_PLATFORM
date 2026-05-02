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

enum class WaitResult : int32_t
{
    Success,   /*!< @brief Command completed successfully. */
    Timeout,   /*!< @brief Command failed to complete within the timeout period, command is still in progress. */
    Cancelled, /*!< @brief Wait was cancelled, command is still in progress. */
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( WaitResult )

} //  namespace imp
