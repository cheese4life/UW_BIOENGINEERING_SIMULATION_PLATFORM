/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IEmulatedDriver.h"

namespace imp
{

class IEmulatedAxisDriver;
using IEmulatedAxisDriverSPtr = std::shared_ptr<IEmulatedAxisDriver>;

/*!
 *  @brief   Defines a test interface an emulated Axis driver must adhere to.
 *           This allows the behaviour of the emulated axis to be controlled
 *           for both happy day cases and error cases.
 */
class IEmulatedAxisDriver : public virtual IEmulatedDriver
{
public:
    virtual ~IEmulatedAxisDriver() = default;

    /*!
     *   @brief      Sets the current position of the motor. Useful for
     *               emulating different motor positions at power up.
     *   @param[in]  position            New position of the motor
     */
    virtual void SetCurrentPosition( double position ) = 0;

    /*!
     *   @brief      Sets the enable status of the emulated flag range. When
     *               disabled, the flag status returned within the range is always zero.
     *   @param[in]  flagIndex   Driver-dependent flag index to search for
     *   @param[in]  enable      Enable or disable flag status detection
     */
    virtual void SetFlagStatusEnable( uint8_t flagIndex, bool enable ) = 0;

    /*!
     * @brief   Inject an asynchronous error detected while the axis is Idle.
     *          This provides a simple mechanism for injecting asynchronous errors.
     * @param[in] response The response code the driver will return via the IdleErrorCallback.
     */
    virtual void InjectErrorWhileIdle( DriverResponseType response ) = 0;
};

} //  namespace imp
