/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "EmulatedDriverBase.h"
#include "IDigitalInputDriver.h"
#include "IEmulatedDigitalInputDriver.h"

#include <memory>
#include <queue>
#include <string>


namespace imp
{

class IDriverFactoryRegistration;
using IDriverFactoryRegistrationSPtr = std::shared_ptr<IDriverFactoryRegistration>;

/*!
 *  @brief   Implementation of EmulatedDigitalInputDriver. Adheres to IDigitalInputDriver interface.
 */
class EmulatedDigitalInputDriver
    : public EmulatedDriverBase
    , public IDigitalInputDriver
    , public IEmulatedDigitalInputDriver
{
public:
    /*! @brief The specific type string of this driver. */
    static const std::string DriverType;

    /*!
     *   @brief Static method used to register this device driver with the registration container.
     *
     *   @param[in]  pDriverFactoryRegistration Driver factory registration instance.
     */
    static void Register( const IDriverFactoryRegistrationSPtr& pDriverFactoryRegistration );

    /*!
     *   @brief Static method used to register types associated with this driver.
     */
    static void RegisterTypes();

    /*!
     *   @brief      Constructor.
     *   @param[in]  name            Name of this driver instance
     */
    explicit EmulatedDigitalInputDriver( const std::string& name );

    ~EmulatedDigitalInputDriver() override;

    /*!
     *  @brief  Static method to provide the driver type string to be used in factories.
     *  @return Returns driver type string.
     */
    static std::string
    GetDriverType()
    {
        return DriverType;
    }

    // overrides from IDeviceDriver
    void Detach() override;

    // overrides from IDigitalInputDriver

    void GetInputRegister( std::function<void( DriverResponseType, uint8_t )> completionCb ) override;

    bool IsCachedInputSupported() const override;
    uint8_t GetCachedInputRegister() override;
    bool InputChangeNotificationSupported() const override;
    void SubscribeInputChangeNotification(
        InputRegisterCallback inputRegisterCb,
        std::function<void( DriverResponseType )> completionCb ) override;
    void UnsubscribeInputChangeNotification( std::function<void( DriverResponseType )> completionCb ) override;
    bool
    IsDigitalInputInitializeRequired() const override
    {
        return true;
    }

    // overrides from IEmulatedDigitalInputDriver

    void OnInputBitStateChange( DigitalIOBit bit, DigitalIOBitState state ) override;
    void AddToInputBitStateChangeList( DigitalIOBit bit, DigitalIOBitState state ) override;

private:
    /*!
     *   @brief Static method used to create an instance of this device driver. This method must be registered with
     *          the DriverFactory so it can be invoked when a new instance is required.
     *
     *   @param[in]  driverName            Name of the driver instance to create.
     *   @param[in]  pConfig               Instrument configuration file containing settings for the device driver.
     *   @returns A new instance of a concrete device driver on success.
     *   @throws A std::runtime_error on failure.
     */
    static IDeviceDriverSPtr Create( std::string const& driverName, IConfigurationSPtr pConfig );

    void ChangeInputBitState( DigitalIOBit bit, DigitalIOBitState state );
    void ClearInputCallback();

    uint8_t m_inputDataRegister;
    InputRegisterCallback m_inputRegisterCallback;
    using BitStatePair = std::pair<DigitalIOBit, DigitalIOBitState>;
    std::queue<BitStatePair> m_inputBitStateList;
    std::mutex m_inputBitStateListMutex;
};


} //  namespace imp
