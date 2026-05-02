/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AsyncActivity.h"
#include "CachedValue.h"
#include "CallbackContainer.h"
#include "DeviceResponseType.h"
#include "DriverResponseType.h"
#include "ICCPDevice.h"
#include "IDeviceDriver.h"

namespace imp
{

class CCPDevice;

/*! @brief  Shorthand for creating shared pointers of CCPDevice. */
typedef std::shared_ptr<CCPDevice> CCPDeviceSPtr;

/*!
 *  @brief   Defines the base class for all CCP devices.
 */
class CCPDevice : public virtual ICCPDevice
{
public:
    /*!
     *   @brief Static method used to register types associated with this device.
     */
    static void RegisterTypes();

    explicit CCPDevice( const std::string& deviceName, const std::string& deviceType );

    ~CCPDevice() override = default;

    // ICCPDevice interface - see ICCPDevice for a description

    void Wait() override;

    void Wait( uint32_t timeout_ms ) override;

    void EStopWait() override;

    void EStopWait( uint32_t timeout_ms ) override;

    virtual SubscriptionId
    SubscribeCompletionCallback( const IDispatcherSPtr& pDispatcher, CompletionCallback callback ) override;

    virtual void UnsubscribeCompletionCallback( SubscriptionId subscriptionId ) override;

    void BeginPreventWait() override;

    void FinishPreventWait() override;

    bool IsActionInProgress() override;

    std::string
    Name() const override
    {
        return m_name;
    }

    std::string
    Type() const override
    {
        return m_type;
    }

    uint32_t
    DefaultTimeout() const override
    {
        return m_defaultTimeout_ms;
    }

    uint32_t
    MaxTimeout() const override
    {
        return m_maxTimeout_ms;
    }

    void
    Connect() override
    {
        // Do nothing by default
    }

    void Configure( IConfigurationSPtr pConfig ) override;

    bool
    GetIsConnected( bool /*forceRefresh = true*/ ) override
    {
        return false;
    }

    bool
    HaveAttemptedConnect() const override
    {
        return false;
    }

protected:
    void BeginAsyncActionActivity();
    void ExtendAsyncActionActivity();
    void FinishAsyncActionActivity(
        DriverResponseType response,
        std::function<void()> completionCb = nullptr,
        std::function<bool()> waitCompletePredicate = nullptr );
    void FinishExtendedAsyncActionActivity( DriverResponseType response );
    virtual void
    OnDeviceResponse( const DeviceResponseType& /*response*/ )
    {
        // Allow derived classes to take additional action based on a device response.
    }
    void FinishDeviceAsyncActionActivity(
        DeviceResponseType response,
        std::function<void()> completionCb = nullptr,
        std::function<bool()> waitCompletePredicate = nullptr );
    void BeginAsyncInterruptActionActivity();
    void FinishAsyncInterruptActionActivity(
        DriverResponseType response,
        std::function<void()> completionCb = nullptr,
        std::function<bool()> waitCompletePredicate = nullptr );
    void BeginAsyncEStopActionActivity();
    void FinishAsyncEStopActionActivity(
        DriverResponseType response,
        std::function<void()> completionCb = nullptr,
        std::function<bool()> waitCompletePredicate = nullptr );
    void ResetAllCachedActionResponses();

    bool IsActionResponseSuccessful() const;
    bool IsInterruptActionResponseSuccessful() const;
    bool IsEStopActionResponseSuccessful() const;

    DeviceResponseType GetCachedActionResponse() const;
    DeviceResponseType GetCachedInterruptActionResponse() const;
    DeviceResponseType GetCachedEStopActionResponse() const;

    void SetCachedActionResponse( DeviceResponseType response );
    void SetCachedInterruptActionResponse( DriverResponseType response );
    void SetCachedEStopActionResponse( DriverResponseType response );

    template<typename TPropertyType>
    friend class AsyncPropertyGet; // So it can use TranslateDriverResponse
    static DeviceResponseType TranslateDriverResponse( DriverResponseType response );

    /*!
     *  @brief   Basic initialization of the device, with an optionally-null function clearing cached values
     */
    void
    InitializeDevice( std::shared_ptr<IDeviceDriver> const& pDriver, std::function<void()> const& resetCachedValues );
    void EStopDevice( std::shared_ptr<IDeviceDriver> const& pDriver, bool bypassDispatcher = false );
    void ShutdownDevice( std::shared_ptr<IDeviceDriver> const& pDriver );

