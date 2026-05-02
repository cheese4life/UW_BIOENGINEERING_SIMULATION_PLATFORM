/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ActiveControlMode.h"
#include "CCPDevice.h"
#include "CachedValue.h"
#include "CallbackContainer.h"
#include "DiagnosticLog.h"
#include "IAxisDevice.h"
#include "IAxisDriver.h"
#include "LogExceptions.h"
#include "MotionControllerConfiguration.h"
#include "UserDefinedProfile.h"

#include <cstdint>


namespace imp
{

class IDeviceFactoryRegistration;
using IDeviceFactoryRegistrationSPtr = std::shared_ptr<IDeviceFactoryRegistration>;

class AxisDeviceAttorney;

/*!
 *  @brief   Provides the declaration of the Louganis Axis Device implementation.
 */
class AxisDevice
    : public CCPDevice
    , public IAxisDevice
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

    /*! @brief  Constructor. */
    explicit AxisDevice( std::string const& name, IAxisDriverSPtr pDriver );

    /*!
     *  @brief  Static method to provide the device type string to be used in factories.
     *  @return Returns device type string.
     */
    static std::string GetDeviceType();

public: //  CCPDevice interface
    void Configure( IConfigurationSPtr pConfig ) override;

    double GetActualPosition( bool forceRefresh = true ) override;
    double GetCommandedPosition( bool forceRefresh = true ) override;
    double GetPositionError( bool forceRefresh = true ) override;

    void SetVelocity( double velocity ) override;
    void SetAcceleration( double acceleration ) override;
    void SetJerk( double jerk ) override;
    void SetDeceleration( double deceleration ) override;
    double GetVelocity( bool forceRefresh = true ) override;
    double GetAcceleration( bool forceRefresh = true ) override;
    double GetJerk( bool forceRefresh = true ) override;
    double GetDeceleration( bool forceRefresh = true ) override;
    ActiveControlMode GetActiveControlMode() override;

    void MoveAbsolute( double position ) override;
    void MoveRelative( double distance ) override;

    void NMoveAbsolute( const std::string& position_name ) override;
    void NMoveRelative( const std::string& distance_name ) override;

    void NMoveOffset( const std::string& position_name, const std::string& distance_name ) override;

    void MoveRelativeToFlag( double maximumDistance, const std::string& flagName ) override;
    void NMoveRelativeToFlag( const std::string& maximumDistanceName, const std::string& flagName ) override;

    void MoveRelativeToFlagOpenLoop( const std::string& flagName, double offsetFromFlag ) override;
    void
    NMoveRelativeToFlagOpenLoop( const std::string& flagName, const std::string& offsetFromFlagDistanceName ) override;

    bool GetFlagStatus( const std::string& flagName, bool forceRefresh = true ) override;
    void ResetPosition() override;
    void ResetPosition( double position ) override;
    void NResetPosition( const std::string& positionName ) override;

    void Stop() override;

    void Connect() override;
    void Initialize() override;
    void EStop() override;
    void Shutdown() override;

    NamedPositionsMap GetNamedPositions() const override;
    void SetNamedPositions( const NamedPositionsMap& namedPositions, bool doWriteToLocalConfig = true ) override;
    NamedDistancesMap GetNamedDistances() const override;
    void SetNamedDistances( const NamedDistancesMap& namedDistances, bool doWriteToLocalConfig = true ) override;
    NamedFlagsMap GetNamedFlags() const override;
    void
    SetNamedMoveProfiles( const NamedMoveProfilesMap& namedMoveProfiles, bool doWriteToLocalConfig = true ) override;
    NamedMoveProfilesMap GetNamedMoveProfiles() const override;
    void SelectMoveProfile( const std::string& profileName ) override;

    void Enable() override;
    void Disable() override;
    bool GetIsEnabled( bool forceRefresh = true ) override;

