/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "Enum.h"
#include "StandardDeviceResponse.h"


namespace imp
{

namespace AxisDeviceResponse
{
/*!
 * @brief The set of responses specific to the axis device
 */
enum Enum
{
    AxisDevicePositionError = StandardDeviceResponse::FirstAxisDeviceResponse,
    AxisDeviceOverTemperature,
    AxisDeviceCurrentFoldback,
    AxisDeviceOverVoltage,
    AxisDeviceUnderVoltage,
    AxisDeviceDriveDisabled,
    AxisDeviceGroundFault,
    AxisDeviceOverCurrent,
    AxisDeviceCommutationError,
    AxisDeviceProcessorReset,
    LAST_ENUM = 10
};
VARNUM_FUNCTIONS_DECL
} // namespace AxisDeviceResponse

} //  namespace imp
