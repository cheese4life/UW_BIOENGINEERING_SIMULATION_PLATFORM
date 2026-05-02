/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDigitalInputDevice.h"

#include "DigitalIODeviceBase.h"


namespace imp
{

// Forward declarations
class IDigitalInputDriver;
class IDeviceFactoryRegistration;
using IDeviceFactoryRegistrationSPtr = std::shared_ptr<IDeviceFactoryRegistration>;

/*! @brief The concrete implementation of an IMP Digital Input Device. */
class DigitalInputDevice
    : public virtual DigitalIODeviceBase
    , public virtual IDigitalInputDevice
{
public:
    /*!
     *   @brief Static method used to register this device with the registration container.
     *
     *   @param[in]  pDeviceFactoryRegistration Device factory registration instance.
     *   @throws std::runtime_error             This device type has already been registered.
     */
    static void Register( const IDeviceFactoryRegistrationSPtr& pDeviceFactoryRegistration );

    /*!
     *   @brief Static method used to register types associated with this device.
     */
    static void RegisterTypes();

    /*!
     * @brief Constructor.
     * @param   name      The name of the device.
     * @param   pDriver   The underlying driver.
     */
    DigitalInputDevice( std::string const& name, std::shared_ptr<IDigitalInputDriver> pDriver );

    /*!
     *  @brief  Static method to provide the device type string to be used in factories.
     *  @return Returns device type string.
     */
    static std::string GetDeviceType();

public: //  CCPDevice interface
    void Initialize() override;

public: //  IDigitalInputDevice interface
    DigitalIOBitState GetBitState( DigitalIOBit bit, bool forceRefresh = false ) override;
    uint8_t GetRegister( bool forceRefresh = false ) override;

    bool ChangeNotificationSupported() const override;
    void EnableChangeNotification() override;
    void DisableChangeNotification() override;
    bool ChangeNotificationEnabled() override;

private:
    /*!
     *   @brief Static method used to create an instance of this device. This method must be registered with
     *          the DeviceFactory so it can be invoked when a new instance is required.
     *
     *   @param[in]  deviceName            Name of the device instance to create.
     *   @param[in]  pDriver               The device driver associated with the device. It must actually be a
     *                                     IDigitalInputDriver and the function will downcast it to this type.
     *   @returns A new instance of a device on success.
     *   @throws std::runtime_error        The pDriver doesn't convert to an IDigitalInputDriver derived type.
     */
    static std::shared_ptr<DigitalInputDevice> Create( std::string const& deviceName, IDeviceDriverSPtr pDriver );

    std::shared_ptr<IDigitalInputDriver> m_pDriver;
    bool m_changeNotificationEnabled;
};

} //  namespace imp
