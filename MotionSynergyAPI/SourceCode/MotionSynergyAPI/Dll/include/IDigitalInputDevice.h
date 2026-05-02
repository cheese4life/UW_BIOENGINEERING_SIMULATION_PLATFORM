/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDigitalIOReader.h"


namespace imp
{
class LuaDevice;

/*!
 *  @brief   Defines the interface a Digital Input device must adhere to
 *           for building motion control systems within CCP
 *  @details Using virtual inheritance here caused issues when
 *           using dynamic_cast to this type from @link ICCPDevice @endlink ;
 *           see the GetDevice method of @link LuaDevice @endlink for details.
 */
class IDigitalInputDevice : public IDigitalIOReader
{
public:
    /*!
     *   @brief  Virtual destructor (empty).
     */
    virtual ~IDigitalInputDevice() = default;
};

} //  namespace imp
