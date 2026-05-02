/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "VarnumInfo.h"


namespace imp
{

class DriverResponseType;

/*!
 * @brief A VarnumInfo which holds a device response.
 */
class DriverResponseTypeInfo : public VarnumInfo<DriverResponseType, int32_t>
{
public:
    DriverResponseTypeInfo() = delete;
};

} //  namespace imp
