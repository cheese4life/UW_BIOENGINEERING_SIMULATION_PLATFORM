/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "Enum.h"

namespace Alert
{
namespace Severity
{
/*! @brief   Defines the severity of an alert. */
enum Enum
{
    Undefined,
    Low,
    Medium,
    High,
    SafetyCritical,
    LAST_ENUM
};

ENUM_FUNCTIONS_DECL
}
} // namespace Alert

