/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <boost/uuid/uuid.hpp>

#include "DeviceResponseType.h"
#include <cstdint>
#include <memory>
#include <string>

namespace imp
{

class IDispatcher;
using IDispatcherSPtr = std::shared_ptr<IDispatcher>;

class IConfiguration;
using IConfigurationSPtr = std::shared_ptr<IConfiguration>;

class ICCPDevice;
using ICCPDeviceSPtr = std::shared_ptr<ICCPDevice>;

/*!
 *  @brief   Defines the interface any device must adhere to for integration with
 *           the CCP.
 */
class ICCPDevice
{
public:
    /*! @brief Common type used for any devices supporting subscription. */
    using SubscriptionId = boost::uuids::uuid;
    static const SubscriptionId InvalidSubscriptionId;

    /*! @brief Function prototype for device action completion callback. */
    using CompletionCallback = std::function<void( DeviceResponseType deviceResponse )>;

    /*! @brief Default destructor. */
    virtual ~ICCPDevice() = default;

    /*!
     *   @brief      Returns the default timeout which can be used in Wait() calls
     *   @return     The default timeout (in milliseconds)
     */
    virtual uint32_t DefaultTimeout() const = 0;

    /*!
     *   @brief      Returns the maximum timeout which can be used in Wait() calls
     *   @return     The maximum timeout (in milliseconds)
     */
    virtual uint32_t MaxTimeout() const = 0;

    /*!
     *   @brief      Wait for up to the default timeout for the previous asynchronous action to complete.
     *   @details    Waits up to the default timeout for any active asynchronous action to complete.
     *   @throw       DeviceException if;
     *               - Any action or attribute access results in a failure of any type
     *               - An EStop was performed on the device.
     *               - The default timeout is reached
     */
    virtual void Wait() = 0;

    /*!
     *   @brief      Wait for the previous asynchronous action to complete.
     *   @details    Waits for any active asynchronous action to complete.
     *   @param[in]  timeout_ms   The length of time to wait before timing out.
     *   @throw       DeviceException if;
     *               - Any action or attribute access results in a failure of any type
     *               - An EStop was performed on the device.
     *               - Timeout is reached
     *               - timeout_ms is > the maximum action timeout (default of 2 minutes)
     */
    virtual void Wait( uint32_t timeout_ms ) = 0;

    /*!
     *   @brief      Wait for up to the default timeout for the previous EStop to complete.
     *   @details    Waits up to the default timeout for any active asynchronous action to complete.
     *               Does not throw an exception if the EStop was successful.
     *   @throw       DeviceException if;
     *               - The EStop action itself fails (An action prematurely
     *                 terminated due to an EStop will not result in an exception)
     *               - The default timeout is reached
     */
    virtual void EStopWait() = 0;

    /*!
     *   @brief      Wait for the previous EStop to complete.
     *   @details    Waits for any active asynchronous action to complete.
     *               Does not throw an exception if the EStop was successful.
     *   @param[in]  timeout_ms   The length of time to wait before timing out.
     *   @throw       DeviceException if;
     *               - The EStop action itself fails (An action prematurely
     *                 terminated due to an EStop will not result in an exception)
     *               - Timeout is reached
     *               - timeout_ms is > the maximum action timeout (default of 2 minutes)
     */
    virtual void EStopWait( uint32_t timeout_ms ) = 0;

    /*!
     *   @brief      Indicates the start of a period where Wait() should not block. If a Wait()
     *               is currently in progress, it will be unblocked and the call will return immediately.
     *   @details    This does not change the state of the device. Wait() must be called again
     *               (after FinishPreventWait()) to ensure the asynchronous action has completed.
     *               This method must be called from another thread.
     */
    virtual void BeginPreventWait() = 0;

    /*!
     *   @brief      Indicates the completion of a period where Wait() should not block. The next
     *               call to Wait() will block until the outstanding asynchronous action has completed.
     *   @details    This method can be called from any thread.
     */
    virtual void FinishPreventWait() = 0;

    /*!
     * @brief       Gets the action progress state of the device.
     * @returns     True if an action is in progress.
     */
    virtual bool IsActionInProgress() = 0;

    /*!
     *   @brief      Subscribes for callback upon asynchronous action completion.
     *   @param[in]  pDispatcher Pointer to the dispatcher the callback is to be invoked on.
     *   @param[in]  callback    The callback to be invoked upon asynchronous action completion.
     *   @throw       Exception if;
     *               - pDispatcher is nullptr
     */
    virtual SubscriptionId
    SubscribeCompletionCallback( const IDispatcherSPtr& pDispatcher, CompletionCallback callback ) = 0;

    /*!
     *   @brief      Unsubscribes for callback upon asynchronous action completion.
     *   @param[in]  subscriptionId Identifier of the subscribed callback.
     */
    virtual void UnsubscribeCompletionCallback( SubscriptionId subscriptionId ) = 0;

    /*!
     *   @brief      Establish a connection to the hardware.
     *   @details    Most commands require initialization before it can be properly used.
     *               However, a few commands only require an established connection.
     *               Calling this is not required if the hardware is already initialized.
     */
    virtual void Connect() = 0;

    /*!
     *   @brief      Initialize the device. A device must be initialized before most other commands can be performed.
     *   @details    In the case of an axis this involves:
     *                - Establishing a connection to the hardware.
     *                - Performing Analog Calibration.
     *                - Performing Commutation.
     */
    virtual void Initialize() = 0;

    /*!
     *   @brief      Perform an emergency stop of the device. This will immediately halt any motion (e.g. an abrupt stop will be
     *               performed for an axis device) and return the device to a safe, passive state
     *               (e.g. for an axis device this means the operating mode is set to disable the axis, motor output, current control, position loop, and trajectory).
     *   @details    The next call to Wait() on this device will throw an exception, indicating the device has been
     *               EStopped and the command in progress was interrupted.
     *               Alternatively, EStopWait() can be called to wait for the estop to complete without throwing an exception.
     *               Initialize() must be called on this device before it will accept further commands.
     */
    virtual void EStop() = 0;

    /*!
     *   @brief      Shutdown the device in preparation for power off. Once a device has been shutdown, it will accept no further commands.
     *   @details    If there is a connection associated with this device and this is the last device using the connection, the connection will be closed.
     */
    virtual void Shutdown() = 0;

    /*!
     *   @brief  Retrieve the name of the device.
     *   @return The name of the device.
     */
    virtual std::string Name() const = 0;

    /*!
     *   @brief  The type of device.
     *   @return The type of device.
     */
    virtual std::string Type() const = 0;

    /*!
     *   @brief Configure the device.
     *   @param[in]  pConfig   The instrument configuration used to configure the device.
     */
    virtual void Configure( IConfigurationSPtr pConfig ) = 0;

    /*!
     * @brief       Gets the connected state of the device.
     * @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                   false = Return locally cached value.
                                   true = Send request to h/w and wait for value (blocking).
     * @returns     True if the device is successfully connected to hardware, false if not.
     */
    virtual bool GetIsConnected( bool forceRefresh = true ) = 0;

    /*!
     * @brief       Was a connection to the device attempted?
     * @returns     True if an attempt was made to connect to the hardware, false if not.
     */
    virtual bool HaveAttemptedConnect() const = 0;
};

} //  namespace imp
