/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "Enum.h"

#include <cstdint>


namespace imp
{

namespace StandardDeviceResponse
{
/*!
 * @brief The number of response values to allocate per device type.
 */
static constexpr uint32_t MaxResponsesPerDeviceType = 128;

/*!
 * @brief The standard set of responses supported by all devices
 */
enum Enum
{
    DeviceSuccess,
    DeviceCommunicationFailure,
    DeviceGeneralError,
    DeviceBusy,
    DeviceInvalidState,
    DeviceUnableToGetOrSetValue,
    DeviceActionFailed,
    DeviceTimeout,
    DeviceHalt,
    DeviceEStop,
    LAST_ENUM,

    // Device-specific responses must start here to avoid clashing with
    // the standard responses.
    FirstDeviceSpecificResponse = 128,

    FirstAxisDeviceResponse = FirstDeviceSpecificResponse,
    LastAxisDeviceResponse = FirstAxisDeviceResponse + ( MaxResponsesPerDeviceType - 1 ),
    FirstBarcodeScannerDeviceResponse,
    LastBarcodeScannerDeviceResponse = FirstBarcodeScannerDeviceResponse + ( MaxResponsesPerDeviceType - 1 ),
};

VARNUM_FUNCTIONS_DECL

} // namespace StandardDeviceResponse

} //  namespace imp
