/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "DigitalOutputTrigger.h"
#include "EmulatedDriverBase.h"
#include "IAxisDriver.h"
#include "IEmulatedAxisDriver.h"
#include "Stopwatch.h"

#include <memory>
#include <string>

namespace imp
{

class IDriverFactoryRegistration;
using IDriverFactoryRegistrationSPtr = std::shared_ptr<IDriverFactoryRegistration>;

class EmulatedAxisDriverAttorney;

/*!
 *  @brief   Defines emulated axis driver to support operation and testing without a real axis.
 *  @details Basic emulation strategy:
 *           Movement calculates the time for ramping up and down to full velocity, ignoring jerk
 *             since the trinamics that this emulates also ignores jerk.
 *
 */
class EmulatedAxisDriver
    : public EmulatedDriverBase
    , public IAxisDriver
    , public IEmulatedAxisDriver
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
     */
    explicit EmulatedAxisDriver( const std::string& name );

    /*!
     *   @brief      Virtual destructor.
     */
    ~EmulatedAxisDriver() override;

    //  IDeviceDriver interface
    void Configure( IConfigurationSPtr pConfig ) override;


    //  IAxisDriver interface

    /*!
     *  @brief  Static method to provide the driver type string to be used in factories.
     *  @return Returns driver type string.
     */
    static std::string
    GetDriverType()
    {
        return DriverType;
    }

    void GetActualPosition( std::function<void( DriverResponseType, double )> completionCb ) override;

    void GetCommandedPosition( std::function<void( DriverResponseType, double )> completionCb ) override;

    void GetPositionError( std::function<void( DriverResponseType, double )> completionCb ) override;

    void SetVelocity( double velocity, std::function<void( DriverResponseType )> completionCb ) override;

    void SetAcceleration( double acceleration, std::function<void( DriverResponseType )> completionCb ) override;

    void SetJerk( double jerk, std::function<void( DriverResponseType )> completionCb ) override;

    void SetDeceleration( double deceleration, std::function<void( DriverResponseType )> completionCb ) override;

    void GetVelocity( std::function<void( DriverResponseType, double )> completionCb ) override;

    void GetAcceleration( std::function<void( DriverResponseType, double )> completionCb ) override;

    void GetJerk( std::function<void( DriverResponseType, double )> completionCb ) override;

    void GetDeceleration( std::function<void( DriverResponseType, double )> completionCb ) override;

    void MoveAbsolute( double position, std::function<void( DriverResponseType )> completionCb ) override;

    void MoveRelativeUDP( uint32_t position, std::function<void( DriverResponseType )> completionCb ) override;

    void MoveRelative( double distance, std::function<void( DriverResponseType )> completionCb ) override;

    void MoveRelativeToFlag(
        double maximumDistance,
        uint8_t flagIndex,
        std::function<void( DriverResponseType )> completionCb ) override;

    void MoveRelativeToFlagOpenLoop(
        uint8_t flagIndex,
        double offsetFromFlag,
        std::function<void( DriverResponseType )> completionCb ) override;

    void MoveContinuous(
        double position,
        double acceleration,
        double velocity,
        double deceleration,
        std::function<void( DriverResponseType )> completionCb ) override;


    void GetFlagStatuses( std::function<void( DriverResponseType, uint32_t )> completionCb ) override;

    void ResetToActualPosition( std::function<void( DriverResponseType, double )> completionCb ) override;

    void ResetPosition( double rawPosition, std::function<void( DriverResponseType )> completionCb ) override;

    void Stop( std::function<void( DriverResponseType )> completionCb ) override;

    void Enable( std::function<void( DriverResponseType )> completionCb ) override;

    void Disable( std::function<void( DriverResponseType )> completionCb ) override;

    void GetIsEnabled( std::function<void( DriverResponseType, bool )> completionCb ) override;

    void
    SetExternalControl( double moveDistancePerPulse, std::function<void( DriverResponseType )> completionCb ) override;

    void GetExternalControl( std::function<void( DriverResponseType, double )> completionCb ) override;

    void EnableTraceCapture(
        TraceTrigger trigger,
        uint16_t period,
        const std::vector<TraceVariable>& variables,
        uint64_t waitMotionStabilizeUs,
        std::function<void( DriverResponseType )> completionCb ) override;

    void StopTraceCapture( std::function<void( DriverResponseType )> completionCb ) override;

    void GetIsTraceCaptureEnabled( std::function<void( DriverResponseType, bool )> completionCb ) override;

    void SaveTraceCapture(
        const std::string& pathname,
        std::int32_t traceCount,
        std::function<void( DriverResponseType )> completionCb ) override;

