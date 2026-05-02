/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "DeviceDriverBase.h"
#include "EnumClass.h"
#include "IEmulatedDriver.h"
#include "ITimer.h"
#include "StandardDriverResponse.h"
#include "TimerFactory.h"

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>


namespace imp
{

/*!
 *  @brief   Support a basic device driver state machine.
 *  @details The implementation should be tidied up as part of the JIRA ticket below.
 *           See http://jira.invetech.com.au/browse/REUSE-368 for details.
 */
enum class EmulatedDriverState : int8_t
{
    Uninitialized,
    Connected,
    Idle,
    Busy,
    Error,
    Shutdown,
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( EmulatedDriverState )


/*!
 *  @brief   Base class for all emulated drivers
 *  @details Basic emulation strategy:
 *           - Have an atomic busy flag get/set for timers
 *           - check the busy flag on entry (and set)
 *           - if not okay, sendresponse (fail busy)
 *           - if okay, set a timer to call []()endaction (clears busy flag), sendresponse (success)
 *           .
 *           A short time exists for setting properties and for getting properties
 *             (note that this time includes simulated overhead for comms on real hardware)
 */
class EmulatedDriverBase
    : public DeviceDriverBase
    , public virtual IEmulatedDriver
{
protected:
    /*!
     *   @brief      Constructor.
     *   @param[in]  name                Name of this driver instance
     *   @param[in]  emulatedDriverType  The type of emulated driver
     */
    EmulatedDriverBase( const std::string& name, const std::string& emulatedDriverType );

public:
    virtual ~EmulatedDriverBase() = default;

    // overrides from IDeviceDriver

    void Configure( IConfigurationSPtr pConfig ) override;
    void Connect( std::function<void( DriverResponseType )> completionCb ) override;
    void Initialize( std::function<void( DriverResponseType )> completionCb ) override;
    void Shutdown( std::function<void( DriverResponseType )> completionCb ) override;
    void EStop( std::function<void( DriverResponseType )> completionCb ) override;

    bool
    IsEmulated() const override
    {
        return true;
    }

    void GetIsConnected( std::function<void( DriverResponseType, bool )> completionCb ) override;

    // overrides from IEmulatedDriver
    void SetResponse( DriverResponseType response ) override;
    void InsertAtHeadOfResponseList( DriverResponseType response ) override;
    void AddToResponseList( DriverResponseType response, uint32_t count ) override;
    void SetResponseListLooping( bool isLoopingEnabled ) override;
    bool GetResponseListLooping() const override;

protected:
    /*!
     * @brief   Called during Initialize(). Derived classes can override this function to perform
     *          driver-specific behaviour during Initialize().
     */
    virtual void
    OnInitialize()
    {
        // Do nothing
    }

    /*!
     * @brief   Send a response from the driver to the device which includes a value (e.g. a setting),
     *          but first waiting for a specified period of time to simulate communication latency with
     *          a real device.
     * @tparam    TSetting       The type of the setting.
     * @param[in] completionCb   Device callback to invoke.
     * @param[in] value          The value to return to the device.
     * @param[in] duration       The duration of the communication latency to emulate. (defaults to 20ms).
     */
    template<typename TSetting>
    void
    GetSetting(
        std::function<void( DriverResponseType, TSetting )> completionCb,
        TSetting value,
        std::chrono::milliseconds duration = std::chrono::milliseconds( 20 ) )
    {
        if ( !GetSettingStart() )
        {
            SendResponseWithValue( completionCb, TSetting{}, StandardDriverResponse::DriverBusy );
            return;
        }

        m_pGetSettingTimerCallback = [ this, completionCb, value ]()
        {
            GetSettingEnd();
            completionCb( GetResponse(), value );
        };
        m_pGetSettingTimer->Set(
            duration,
            [ & ]() { RunGetSettingTimerCallback(); },
            GetDriverDispatcher() );
    }

    /*!
     * @brief   Send a response from the driver to the device which includes a value (e.g. a setting),
     *          but first waiting for a specified period of time to simulate communication latency with
     *          a real device. This method supports running multiple requests in parallel, useful
     *          when emulating devices that allow multiple property gets to be in progress at any one time.
     * @tparam    TSetting       The type of the setting.
     * @param[in] completionCb   Device callback to invoke.
     * @param[in] value          The value to return to the device.
     * @param[in] duration       The duration of the communication latency to emulate. (defaults to 20ms).
     */
    template<typename TSetting>
    void
    GetSettingInParallel(
        std::function<void( DriverResponseType, TSetting )> completionCb,
        TSetting value,
        std::chrono::milliseconds duration = std::chrono::milliseconds( 20 ) )
    {

        auto pTimer = TimerFactory::Create();

        pTimer->Set(
            duration,
            [ this, pTimer, completionCb, value ]()
            {
                ASSERT(
                    !pTimer
                         ->IsActive() ); // Capturing pTimer ensures the timer instance remains allocated until the callback is executed.
                completionCb( GetResponse(), value );
            },
            GetDriverDispatcher() );
    }

    /*!
     * @brief   Set a value in the emulated driver (e.g. a setting),
     *          waiting for a specified period of time to simulate communication latency with
     *          a real device and then sending a response to the device.
     * @tparam    TSetting       The type of the setting.
     * @param[in] completionCb   Device callback to invoke.
     * @param[in] localSetting   The local setting in this emulated driver to update.
     * @param[in] newValue       The new value to update the setting to.
     * @param[in] duration       The duration of the communication latency to emulate. (defaults to 20ms).
     */
    template<typename TSetting>
    void
    SetSetting(
        std::function<void( DriverResponseType )> completionCb,
        TSetting& localSetting,
        TSetting newValue,
        std::chrono::milliseconds duration = std::chrono::milliseconds( 20 ) )
    {
        if ( !ActionStart() )
        {
            SendResponse( completionCb, StandardDriverResponse::DriverBusy );
            return;
        }

        localSetting = newValue;
        m_pTimerCallback = [ this, completionCb ]()
        {
            ActionEnd();
            completionCb( GetResponse() );
        };
        m_pTimer->Set(
            duration,
            [ & ]() { RunTimerCallback(); },
            GetDriverDispatcher() );
    }

    /*!
     * @brief   Execute a command on an emulated driver by
     *          waiting for a specified period of time to simulate the action execution time (including
     *          communications latency) and then sending a response to the device.
     * @param[in] completionCb   Device callback to invoke.
     * @param[in] command        A function to execute in the emulated driver (which could update local state)
     * @param[in] duration       The duration of the command being emulated. (defaults to 100ms).
     */
    void
    ExecuteCommand(
        std::function<void( DriverResponseType )> completionCb,
        std::function<void()> command,
        std::chrono::milliseconds duration = std::chrono::milliseconds( 100 ) )
    {
        if ( !ActionStart() )
        {
            SendResponse( completionCb, StandardDriverResponse::DriverBusy );
            return;
        }

        m_pTimerCallback = [ this, completionCb, command ]()
        {
            command();
            ActionEnd();
            completionCb( GetResponse() );
        };
        m_pTimer->Set(
            duration,
            [ & ]() { RunTimerCallback(); },
            GetDriverDispatcher() );
    }

    /*!
     * @brief   Execute a 'no-op' command on an emulated driver by
     *          waiting for a specified period of time to simulate the action execution time (including
     *          communications latency) and then sending a response to the device.
     * @param[in] completionCb   Device callback to invoke.
     * @param[in] duration       The duration of the command being emulated. (defaults to 100ms).
     */
    void
    ExecuteNOPCommand(
        std::function<void( DriverResponseType )> completionCb,
        std::chrono::milliseconds duration = std::chrono::milliseconds( 100 ) )
    {
        if ( !ActionStart() )
        {
            SendResponse( completionCb, StandardDriverResponse::DriverBusy );
            return;
        }

        m_pTimerCallback = [ this, completionCb ]()
        {
            ActionEnd();
            completionCb( GetResponse() );
        };
        m_pTimer->Set(
            duration,
            [ & ]() { RunTimerCallback(); },
            GetDriverDispatcher() );
    }

    /*!
     * @brief   Execute a command on an emulated driver by
     *          waiting for a specified period of time to simulate the action execution time (including
     *          communications latency) and then sending a response to the device.
     *          This method allows a specific command response code to be returned.
     * @param[in] completionCb   Device callback to invoke.
     * @param[in] command        A function to execute in the emulated driver (which could update local state)
     * @param[in] response       The response code to return to the device on completion of the command.
     * @param[in] duration       The duration of the command being emulated. (defaults to 100ms).
     */
    void
    ExecuteCommandWithSpecifiedResponse(
        std::function<void( DriverResponseType )> completionCb,
        std::function<void()> command,
        StandardDriverResponse::Enum response,
        std::chrono::milliseconds duration = std::chrono::milliseconds( 100 ) )
    {
        if ( !ActionStart() )
        {
            SendResponse( completionCb, StandardDriverResponse::DriverBusy );
            return;
        }

        m_pTimerCallback = [ this, completionCb, command, response ]()
        {
            command();
            ActionEnd();
            completionCb( imp::DriverResponseType( response ) );
        };
        m_pTimer->Set(
            duration,
            [ & ]() { RunTimerCallback(); },
            GetDriverDispatcher() );
    }

    /*!
     * @brief    Immediately dispatch the timer callback function
     * @details  This function is for interrupting a timer so that the callback can be executed before the
     *           timer expires. For example, if a movement is to be interrupted before completion, calling
     *           this function will execute the callback code and cancels the timer.
     * @param[in] isDueToEStop Is the timed callback being dispatched immediately due to an EStop? If so a
     *                         StandardDriverResponse::DriverEStop response will be returned to the request
     *                         in progress.
     */
    void
    ImmediatelyDispatchTimedCallback( bool isDueToEStop = false )
    {
        BeginInvokeCompletionCallback( [ this, isDueToEStop ]() { RunTimerCallback( true, isDueToEStop ); } );
    }

    /*!
     * @brief    Start a timer which delays for the specified period before executing the specified function
     *           on the dispatcher thread.
     * @param[in] delay Period to delay until the function is executed.
     * @param[in] func  Function to execute after the delay.
     */
    void ExecuteFunctionAfterDelay( std::chrono::milliseconds delay, std::function<void()> func );

    /*!
     * @brief    Cancel the timer started by ExecuteFunctionAfterDelay() so the function is not called.
     */
    void CancelTimerFunction();

    /*!
     * @brief    Cancel the existing command in progress and do not send a response.
     */
    void CancelExistingCommand();

    EmulatedDriverState
    GetEmulatedDriverState() const
    {
        return m_driverState;
    }

private:
    void
    RunTimerCallback( bool cancelTimer = false, bool isDueToEStop = false )
    {
        if ( m_pTimerCallback != nullptr )
        {
            if ( isDueToEStop )
            {
                InsertAtHeadOfResponseList( StandardDriverResponse::DriverEStop );
            }

            m_pTimerCallback();
            m_pTimerCallback = nullptr;
        }

        if ( cancelTimer && m_pTimer->IsActive() )
        {
            m_pTimer->Cancel();
        }
    }

    void
    RunGetSettingTimerCallback()
    {
        if ( m_pGetSettingTimer != nullptr )
        {
            auto callback = m_pGetSettingTimerCallback;
            m_pGetSettingTimerCallback = nullptr;
            callback();
        }
    }

private:
    bool ActionStart();
    void ActionEnd();
    bool GetSettingStart();
    void GetSettingEnd();
    DriverResponseType GetResponse();

    std::string m_name;
    std::string m_type;
    EmulatedDriverState m_driverState = EmulatedDriverState::Uninitialized;
    DriverResponseType m_responseCode;
    using ResponseCodeList = std::vector<DriverResponseType>;
    ResponseCodeList m_responseCodeList;
    bool m_isResponseListLoopingEnabled = false;
    mutable std::mutex m_responseMutex;
    std::atomic<bool> m_busy;
    std::atomic<bool> m_getSettingBusy;
    std::shared_ptr<ITimer> m_pTimer;
    TimerExpiryCallback m_pTimerCallback;
    std::shared_ptr<ITimer> m_pGetSettingTimer;
    TimerExpiryCallback m_pGetSettingTimerCallback;
};

} //  namespace imp
