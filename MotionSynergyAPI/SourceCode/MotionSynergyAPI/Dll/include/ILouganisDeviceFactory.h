/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDeviceFactory.h"
#include "IDeviceFactoryRegistration.h"

namespace imp
{

/*!
 * @brief   Specifies the combined interface which must be implemented by the LouganisDeviceFactory.
 */
class ILouganisDeviceFactory
    : public IDeviceFactory
    , public IDeviceFactoryRegistration
{
public:
    ILouganisDeviceFactory() = default;
};

} //  namespace imp
