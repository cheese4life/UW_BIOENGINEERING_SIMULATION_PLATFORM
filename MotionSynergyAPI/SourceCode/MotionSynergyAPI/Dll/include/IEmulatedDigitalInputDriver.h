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
enum class DigitalIOBit;
enum class DigitalIOBitState;

/*!
 * @brief    Common interface for emulated Digital Input drivers
 */
class IEmulatedDigitalInputDriver : public virtual IEmulatedDriver
{
public:
    virtual ~IEmulatedDigitalInputDriver() = default;

    /*!
     *   @brief      Notifies the emulated driver that the input state of a bit has changed
     *   @param[in]  bit     The digital input bit that has changed
     *   @param[in]  state   The new state of the digital input bit
     *   @return     void
     */
    virtual void OnInputBitStateChange( DigitalIOBit bit, DigitalIOBitState state ) = 0;

    /*!
     *   @brief      Add to a list of input bit changes for the emulated driver to perform before each read
     *   @param[in]  bit     The digital input bit to change
     *   @param[in]  state   The new state of the digital input bit
     *   @return     void
     */
    virtual void AddToInputBitStateChangeList( DigitalIOBit bit, DigitalIOBitState state ) = 0;
};

} //  namespace imp