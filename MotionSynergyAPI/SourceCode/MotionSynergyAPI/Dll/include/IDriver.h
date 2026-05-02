/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <string>

namespace imp
{

/*!
 * @brief    Common interface for all drivers.
 * @details  All drivers (whether associated with a device, or an extension for support
 *           in emulation) shall be identifiable by a name, have a driver type as a
 *           string, and shall be recognised as emulated (or not).
 */
class IDriver
{
public:
    /*! @brief  Constructor. */
    IDriver() = default;

    /*! @brief  Destructor. */
    virtual ~IDriver() = default;

    /*!
     * @brief   Get the name of this driver.
     * @returns The name of this driver.
     */
    virtual std::string Name() const = 0;

    /*!
     *   @brief  The type of driver.
     *   @return The type of driver.
     */
    virtual std::string Type() const = 0;

    /*!
     *   @brief  Is this driver emulated or does it control real hardware.
     *   @return true if the driver is emulated, false if it controls real hardware.
     */
    virtual bool IsEmulated() const = 0;
};

} //  namespace imp
