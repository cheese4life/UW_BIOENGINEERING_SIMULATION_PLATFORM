/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDeviceDriver.h"
#include "IDispatcher.h"
#include "StandardDriverResponse.h"

namespace imp
{
/*!
 * @brief   Base implementation for all drivers on the instrument
 * @details All calls to the driver interface are executed on a shared driver thread.
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
class DeviceDriverBase : public virtual IDeviceDriver
{
public:
    /*!
     *   @brief Static method used to register types associated with this driver.
     */
    static void RegisterTypes();

    /*! @brief  Destructor. */
    virtual ~DeviceDriverBase() = default;

    // overrides from IDeviceDriver

    IDispatcherSPtr GetDriverDispatcher() override;

    void SetDriverDispatcher( IDispatcherSPtr pDriverDispatcher ) override;

    void Detach() override;

    std::string Name() const override;

    std::string Type() const override;

    bool
    IsEmulated() const override
    {
        return false;
    }

    void GetIsConnected( std::function<void( DriverResponseType, bool )> completionCb ) override;

protected:
    /*! @brief  Constructor. */
    explicit DeviceDriverBase( const std::string& name, const std::string& type );

    /*!
     * @brief   Invoke the completion callback on the shared driver thread.
     * @param[in] completionCb   Device callback to invoke.
     */
    void BeginInvokeCompletionCallback( IDispatcher::VoidFunctionPtr completionCb );

    /*!
     * @brief   Send a response from the driver to the device by invoking the callback on the
     *          driver's dispatcher.
     * @param[in] completionCb   Device callback to invoke.
     * @param[in] response       The response code to return (defaults to DriverSuccess).
     */
    void SendResponse(
        std::function<void( DriverResponseType )> completionCb,
        imp::StandardDriverResponse::Enum response = imp::StandardDriverResponse::DriverSuccess );

    /*!
     * @brief   Send a response from the driver to the device which includes a value (e.g. a setting)
     *          by invoking the callback on the driver's dispatcher.
     * @param[in] completionCb   Device callback to invoke.
     * @param[in] value          The value to return to the device.
     * @param[in] response       The response code to return (defaults to DriverSuccess).
     */
    template<typename TValue>
    void
    SendResponseWithValue(
        std::function<void( DriverResponseType, TValue )> completionCb,
        const TValue& value,
        imp::StandardDriverResponse::Enum response = imp::StandardDriverResponse::DriverSuccess )
    {
        BeginInvokeCompletionCallback( [ completionCb, value, response ]()
                                       { completionCb( imp::DriverResponseType( response ), value ); } );
    }

private:
    std::string m_name;
    std::string m_type;
    IDispatcherSPtr m_pSharedDriverDispatcher;
};

} //  namespace imp
