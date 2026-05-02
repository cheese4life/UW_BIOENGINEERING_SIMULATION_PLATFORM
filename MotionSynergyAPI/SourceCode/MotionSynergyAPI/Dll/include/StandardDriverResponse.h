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

namespace StandardDriverResponse
{
/*!
 * @brief The number of response values to allocate per device type.
 */
static constexpr uint32_t MaxResponsesPerDriverType = 128;

/*!
 * @brief The standard set of responses supported by all device drivers
 */
enum Enum
{
    DriverSuccess,
    DriverCommunicationFailure,
    DriverGeneralError,
    DriverBusy,
    DriverInvalidState,
    DriverUnableToGetOrSetValue,
    DriverActionFailed,
    DriverHalt,
    DriverEStop,
    LAST_ENUM,

    // Driver-specific responses must start here to avoid clashing with
    // the standard responses.
    FirstDriverSpecificResponse = 128,

    FirstAxisDriverResponse = FirstDriverSpecificResponse,
    LastAxisDriverResponse = FirstAxisDriverResponse + ( MaxResponsesPerDriverType - 1 ),
    FirstBarcodeScannerDriverResponse,
    LastBarcodeScannerDriverResponse = FirstBarcodeScannerDriverResponse + ( MaxResponsesPerDriverType - 1 ),
};

VARNUM_FUNCTIONS_DECL
} // namespace StandardDriverResponse

} //  namespace imp