    void SetIdleErrorCallback( std::function<void( DriverResponseType )> axisErrorCb ) override;

    void EnableTriggerOnPosition(
        double startPosition,
        double endPosition,
        double increment,
        bool isBidirectional,
        int pulseWidthNs,
        std::function<void( DriverResponseType )> completionCb ) override;

    void DisableTriggerOnPosition( std::function<void( DriverResponseType )> completionCb ) override;

    void GetIsTriggerOnPositionEnabled( std::function<void( DriverResponseType, bool )> completionCb ) override;

    void SetDigitalOutputTrigger( DigitalOutputTrigger trigger, std::function<void( DriverResponseType )> completionCb )
        override;
    void
    GetDigitalOutputTrigger( std::function<void( DriverResponseType, DigitalOutputTrigger )> completionCb ) override;

    void GetMotorCurrent( std::function<void( DriverResponseType, double )> completionCb ) override;

    void SetUDPProfile( UDPProfileSPtr pProfile, std::function<void( DriverResponseType )> completionCb ) override;
    void SetRMEProfile( RMEProfileSPtr pProfile, std::function<void( DriverResponseType )> completionCb ) override;

    void GetMotionTracking( std::function<void( DriverResponseType, MotionTrackingSettings )> completionCb ) override;
    void SetMotionTracking(
        MotionTrackingSettings motionTrackingSettings,
        std::function<void( DriverResponseType )> completionCb ) override;
    void GetMotionError( std::function<void( DriverResponseType, MotionErrorSettings )> completionCb ) override;
    void SetMotionError(
        MotionErrorSettings motionErrorSettings,
        std::function<void( DriverResponseType )> completionCb ) override;
    void GetPositionLoop( std::function<void( DriverResponseType, PositionLoopSettings )> completionCb ) override;
    void SetPositionLoop(
        PositionLoopSettings positionLoopSettings,
        std::function<void( DriverResponseType )> completionCb ) override;
    void GetCurrentLoop( std::function<void( DriverResponseType, CurrentLoopSettings )> completionCb ) override;
    void SetCurrentLoop(
        CurrentLoopSettings currentLoopSettings,
        std::function<void( DriverResponseType )> completionCb ) override;
    void GetBiquadFilters( std::function<void( DriverResponseType, BiquadFilterSettings )> completionCb ) override;
    void SetBiquadFilters(
        BiquadFilterSettings biquadFilterSettings,
        std::function<void( DriverResponseType )> completionCb ) override;
    void GetCommunication( std::function<void( DriverResponseType, CommunicationSettings )> completionCb ) override;
    void SetCommunication(
        CommunicationSettings communicationSettings,
        std::function<void( DriverResponseType )> completionCb ) override;
    void WriteToNVRAM(
        std::vector<MotionControllerConfigurationType> configurationTypes,
        std::function<void( DriverResponseType )> completionCb ) override;
    void RestoreMotionControllerConfiguration(
        std::string pathname,
        std::function<void( DriverResponseType )> completionCb ) override;

    void SaveMotionControllerConfiguration(
        std::string pathname,
        std::function<void( DriverResponseType )> completionCb ) override;

    void RestoreMotionControllerConfigurationToNVRAM(
        std::string pathname,
        std::function<void( DriverResponseType )> completionCb ) override;

    void GetFirmwareVersion( std::function<void( DriverResponseType, FirmwareVersionInfo )> completionCb ) override;
    void GetPhaseAngle( std::function<void( DriverResponseType, PhaseAngleInfo )> completionCb ) override;
    void
    SetPhaseAngle( PhaseAngleInfo phaseAngleInfo, std::function<void( DriverResponseType )> completionCb ) override;
    void GetCommutationParameter1( std::function<void( DriverResponseType, int32_t )> completionCb ) override;
    void SetCommutationParameter1(
        int32_t commutationParameter1,
        std::function<void( DriverResponseType )> completionCb ) override;
    void
    GetElectricalProperties( std::function<void( DriverResponseType, ElectricalProperties )> completionCb ) override;

    // IEmulatedAxisDriver interface

    void SetCurrentPosition( double position ) override;

    void SetFlagStatusEnable( uint8_t flagIndex, bool enable ) override;

    void InjectErrorWhileIdle( DriverResponseType response ) override;

    //  Methods for the emulation

    /*!
     *  @brief   Calculate how long it will take to perform a move command given (absolute) distance after time scaling
     *  @details Ignores jerk, includes overhead time
     *  @return  A time in milliseconds
     */
    std::chrono::milliseconds MoveDuration( double absDistance ) const;

    /*!
     *  @brief   Calculate how long it will take to move a given (absolute) distance (unscaled)
     *  @details Ignores jerk, has no overhead time
     *  @return  A time in milliseconds
     */
    std::chrono::milliseconds UnscaledMoveDuration( double absDistance ) const;