    void SetExternalControl( double moveDistancePerPulse ) override;
    double GetExternalControl( bool forceRefresh = true ) override;

    void EnableTraceCapture(
        TraceTrigger trigger,
        uint16_t period,
        const std::vector<TraceVariable>& variables,
        uint64_t waitMotionStabilizeUs = 0 ) override;
    void NEnableTraceCapture( TraceTrigger trigger, const std::string& traceProfile ) override;
    void StopTraceCapture() override;
    bool GetIsTraceCaptureEnabled( bool forceRefresh = true ) override;
    void SaveTraceCapture( const std::string& pathname, std::int32_t traceCount = 0 ) override;

    void EnableTriggerOnPosition(
        double startPosition,
        double endPosition,
        double increment,
        bool isBidirectional,
        int pulseWidthNs ) override;
    void NEnableTriggerOnPosition(
        const std::string& startPositionName,
        const std::string& endPositionName,
        const std::string& namedDistance,
        bool isBidirectional,
        int pulseWidthNs ) override;
    void DisableTriggerOnPosition() override;
    bool GetIsTriggerOnPositionEnabled( bool forceRefresh = true ) override;

    SubscriptionId SubscribeToMovementStarted(
        const std::shared_ptr<IDispatcher>& pDispatcher,
        MovementStartedCallback callback ) override;
    bool UnsubscribeFromMovementStarted( SubscriptionId subscriptionId ) override;

    void SetIdleErrorCallback( std::function<void( DeviceResponseType )> axisErrorCb ) override;

    void SetDigitalOutputTrigger( DigitalOutputTrigger trigger ) override;
    DigitalOutputTrigger GetDigitalOutputTrigger( bool forceRefresh = true ) override;

    double GetMotorCurrent( bool forceRefresh = true ) override;
    AxisRecoveryOperation GetRecoveryOperation( bool forceRefresh = true ) override;

    IMotionControllerConfiguration& GetMotionControllerConfiguration() override;
    IUserDefinedProfile& GetUserDefinedProfile() override;
    void MoveRelativeUDP( uint32_t repeat ) override;
    void MoveContinuous( double position, double acceleration, double velocity, double deceleration ) override;

    bool GetIsConnected( bool forceRefresh = true ) override;
    bool HaveAttemptedConnect() const override;

    void EnterHomingMode() override;
    void ExitHomingMode() override;

protected:
    void OnDeviceResponse( const DeviceResponseType& response ) override;

private:
    friend class MotionControllerConfiguration;
    friend class UserDefinedProfile;
    /*!
     *   @brief Static method used to create an instance of this device. This method must be registered with
     *          the DeviceFactory so it can be invoked when a new instance is required.
     *
     *   @param[in]  deviceName            Name of the device instance to create.
     *   @param[in]  pDriver               The device driver associated with the device. It must actually be a
     *                                     IAxisDriver and the function will downcast it to this type.
     *   @returns A new instance of a device on success.
     *   @throws std::runtime_error        The pDriver doesn't convert to an IAxisDriver derived type.
     */
    static std::shared_ptr<AxisDevice> Create( std::string const& deviceName, IDeviceDriverSPtr pDriver );

    void GetFlagStatuses( bool forceRefresh = true );

    static void RegisterAxisDriverResponseTranslations();
    void RegisterAndHandleIdleErrors();
    void ClearPositionErrorAlert();
    void ClearAxisAlerts();

    void SetActiveControlMode( ActiveControlMode mode );
    void CheckActiveControlMode();

