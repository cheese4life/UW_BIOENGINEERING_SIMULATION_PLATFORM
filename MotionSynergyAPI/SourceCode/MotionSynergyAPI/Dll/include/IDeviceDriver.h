/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "DriverResponseType.h"
#include "IDriver.h"
#include <functional>
#include <memory>


namespace imp
{

class IConfiguration;
class IDispatcher;
class IDeviceDriver;

using IConfigurationSPtr = std::shared_ptr<IConfiguration>;
using IDispatcherSPtr = std::shared_ptr<IDispatcher>;
using IDeviceDriverSPtr = std::shared_ptr<IDeviceDriver>;

/*!
 * @brief   Interface all device drivers must conform to. Concrete driver implementations
 *          should not implement this interface directly. Instead they should inherit from
 *          DeviceDriverBase or AsyncDeviceDriverBase as appropriate.
 * @details All calls to the driver interface (except interrupt actions such as EStop and Stop)
 *          are executed on a shared driver thread.
 *          Therefore all calls must execute within a few milliseconds to ensure they
 *          don't block other drivers.
 *          Operations that take more than a few milliseconds must be executed asynchronously.
 *          These operations must be passed a completion callback which can be invoked once
 *          the operation completes. This completion callback should also be invoked on the
 *          shared driver thread.
 *          Drivers which need to block for extended periods of time need to create their own
 *          private thread and associated dispatcher. The AsyncDeviceDriverBase class provides
 *          this functionality.
 */
class IDeviceDriver : public virtual IDriver
{
public:
    /*! @brief  Constructor. */
    IDeviceDriver() = default;
    /*! @brief  Destructor. */
    virtual ~IDeviceDriver() = default;

    /*!
     *  @brief Get the dispatcher of the driver thread.
     *         All device driver calls will be run using this dispatcher.
     *         Drivers should also invoke the completion callback on this thread.
     */
    virtual IDispatcherSPtr GetDriverDispatcher() = 0;

    /*!
     *  @brief Set the dispatcher of the driver thread.
     *         All device driver calls will be run using this dispatcher.
     *         This method is called when the instance is first registered with the LouganisDriverManager.
     *  @param[in]  pDriverDispatcher    The dispatcher associated with the driver thread.
     */
    virtual void SetDriverDispatcher( IDispatcherSPtr pDriverDispatcher ) = 0;

    /*!
     *  @brief      Configure the driver.
     *  @details    This method is executed synchronously on the device thread (NOT the driver thread)
     *              and does not have a maximum duration for its execution.
     *  @param[in]  pConfig    The configuration file containing the driver settings.
     */
    virtual void Configure( IConfigurationSPtr pConfig ) = 0;

    /*!
     *  @brief      Establish connection through the driver.
     *  @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void Connect( std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *  @brief      Initialize the driver.
     *  @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void Initialize( std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *  @brief      Shutdown the driver.
     *  @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void Shutdown( std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *  @brief      EStop the driver. Do not dispatch this call onto the driver thread.
     *  @details    Perform an emergency stop. If there is an operation currently executing, it
     *              must be interrupted immediately.
     *  @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void EStop( std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *  @brief      Detach any remaining callbacks registered by the associated device.
     *  @details    Called just prior to the associated device being deleted. The
     *              driver should ensure any remaining callbacks are deregistered so there
     *              is no possibility they are invoked after the device has been
     *              deleted. Commonly used by drivers which support streaming callbacks.
     *              Note this method is called synchronously.
     *              The driver must not send any messages over a connection during
     *              this call, as the connection has almost certainly been closed by this
     *              stage.
     */
    virtual void Detach() = 0;

    /*!
     *  @brief      Gets the connected state of the device.
     *  @param[in]  completionCb    Function to return success/failure and value when call is complete.
     */
    virtual void GetIsConnected( std::function<void( DriverResponseType, bool )> completionCb ) = 0;
};

} //  namespace imp