    /*! @brief Defines a function type which can be invoked on a driver to update a setting. */
    template<typename T, typename TDriver>
    using DriverSetFunction = void ( TDriver::* )( T, std::function<void( DriverResponseType )> );

    /*!
     * @brief        Template method which invokes a driver method to update a setting and handles
     *               the response from the driver. If the response is successful, the device's
     *               local cached value is updated. It's possible to chain multiple set methods
     *               into a single 'async activity'.
     *
     * @tparam TSetting                  The type of setting to update (e.g. int32_t).
     * @tparam TDriver                   The driver interface type (e.g. IAxisDriver).
     * @param [in] pDriver               The Driver to invoke the set function on.
     * @param [in] pDriverSetFunction    The function to invoke on the driver interface to perform the setting update.
     * @param [in] valueToSet            The new value of the setting.
     * @param [in] cachedValueToUpdate   The local cached setting value in the device to update if successful.
     * @param [in] beginAsyncActivity    If true (the default), this is the first call to SetDriverSettting and m_asyncActivity.Begin() will be called.
     * @param [in] nextSetFunction       The next set 'SetDriverSetting' function to invoke. A nullptr (the default) indicates no further set functions.
     */
    template<typename TSetting, typename TDriver>
    void
    SetDriverSetting(
        std::shared_ptr<TDriver> pDriver,
        DriverSetFunction<TSetting, TDriver> pDriverSetFunction,
        TSetting valueToSet,
        CachedValue<TSetting>& cachedValueToUpdate,
        bool beginAsyncActivity = true,
        std::function<void()> nextSetFunction = nullptr )
    {
        if ( beginAsyncActivity )
        {
            BeginAsyncActionActivity();
        }

        // Prepare a callback method. Executed in a different thread.
        auto callback = [ valueToSet, &cachedValueToUpdate, nextSetFunction, this ]( DriverResponseType const response )
        {
            // execute the next function in the chain if nextSetFunction is not nullptr and to continue
            // asynchronous activity
            FinishAsyncActionActivity(
                response,
                [ valueToSet, &cachedValueToUpdate, nextSetFunction, this ]()
                {
                    if ( IsActionResponseSuccessful() )
                    {
                        cachedValueToUpdate = valueToSet;

                        // Call the next 'Set' function in the chain.
                        if ( nextSetFunction )
                        {
                            nextSetFunction();
                        }
                    }
                },
                [ nextSetFunction, this ]() { return nextSetFunction == nullptr || !IsActionResponseSuccessful(); } );
        };

        // Dispatch call to driver
        pDriver->GetDriverDispatcher()->BeginInvoke( std::bind( pDriverSetFunction, pDriver, valueToSet, callback ) );
    }

    void Wait_Internal( uint32_t timeout_ms, bool acceptEStop );

    /*!
     *  @brief  Checks whether the cached value object is valid for use,
     *          i.e.: First update was received.
     *          Throws std::logic_error otherwise.
     *  @param[in]  obj A CachedValue object to check the validity.
     */
    template<class T>
    void
    AssertCachedValueValid( CachedValue<T> const& obj ) const
    {
        ASSERT_MSG( obj.IsValid(), "Severe error: Device " << Name() << " is not initialized" );
    }

    /*!
     * @brief Make device setting path by combining device section name and setting name.
     * @param [in]  settingName  Setting name
     * @return Device setting path
     */
    std::string DeviceSettingPath( const std::string& settingName ) const;

    std::mutex m_mutex;

private:
    friend class CCPDeviceAttorney;

    using CompletionCallbackContainer = CallbackContainer<DeviceResponseType>;
    AsyncActivity m_asyncActivity;
    CompletionCallbackContainer m_completionCallbacks;
    CachedValue<DeviceResponseType> m_cachedActionResponse;
    CachedValue<DeviceResponseType> m_cachedInterruptActionResponse;
    CachedValue<DeviceResponseType> m_cachedEStopActionResponse;
    const std::string m_name;
    const std::string m_type;
    uint32_t m_defaultTimeout_ms;
    uint32_t m_maxTimeout_ms;
};

} //  namespace imp
