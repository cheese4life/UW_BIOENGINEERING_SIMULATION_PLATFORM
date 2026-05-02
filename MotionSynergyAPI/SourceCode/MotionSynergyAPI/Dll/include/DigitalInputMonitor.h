/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <IDigitalInputDevice.h>

#include <functional>
#include <string>

namespace DoverMotion
{
namespace IMP
{

using DigitalInputStateChangeCallback = std::function<void()>;

/*!
 *  @brief   Class which will invoke a callback whenever the state changes on
 *           a DigitalInput device. This class uses RAII to subscribe/unsubscribe from
 *           the digital input state changes.
 *  @details Note that in order to receive state change updates from a digital input device,
 *           EnableAsynchChangeUpdates() must first be called. This should be done in the
 *           Initialize.lua script.
 */
class DigitalInputMonitor
{
public:
    /*!
     *  @brief  Constructor will subscribe for digital input state changes.
     *   @param[in]  pDispatcher             The dispatcher on which the callbacks will be invoked.
     *   @param[in]  digitalInputDeviceName  The name of the digital input device to monitor.
     *   @param[in]  monitorBit              The bit in the digital IO to monitor for change on.
     *   @param[in]  offStateChangeCallback  The callback to invoke whenever the digital input state changes to off / low.
     *   @param[in]  onStateChangeCallback   The callback to invoke whenever the digital input state changes to on / high.
     *   @throws A std::runtime_error if the digital input device cannot be found or is the wrong type.
     */
    DigitalInputMonitor(
        imp::IDispatcherSPtr pDispatcher,
        const std::string& digitalInputDeviceName,
        imp::DigitalIOBit monitorBit,
        DigitalInputStateChangeCallback offStateChangeCallback,
        DigitalInputStateChangeCallback onStateChangeCallback );

    /*!
     *  @brief   Destructor automatically unsubscribes from the digital input device.
     */
    virtual ~DigitalInputMonitor();

protected:
    std::shared_ptr<imp::IDigitalInputDevice> GetDigitalInputPublisher( const std::string& digitalInputDeviceName );

private:
    void OnDigitalIOStateChange( imp::DigitalIOBit bit, imp::DigitalIOBitState bitState );

    std::string m_digitalInputDeviceName;
    DigitalInputStateChangeCallback m_offStateChangeCallback;
    DigitalInputStateChangeCallback m_onStateChangeCallback;
    imp::ICCPDevice::SubscriptionId m_subscriberId;
};

} // namespace IMP
} // namespace DoverMotion
