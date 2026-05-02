/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDeviceDriver.h"

namespace imp
{

class IDigitalInputDriver;

/*! @brief  Shorthand for creating shared pointers of IDigitalInputDriver. */
typedef std::shared_ptr<IDigitalInputDriver> IDigitalInputDriverSPtr;

/*!
 *  @brief   Defines the interface a DigitalInputDriver must adhere to within the CCP
 */
class IDigitalInputDriver : public virtual IDeviceDriver
{
public:
    IDigitalInputDriver() = default;
    virtual ~IDigitalInputDriver() = default;

    /*!
     *   @brief      Gets the state of all the bits as one byte.
     *   @param[in]  completionCb    Function to return success or failure
     *               when call is complete along with a byte reflecting the
     *               states of input bits.
     */
    virtual void GetInputRegister( std::function<void( DriverResponseType, uint8_t )> completionCb ) = 0;

    /*!
     *   @brief      Queries whether the driver supports caching of input values.
     *   @return     true if the driver supports caching of input values.
     */
    virtual bool IsCachedInputSupported() const = 0;

    /*!
     *   @brief      Synchronously gets the cached state of all the bits as one byte.
     *               This should only be called if IsCachedInputSupported() returns true.
     *   @return     The input register value.
     */
    virtual uint8_t GetCachedInputRegister() = 0;

    /*!
     *   @brief      Queries whether the driver supports input change notification
     *   @return     true if the driver supports input change notification
     */
    virtual bool InputChangeNotificationSupported() const = 0;

    typedef std::function<void( uint8_t inputRegister )> InputRegisterCallback;

    /*!
     *   @brief      Subscribe for notification of input change
     *   @param[in]  callback        Callback to be invoked each time the
     *               state of input bits change.
     *   @param[in]  completionCb    Function to return success or failure
     *               when call is complete.
     *   @throw      std::runtime_error if the device does not support input change notification
     */
    virtual void SubscribeInputChangeNotification(
        InputRegisterCallback callback,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Unsubscribe for notification of input change
     *   @param[in]  completionCb    Function to return success or failure
     *               when call is complete.
     */
    virtual void UnsubscribeInputChangeNotification( std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *  @brief      Check if this digital input driver requires Initialize to be called.
     *  @details    Some hardware devices may support multiple driver interfaces (e.g. an axis with some GPIO lines).
     *              In this case, the hardware only needs to be initialized through one driver interface (e.g. the axis, not the digital input/outputs).
     *  @returns    true if initialize should be called, false if not.
     */
    virtual bool IsDigitalInputInitializeRequired() const = 0;
};

} //  namespace imp