    void ParseNamedDistances( const IConfigurationSPtr& pConfig );
    void ParseNamedPositions( const IConfigurationSPtr& pConfig );
    void ParseNamedFlags( const IConfigurationSPtr& pConfig );
    void ParseNamedMoveProfiles( const IConfigurationSPtr& pConfig );
    UDPProfileSPtr ParseUDPProfile( const IConfigurationSPtr& pConfig, const std::string& profileName );
    RMEProfileSPtr ParseRMEProfile( const IConfigurationSPtr& pConfig, const std::string& profileName );
    void ParseRMECompensationData(
        const IConfigurationSPtr& pConfig,
        const std::string& compensationDataSectionName,
        std::vector<int32_t>& compensationData );
    void CheckConfigureCalled( const std::string& settingsNameBeingUpdated );
    void TrimTraceVariables( const std::vector<TraceVariable>& inVars, std::vector<TraceVariable>& outVars );
    void SelectUDPMoveProfile( UDPProfileSPtr pUDPProfile );

    template<typename T>
    T
    GetNamedValue( const NamedValuesMap<T>& namedValuesMap, const std::string& name ) const
    {
        auto iter = namedValuesMap.find( name );

        if ( iter == namedValuesMap.end() )
        {
            std::ostringstream oss;
            oss << "Could not find named value '" << name << "'!";
            THROW_RUNTIME_ERROR( oss.str() );
        }

        return iter->second;
    }

    void PublishMovementStarted( double positionStart, double positionEnd );

    friend class AxisDeviceAttorney;

    MotionControllerConfiguration m_motionControllerConfiguration;
    UserDefinedProfile m_userDefinedProfile;
    IAxisDriverSPtr m_pAxisDriver;
    NamedDistancesMap m_namedDistanceMap;
    NamedPositionsMap m_namedPositionMap;
    NamedFlagsMap m_namedFlagMap;

    /*! @brief A map of named RME profiles. */
    using NamedRMEProfilesMap = std::map<std::string, RMEProfileSPtr>;

    /*! @brief A map of named UDP profiles. */
    using NamedUDPProfilesMap = std::map<std::string, UDPProfileSPtr>;

    enum class ProfileType
    {
        NUL,
        MOVE,
        RME,
        UDP,
    };

    ProfileType GetMoveProfileType( const std::string& profileName ) const;

    NamedUDPProfilesMap m_namedUDPProfilesMap;
    NamedMoveProfilesMap m_namedMoveProfilesMap;
    NamedRMEProfilesMap m_namedRMEProfilesMap;

    CallbackContainer<Movement> m_movementStartedSubscriptions;
    IConfigurationSPtr m_pConfig;

    std::function<void( DeviceResponseType )> m_axisErrorCb;
    std::vector<uint16_t> m_activeAlerts;
    uint16_t m_positionErrorAlert;
    bool m_haveAttemptedConnect;
    ActiveControlMode m_activeControlMode;

    struct HomingModeInfo
    {
        bool isInHomingMode;
        MotionCompleteMode originalMotionCompleteMode;
    };

    HomingModeInfo m_homingModeInfo;

    void ResetCachedValues();

private:
    //Locally cached values
    CachedValue<double> m_cachedActualPosition;
    CachedValue<double> m_cachedCommandedPosition;
    CachedValue<double> m_cachedPositionError;
    CachedValue<uint8_t> m_cachedDeviceState;
    CachedValue<double> m_cachedVelocity;
    CachedValue<double> m_cachedAcceleration;
    CachedValue<double> m_cachedJerk;
    CachedValue<double> m_cachedDeceleration;
    CachedValue<uint32_t> m_cachedFlagStatuses;
    CachedValue<bool> m_cachedIsEnabled;
    CachedValue<double> m_cachedMoveDistancePerPulse;
    CachedValue<bool> m_cachedIsTraceCaptureEnabled;
    CachedValue<bool> m_cachedIsTriggerOnPositionEnabled;
    CachedValue<DigitalOutputTrigger> m_cachedDigitalOutputTrigger;
    CachedValue<double> m_motorCurrent;
    CachedValue<RMEProfileSPtr> m_cachedRMEProfile;
    CachedValue<UDPProfileSPtr> m_cachedUDPProfile;
    CachedValue<bool> m_cachedIsConnected;
};

} //  namespace imp