    /*!
     * @brief Calculate how long it will take to perform a move UDP command.
     * @param[in] repeats The number of UDP moves to perform.
     * @return A time in milliseconds
     */
    std::chrono::milliseconds MoveUDPDuration( uint32_t repeats ) const;

    /*!
     * @brief     Get the current data stream index being used by the motion controller to calculate the target position of the axis during a UDP move.
     * @param[in] completionCb Function to return success or failure when call is complete, and the value.
     */
    void GetCurrentDataStreamIndex( std::function<void( DriverResponseType, uint32_t )> completionCb ) override;

    /*!
     *   @brief      Update a single position at the specified index in the currently selected user defined profile.
     *   @param[in]  index          Zero-based index into the profile data array.
     *   @param[in]  position       The new position in selected units (mm, um, or nm) to write to this index in the profile data array.
     *   @param[in]  completionCb   Function to return success or failure when call is complete.
     */
    void UpdateProfileData( uint32_t index, double position, std::function<void( DriverResponseType )> completionCb )
        override;

protected:
    void OnInitialize() override;

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

    /*!
     *  @brief   Returns whether a flag can be reached given the direction of travel.
     */
    static bool IsCorrectDirectionForFlag( double maximumDistance, double travelDistance );

    static StandardDriverResponse::Enum ReadMotionControllerConfig( std::string pathname );
    static StandardDriverResponse::Enum WriteMotionControllerConfig( std::string pathname, const std::string& msgs );

    void MoveRelative(
        double distance,
        std::chrono::milliseconds duration,
        std::function<void( DriverResponseType )> completionCb );

    // Emulate a small, random position error on each call.
    void GeneratePositionError();

    /*! @brief   Get the file path for an emulated trace data file that matches the current trace variables and last movement type. */
    std::string GetEmulatedTraceDataFile();
    /*! @brief   Determine if the provided trace variables match the current trace variables. */
    bool IsMatchingTraceVariables( const std::vector<TraceVariable>& variables );

private:
    friend class EmulatedAxisDriverAttorney;

    //  Values are representative of SCP config, noting of course that actual axes
    //  values will differ per axis instance.
    double m_currentPosition = 0;
    double m_positionError = 0;
    double m_velocity = 360;
    double m_acceleration = 500;
    double m_deceleration = 500;
    double m_jerk = 0; //  ignored
    double m_scalingFactor = 1.0;
    bool m_isRotary = false;
    /*!
     *  flags start and end over a space on the axis
     *  the position is the flag start / latch position, the extent is the end position
     *  NB these may not be in low,high order.
     */
    struct FlagRange
    {
        double m_position;
        double m_extent;
        bool m_enable;
    };
    std::map<uint8_t, FlagRange> m_flagRangesMap;
    double m_timeScalingFactor = 1.0;
    Stopwatch m_stopWatch;
    bool m_isEnabled = true;
    double m_moveDistancePerPulse = 0.0;
    bool m_isTraceCaptureEnabledOnNextMove = false;
    bool m_isTriggerOnPositionEnabled = false;
    DigitalOutputTrigger m_digitalOutputTrigger = DigitalOutputTrigger::Disabled;
    double m_motorCurrent = 0.0;
    std::function<void( DriverResponseType )> m_axisErrorCb;
    MotionTrackingSettings m_motionTrackingSettings{ MotionCompleteMode::MotionSettles, 1.5, 0.23 };
    MotionErrorSettings m_motionErrorSettings{ 0.13, MotionErrorAction::SmoothStop };
    PositionLoopSettings m_positionLoopSettings{ 1, 2, 800, 4, 5, 90, 0, 0, true };
    CurrentLoopSettings m_currentLoopSettings{ 1000, 3000, 20000, true };
    PhaseAngleInfo m_phaseAngleInfo{ 9999, 360, false };
    int32_t m_commutationParameter1 = 1234567;
    BiquadFilterSettings m_biquadFilterSettings{
        { BiquadFilterType::None, 0, 0, 0, 0, 0, 0, 0, 0 },
        { BiquadFilterType::Unknown, 700, 1, 212, 424, 212, 32764, -14930, 7184 } };
    CommunicationSettings m_communicationSettings{
        SerialBaudRate::Baud230400,
        SerialProtocol::MultiDropUsingIdleLineDetection,
        4,
        CanBaudRate::Baud20000,
        5 };

    UDPProfileSPtr m_pUDPProfile;

    std::vector<TraceVariable> m_currentTraceVariables;
    double m_lastMoveDistance = 0;
};

} //  namespace imp
