/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ICCPDevice.h"

namespace imp
{
// Forward declarations
class IDigitalInputDevice;
class IDigitalOutputDevice;
class LuaDevice;

// Forward declarations
class DigitalIOChangeSubscription;


/*!
 *  @brief   Defines the interface a DigitalIO device must adhere to
 *           for allowing clients to subscribe for change notifications.
 *  @details We inherit from @link ICCPDevice @endlink here because
 *           @link LuaDevice @endlink had issues when multiple/virtual
 *           inheritance was used in derived classes; see @link IDigitalInputDevice @endlink
 *           and @link IDigitalOutputDevice @endlink for more details.
 */
class IDigitalIOPublisher : public virtual ICCPDevice
{
public:
    /*!
     *   @brief  Virtual destructor (empty).
     */
    virtual ~IDigitalIOPublisher() = default;

    /*!
     *   @brief      Queries whether the device supports change notification
     *   @return     true if the device supports change notification
     */
    virtual bool ChangeNotificationSupported() const = 0;

    /*!
     *   @brief      Queries whether change notification is enabled
     *   @return     true if change notification is enabled
     *   @throw      std::runtime_error if the device does not support change notification
     */
    virtual bool ChangeNotificationEnabled() = 0;

    /*!
     *   @brief      Enables notification of change
     *   @throw      std::runtime_error if the device does not support change notification
     *   @note       A Wait() call should follow this to determine if the notification was successfully enabled
     */
    virtual void EnableChangeNotification() = 0;

    /*!
     *   @brief      Disables notification of change
     *   @throw      std::runtime_error if the device does not support change notification
     *   @note       A Wait() call should follow this to determine if the notification was successfully disabled
     */
    virtual void DisableChangeNotification() = 0;

    /*!
     *   @brief      Subscribe for notification of change
     *   @param[in]  subscription details of the DigitalIO change subscription being made
     *   @return     A unique identifier allowing the subscription to be unsubscribed
     *   @throw      std::runtime_error if the device does not support change notification
     */
    virtual SubscriptionId SubscribeChangeNotification( const DigitalIOChangeSubscription& subscription ) = 0;

    /*!
     *   @brief      Unsubscribe for notification of change
     *   @param[in]  subscriptionId The unique identifier of the subscription as returned by
     *               @link SubscribeChangeNotification @endlink
     *   @return     true if the a subscription with the given ID was successfully unsubscribed
     *   @throw      std::runtime_error if the device does not support change notification
     */
    virtual bool UnsubscribeChangeNotification( const SubscriptionId& subscriptionId ) = 0;
};

} //  namespace imp
