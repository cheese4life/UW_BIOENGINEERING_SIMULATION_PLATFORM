/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CCPDevice.h"
#include "CachedValue.h"
#include "DigitalIOBasicTypes.h"
#include "DigitalIOChangeSubscription.h"
#include "IDigitalIOPublisher.h"

#include <functional>
#include <memory>
#include <mutex>


namespace imp
{
// Forward declarations
class ICCPDevice;
class IConfiguration;
class IDigitalInputDriver;
class IDigitalOutputDriver;
class IDispatcher;


/*! @brief An abstract base class containing common logic for @link CCPDevice @endlink
 *         derived types that manipulate bits in a digital register.
 */
class DigitalIODeviceBase
    : public CCPDevice
    , public virtual IDigitalIOPublisher
{
public:
    using DriverGetRegisterFunction =
        std::function<void( std::function<void( DriverResponseType, uint8_t )> callback )>;
    using DriverGetCachedRegisterFunction = std::function<uint8_t()>;

    /*!
     * @brief  Constructor.
     * @param[in]  deviceName                Name of the device associated with the driver.
     * @param[in]  pDriver                   The driver to interact with.
     * @param[in]  deviceType                Type identifier for the device used by @link CCPDevice @endlink .
     * @param[in]  driverGetRegisterFn       Function that reads the driver's register and
     *                                       reports the result via callback.
     * @param[in]  driverGetCachedRegisterFn Function that synchronously reads the driver's register and
     *                                       returns the result when a cached read is performed by the device.
     *                                       This callback can be null if the driver doesn't support cached reads.
     */
    DigitalIODeviceBase(
        std::string const& deviceName,
        std::shared_ptr<IDeviceDriver> pDriver,
        std::string const& deviceType,
        DriverGetRegisterFunction driverGetRegisterFn,
        DriverGetCachedRegisterFunction driverGetCachedRegisterFn );


public: //  CCPDevice interface
    void Configure( std::shared_ptr<IConfiguration> pConfig ) override;

    void Initialize() override;
    void EStop() override;
    void Shutdown() override;

public: //  IDigitalIOPublisher interface
    SubscriptionId SubscribeChangeNotification( const DigitalIOChangeSubscription& subscription ) override;
    bool UnsubscribeChangeNotification( const SubscriptionId& subscriptionId ) override;

protected:
    DigitalIOBitState DoGetBitState( DigitalIOBit bit, bool forceRefresh );
    uint8_t DoGetRegister( bool forceRefresh );

    uint8_t GetCache() const;
    void SetCache( uint8_t registerValue, bool doPublish );

    void AssertChangeNotificationSupported() const;

    /*!
     *  @brief  Checks whether the cached value object is valid for use,
     *          ie: First update was received.
     *          Throws std::logic_error otherwise.
     *  @param[in]  obj A CachedValue object to check the validity.
     */
    template<class T>
    void
    AssertCachedValueValid( CachedValue<T> const& obj ) const
    {
        ASSERT_MSG( obj.IsValid(), "Severe error: Device " << Name() << " is not initialized" );
    }

    void ResetCachedValues();

private:
    std::shared_ptr<IDeviceDriver> m_pDriver;
    DriverGetRegisterFunction m_driverGetRegisterFn;
    DriverGetCachedRegisterFunction m_driverGetCachedRegisterFn;

    uint8_t m_polarity;

    typedef std::map<SubscriptionId, DigitalIOChangeSubscription> ChangeSubscriptionMap;
    typedef ChangeSubscriptionMap::value_type ChangeSubscriptionMapElement;
    typedef std::pair<ChangeSubscriptionMap::iterator, bool> ChangeSubscriptionMapInsertionIterator;

    ChangeSubscriptionMap m_changeSubscriptions;
    std::mutex m_setCacheMutex;

    //Locally cached values
    CachedValue<uint8_t> m_register;
};

} //  namespace imp
