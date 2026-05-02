/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "Enum.h"
#include "StandardDriverResponse.h"


namespace imp
{

namespace AxisDriverResponse
{
/*!
 * @brief The set of responses specific to the axis driver
 */

enum Enum
{
    AxisDriverPositionError = StandardDriverResponse::FirstAxisDriverResponse,
    AxisDriverOverTemperature,
    AxisDriverCurrentFoldback,
    AxisDriverOverVoltage,
    AxisDriverUnderVoltage,
    AxisDriverOverCurrent,
    AxisDriverCommutationError,
    AxisDriverGroundFault,
    AxisDriverDriveDisabled,
    AxisDriverProcessorReset,
    LAST_ENUM = 10
};
VARNUM_FUNCTIONS_DECL
} // namespace AxisDriverResponse

} //  namespace imp
