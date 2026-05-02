/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ICCPDevice.h"

#include <functional>
#include <map>


namespace imp
{
class IMotionControllerConfiguration;
class IAxisDevice;
class IDispatcher;
class IUserDefinedProfile;

enum class TraceTrigger : int32_t;
enum class TraceVariable : int32_t;
enum class DigitalOutputTrigger : int32_t;
enum class AxisRecoveryOperation : int32_t;
enum class ActiveControlMode : int32_t;

/*! @brief  Shorthand for creating shared pointers of IAxisDevice. */
typedef std::shared_ptr<IAxisDevice> IAxisDeviceSPtr;


/*!
 *  @brief   Defines the interface an Axis must adhere to for building motion control systems
 *           within CCP
 */
class IAxisDevice : public virtual ICCPDevice
{
public:
    /*!
     *   @brief      Virtual destructor (empty).
     */
    virtual ~IAxisDevice() = default;

    /*!
    *   @brief      Get the actual position of the axis as reported by the encoder.
    *   @details    On successful completion of a move, the actual position will match the commanded position (within the axis resolution).
                    If the move failed (e.g. a position error occurred), the actual position will not match the commanded position.
                    The commanded position will be set to the actual position when:
                    * A move is interrupted due to a Stop or EStop command being issued.
                    * While the axis is disabled and when the Enable command is issued to transition the axis from the disabled to enabled state.
    *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                    false = Return locally cached value.
                                    true = Send request to h/w and wait for value (blocking).
    *   @returns    Encoder position in selected units (mm, um, or nm).
    */
    virtual double GetActualPosition( bool forceRefresh = true ) = 0;

    /*!
    *   @brief      Get the position the axis was commanded to move to.
    *   @details    On successful completion of a move, the actual position will match the commanded position (within the axis resolution).
                    If the move failed (e.g. a position error occurred), the actual position will not match the commanded position.
                    The commanded position will be set to the actual position when:
                    * A move is interrupted due to a Stop or EStop command being issued.
                    * While the axis is disabled and when the Enable command is issued to transition the axis from the disabled to enabled state.
    *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                    false = Return locally cached value.
                                    true = Send request to h/w and wait for value (blocking).
    *   @returns    Commanded position in selected units (mm, um, or nm).
    */
    virtual double GetCommandedPosition( bool forceRefresh = true ) = 0;

   /*!
     *   @brief      Get the current position error of the axis.
     *   @details    Position error is the difference between the commanded position and the actual position of the axis, in the selected units (mm, um, or nm).
     *               A non-zero value indicates that the axis did not reach the commanded position. This can occur due to overshoot, mechanical limits, or if a move was interrupted.
     *               The position error is useful for monitoring motion accuracy and detecting issues during moves.
     *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
     *                               false = Return locally cached value.
     *                               true  = Send request to hardware and wait for value (blocking).
     *   @returns    Position error in selected units (mm, um, or nm).
     */
    virtual double GetPositionError( bool forceRefresh = true ) = 0;

    /*!
     *   @brief      Set the maximum velocity of the axis.
     *   @param[in]  velocity        Maximum velocity to be set in selected units (mm/s, um/s, or nm/s).
     */
    virtual void SetVelocity( double velocity ) = 0;

    /*!
     *   @brief      Set the maximum acceleration of the axis.
     *   @param[in]  acceleration    Maximum acceleration to be set in selected units (mm/s^2, um/s^2, or nm/s^2).
     */
    virtual void SetAcceleration( double acceleration ) = 0;

    /*!
     *   @brief      Set the maximum jerk of the axis.
     *   @param[in]  jerk            Maximum jerk to be set in selected units (mm/s^3, um/s^3, or nm/s^3).
     */
    virtual void SetJerk( double jerk ) = 0;

    /*!
     *   @brief      Set the maximum deceleration of the axis.
     *   @param[in]  deceleration    Maximum deceleration to be set in selected units (mm/s^2, um/s^2, or nm/s^2).
     */
    virtual void SetDeceleration( double deceleration ) = 0;

    /*!
    *   @brief      Set the maximum velocity of the axis.
    *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                    false = Return locally cached value.
                                    true = Send request to h/w and wait for value (blocking).
    *   @return     Maximum velocity in selected units (mm/s, um/s, or nm/s).
    */
    virtual double GetVelocity( bool forceRefresh = true ) = 0;

    /*!
    *   @brief      Get the maximum acceleration of the axis.
    *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                    false = Return locally cached value.
                                    true = Send request to h/w and wait for value (blocking).
    *   @return     Maximum acceleration in selected units (mm/s^2, um/s^2, or nm/s^2).
    */
    virtual double GetAcceleration( bool forceRefresh = true ) = 0;

    /*!
    *   @brief      Get the maximum jerk of the axis.
    *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                    false = Return locally cached value.
                                    true = Send request to h/w and wait for value (blocking).
    *   @return     Maximum jerk in selected units (mm/s^3, um/s^3, or nm/s^3).
    */
    virtual double GetJerk( bool forceRefresh = true ) = 0;

    /*!
    *   @brief      Get the maximum deceleration of the axis.
    *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                    false = Return locally cached value.
                                    true = Send request to h/w and wait for value (blocking).
    *   @return     Maximum deceleration in selected units (mm/s^2, um/s^2, or nm/s^2).
    */
    virtual double GetDeceleration( bool forceRefresh = true ) = 0;

    /*!
     *   @brief      Get the active control mode of the axis.
         @return    Active control mode of the axis (Normal, UDPTime, UDPSlave, RCP, or ExternalControl).
     */
    virtual ActiveControlMode GetActiveControlMode() = 0;

    /*!
     *   @brief      Move axis to absolute position.
     *   @details    Moves axis to the specified absolute position.
     *   @param[in]  position     Value of the absolute coordinate to move to in selected units (mm, um, or nm).
     */
    virtual void MoveAbsolute( double position ) = 0;

    /*!
     *   @brief      Move axis to relative position.
     *   @details    Moves axis from the current position by the specified value.
     *   @param[in]  distance     Amount by which to move from the current position in selected units (mm, um, or nm).
     */
    virtual void MoveRelative( double distance ) = 0;

    /*!
     *   @brief      Move axis to absolute position by name.
     *   @details    Moves axis to the specified absolute position in um.
     *   @param[in]  position_name   Name of the absolute coordinate to move to.
     */
    virtual void NMoveAbsolute( const std::string& position_name ) = 0;

    /*!
     *   @brief      Move axis to relative position by name.
     *   @details    Moves axis from the current position by the specified value in um.
     *   @param[in]  distance_name   Name of distance by which to move from the current position in um.
     */
    virtual void NMoveRelative( const std::string& distance_name ) = 0;

    /*!
     *   @brief      Move axis to a location that is a fixed named distance offset from a named absolute position.
     *   @details    Moves axis to the resultant offset position in um.
     *   @param[in]  position_name   Name of the absolute position to base the movement on.
     *   @param[in]  distance_name   Name of the relative distance to add on to the absolute position.
     */
    virtual void NMoveOffset( const std::string& position_name, const std::string& distance_name ) = 0;

    /*!
     *   @brief      Move axis to the given named flag searching up to a maximum distance.
     *   @details    Moves axis to the flag if it can be detected, or the maximum distance which would
     *               raise an exception.
     *               If the flag is reached, the axis will be brought to an abrupt stop.
     *   @param[in]  maximumDistance     Maximum distance to search for the flag in selected units (mm, um, or nm)
     *   @param[in]  flagName            Named flag to search for
     */
    virtual void MoveRelativeToFlag( double maximumDistance, const std::string& flagName ) = 0;

    /*!
     *   @brief      Move axis to the given named flag searching up to a named maximum distance
     *   @details    Moves axis to the flag if it can be detected, or the maximum distance which would
     *               raise an exception.
     *               If the flag is reached, the axis will be brought to an abrupt stop.
     *   @param[in]  maximumDistanceName Name maximum distance to search for the flag
     *   @param[in]  flagName            Named flag to search for
     */
    virtual void NMoveRelativeToFlag( const std::string& maximumDistanceName, const std::string& flagName ) = 0;

    /*!
     *   @brief      Move axis to the given named flag in open loop mode.
     *   @details    This command can only be used prior to initialization. It can be used to move an axis to
     *               a known location prior to initialization and homing.
     *   @param[in]  flagName        Named flag to move to.
     *   @param[in]  offsetFromFlag  An offset (+ve or -ve) in selected units (mm, um, or nm) to move away from the flag
     *                               once the flag has been reached. e.g. supports backing off the positive limit (use a -ve offset)
     *                               or negative limit (use a +ve offset).
     */
    virtual void MoveRelativeToFlagOpenLoop( const std::string& flagName, double offsetFromFlag ) = 0;

    /*!
     *   @brief      Move axis to the given named flag in open loop mode.
     *   @details    This command can only be used prior to initialization. It can be used to move an axis to
     *               a known location prior to initialization and homing.
     *   @param[in]  flagName                   Named flag to move to.
     *   @param[in]  offsetFromFlagDistanceName A named distance offset to move away from the flag once the flag
     *                                          has been reached. e.g. supports backing off the positive limit
     *                                          (use a -ve offset) or negative limit (use a +ve offset).
     */
    virtual void
    NMoveRelativeToFlagOpenLoop( const std::string& flagName, const std::string& offsetFromFlagDistanceName ) = 0;

    /*!
     *   @brief      Returns the activation state of the given named flag
     *   @param[in]  flagName            Named flag to query on
     *   @param[in]  forceRefresh        Force the device to do a deep call to hardware.
     *                                   false = Return locally cached value.
     *                                   true = Send request to h/w and wait for value (blocking).
     *   @return     The activation state of the flag
     */
    virtual bool GetFlagStatus( const std::string& flagName, bool forceRefresh = true ) = 0;

    /*!
     *   @brief      Resets the commanded position to match the current encoder position.
     */
    virtual void ResetPosition() = 0;

    /*!
     *   @brief      Resets the current (and possibly encoder) position of the motor, to the specified position
     *   @param[in]  position            New position of the motor in selected units (mm, um, or nm).
     */
    virtual void ResetPosition( double position ) = 0;

    /*!
     *   @brief      Resets the current (and possibly encoder) position of the motor, to the named value
     *   @param[in]  positionName        New named position of the motor
     */
    virtual void NResetPosition( const std::string& positionName ) = 0;

    /*!
     *   @brief      Stop the current movement action in a controlled manner.
     *   @details    Will stop the current profile/velocity move action and return immediately.
     *               The original move command will complete once the axis has come to a halt. If there
     *               is no movement in progress, the operation will complete without error.
     *               Wait() must still called after issuing a Stop() to wait for the axis to come to a halt.
     *               Unlike EStop(), the Stop() command does not require the axis to be re-initialized.
     */
    virtual void Stop() = 0;


    //------------------------------------------------------|
    // Named positions, distances, flags and move profiles  |
    //------------------------------------------------------|

    /*! @brief A map of key, value pairs where the key is a std::string and the value is a numeric type (e.g. T=double). */
    template<typename T>
    using NamedValuesMap = std::map<std::string, T>;
    /*! @brief A map of named positions where name is a std::string and the position is an double. */
    using NamedPositionsMap = NamedValuesMap<double>;
    /*! @brief A map of named distances where name is a std::string and the distance is an double. */
    using NamedDistancesMap = NamedValuesMap<double>;
    /*! @brief A map of named flags where name is a std::string and the index is a uint8_t. */
    using NamedFlagsMap = NamedValuesMap<uint8_t>;
    /*! @brief A map of named move profiles where name is a std::string and the profile is a map of (parameterName, double). */
    using NamedMoveProfilesMap = std::map<std::string, NamedValuesMap<double>>;

    /*!
     *   @brief      Returns a complete list of named positions for this axis.
     *   @return     A map of named positions (positionName, position) for this axis.
     *               Returns an empty map if this axis does not have any named positions.
     */
    virtual NamedPositionsMap GetNamedPositions() const = 0;

    /*!
     *   @brief      Update the list of named positions for this axis.
     *               The list will be persisted to configuration.
     *   @param[in]  namedPositions        A map of named positions (positionName, position) for this axis.
     *   @param[in]  doWriteToLocalConfig  Whether to write updated values to local config.
     */
    virtual void SetNamedPositions( const NamedPositionsMap& namedPositions, bool doWriteToLocalConfig = true ) = 0;

    /*!
     *   @brief      Returns a complete list of named distances for this axis.
     *   @return     A map of named distances (distanceName, distance) for this axis.
     *               Returns an empty map if this axis does not have any named distances.
     */
    virtual NamedDistancesMap GetNamedDistances() const = 0;

    /*!
     *   @brief      Update the list of named distances for this axis.
     *               The list will be persisted to configuration.
     *   @param[in]  namedDistances        A map of named distances (distanceName, distance) for this axis.
     *   @param[in]  doWriteToLocalConfig  Whether to write updated values to local config.
     */
    virtual void SetNamedDistances( const NamedDistancesMap& namedDistances, bool doWriteToLocalConfig = true ) = 0;

    /*!
     *   @brief      Returns a complete list of named flags for this axis.
     *   @return     A map of named flags (flagName, index) for this axis.
     *               Returns an empty map if this axis does not have any named flags.
     */
    virtual NamedFlagsMap GetNamedFlags() const = 0;

    /*!
     *   @brief      Returns a complete list of named move profiles for this axis.
     *   @return     A map of named move profiles (moveProfileName, (parameterName, parameterValue) ) for this axis.
     *               Returns an empty map if this axis does not have any named move profiles.
     */
    virtual NamedMoveProfilesMap GetNamedMoveProfiles() const = 0;

    /*!
     *   @brief      Update the list of named move profiles for this axis.
     *               The list will be persisted to configuration.
     *   @param[in]  namedMoveProfiles     A map of named move profiles (moveProfileName, (parameterName, parameterValue) ) for this axis.
     *   @param[in]  doWriteToLocalConfig  Whether to write updated values to local config.
     */
    virtual void
    SetNamedMoveProfiles( const NamedMoveProfilesMap& namedMoveProfiles, bool doWriteToLocalConfig = true ) = 0;

    /*!
     *   @brief      Sets the Acceleration, Deceleration, Velocity and Jerk specified by the named profile.
     *               Wait must be called before issuing another command to the device.
     *   @throws     std::runtime_error if the move profile does not exist.
     */
    virtual void SelectMoveProfile( const std::string& profileName ) = 0;


    //---------------------------------|
    // Movement callback functionality |
    //---------------------------------|

    /*!
     *  @brief   Contains the information necessary to describe an axis movement,
     *           primarily for the purposes of emulation.
     */
    struct Movement
    {
        /*! @brief The position of the axis before the movement started. */
        double PositionStart;
        /*! @brief The (requested) position of the axis when the movement finishes. */
        double PositionEnd;
        /*! @brief The maximum velocity of the axis when the movement started. */
        double Velocity;
        /*! @brief The maximum acceleration of the axis when the movement started. */
        double Acceleration;
        /*! @brief The maximum jerk of the axis when the movement started. */
        double Jerk;
        /*! @brief The maximum deceleration of the axis when the movement started. */
        double Deceleration;
    };

    /*!
     *  @brief   Defines an interface for receiving notifications
     *           when an axis has started performing a movement,
     *           including a payload describing the movement itself.
     *           Originally designed for use when emulating axis sensor behaviour.
     */
    using MovementStartedCallback = std::function<void( Movement movement )>;

    /*!
     *   @brief      Subscribe for callback on start of axis movement
     *   @param[in]  pDispatcher The dispatcher through which the callback is to be invoked
     *   @param[in]  callback The callback to invoke
     *   @return     A unique identifier allowing the subscription to be unsubscribed
     */
    virtual SubscriptionId
    SubscribeToMovementStarted( const std::shared_ptr<IDispatcher>& pDispatcher, MovementStartedCallback callback ) = 0;

    /*!
     *   @brief      Unsubscribe for callback on start of axis movement
     *   @param[in]  subscriptionId The unique identifier of the subscription
     *               as returned by @link SubscribeToMovementStarted @endlink
     *   @return     Whether the subscription with the given ID was successfully unsubscribed
     */
    virtual bool UnsubscribeFromMovementStarted( SubscriptionId subscriptionId ) = 0;

    /*!
     *   @brief      Enable / energize the axis motor.
     *   @details    This will restore power to the axis motor (after Disable() has been called) so it can be
     *               commanded to move.
     */
    virtual void Enable() = 0;

    /*!
     *   @brief      Disable / de-energize the axis motor.
     *   @details    This will remove power from the axis so it can be
     *               moved manually. If an encoder is present, it will continue to update as the
     *               axis is manually moved.
     */
    virtual void Disable() = 0;

    /*!
     *   @brief      Get the enabled (energized) / disabled (de-energized) state the axis motor.
     *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                     false = Return locally cached value.
                                     true = Send request to h/w and wait for value (blocking).
     *   @return     true if enabled (energized), false if disabled (de-energized).
     */
    virtual bool GetIsEnabled( bool forceRefresh = true ) = 0;

    /*!
     *   @brief      Enable / disable control of the axis via an external pulse signal.
     *   @details    When using this feature with a SmartStageXY or SmartStageLinear, the GpioConfiguration in Instrument.cfg must be set to PMDGpioConfiguration::S1.
     *   @param[in]  moveDistancePerPulse    Move the axis by this amount in selected units (mm, um, or nm)
     *                                       per positive pulse signal
     *                                       and by the negative amount per negative pulse signal.
     *                                       Setting this value to zero will disable external control.
     */
    virtual void SetExternalControl( double moveDistancePerPulse ) = 0;

    /*!
     *   @brief      Get the currently configured move distance per pulse.
     *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                     false = Return locally cached value.
                                     true = Send request to h/w and wait for value (blocking).
     *   @return     moveDistancePerPulse in selected units (mm, um, or nm). Zero indicates
     *               external control is disabled, a non-zero value indicates external control
     *               is enabled.
     */
    virtual double GetExternalControl( bool forceRefresh = true ) = 0;

    /*!
     *   @brief      Enable capture of trace data.
     *   @details    If the trigger is OnMotionStart, trace capture will be automatically started when the next move begins and stopped once the move completes.
     *               If the trigger is OnMotionStartNoEnd, trace capture will be automatically started when the next move begins and must be stopped by calling StopTraceCapture().
     *               If the trigger is Immediate, trace capture will be started immediately and must be stopped by calling StopTraceCapture().
     *   @param[in]  trigger   Trigger to start the trace capture (e.g. Immediate or OnMotionStart).
     *   @param[in]  period    Period at which the data is sampled in terms of the motion chip cycles (e.g. 1 = every cycle).
     *   @param[in]  variables A list of variables to capture. Up to 4 variables can be captured.
     *   @param[in]  waitMotionStabilizeUs Time in us to wait for motion to stabilize. Only applies when the trigger is OnMotionStartDelayed.
     */
    virtual void EnableTraceCapture(
        TraceTrigger trigger,
        uint16_t period,
        const std::vector<TraceVariable>& variables,
        uint64_t waitMotionStabilizeUs = 0 ) = 0;

    /*!
     *   @brief      Enable capture of trace data using a named trace profile defined in configuration.
     *   @details    If the trigger is OnMotionStart, trace capture will be automatically started when the next move begins and stopped once the move completes.
     *               If the trigger is OnMotionStartNoEnd, trace capture will be automatically started when the next move begins and must be stopped by calling StopTraceCapture().
     *               If the trigger is Immediate, trace capture will be started immediately and must be stopped by calling StopTraceCapture().
     *   @param[in]  trigger         Trigger to start the trace capture (e.g. Immediate or OnMotionStart).
     *   @param[in]  traceProfile    The name of the trace profile section defined in configuration. This profile will specify
     *                               the period and up to 4 trace variables.
     */
    virtual void NEnableTraceCapture( TraceTrigger trigger, const std::string& traceProfile ) = 0;

    /*!
     *   @brief      Stop the capture of trace data. This will stop any trace capture currently in progress.
     *   @details    If the trigger supplied to EnableTraceCapture() was OnMotionStart or OnMotionStartNoEnd and a move has not started,
     *               this will cancel the request to capture trace data on the next move.
     */
    virtual void StopTraceCapture() = 0;

    /*!
     *   @brief      Is trace capture enabled for the next axis move? Does not detect immediate trace capture.
     *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                     false = Return locally cached value.
                                     true = Send request to h/w and wait for value (blocking).
     *   @return     true if trace capture is enabled for the next axis move,
     *               false if not enabled for the next axis move or was enabled using the Immediate trigger.
     */
    virtual bool GetIsTraceCaptureEnabled( bool forceRefresh = true ) = 0;

    /*!
     *   @brief      Save the trace data captured during the last move (see EnableTraceCapture() and NEnableTraceCapture()) as a csv file.
     *   @param[in]  pathname    The path to the file to save the data to.
     *   @param[in]  traceCount  The maximum number of trace samples to save to the file.
     */
    virtual void SaveTraceCapture( const std::string& pathname, std::int32_t traceCount) = 0;

    /*!
     *   @brief      Set callback to be invoked when in idle or electronic gearing and an error is observed.
     *   @param[in]  axisErrorCb    Callback to be invoked when an error is detected. This will be invoked on a different thread to the main thread.
     */
    virtual void SetIdleErrorCallback( std::function<void( DeviceResponseType )> axisErrorCb ) = 0;

    /*!
     *   @brief      Enable trigger on position (TOP). TOP will generate a GPIO pulse every 'increment' of travel while in the window specified by
     *               'startPosition' and 'endPosition'.
     *   @details    When using this feature with a SmartStageXY or SmartStageLinear, the GpioConfiguration in Instrument.cfg must be set to PMDGpioConfiguration::S2 (the default).
     *   @param[in]  startPosition    Position at which to start generating pulses in selected units (mm, um, or nm).
     *   @param[in]  endPosition      Position at which to stop generating pulses in selected units (mm, um, or nm).
     *   @param[in]  increment        Generate a pulse every 'increment' of travel in selected units (mm, um, or nm).
     *   @param[in]  isBidirectional  If true, pulses will also be generated when moving from endPosition back to startPosition.
     *                                If false, pulses will only be generated when moving from startPosition to endPosition.
     *   @param[in]  pulseWidthNs     The width of the pulse to generate at each position in nanoseconds.
     *                                The actual pulse width is a value rounded to the nearest practicable value, which must be a multiple of 50ns and less or equal to 204,800 ns.
     *                                It's ignored by version 1.2 and lower of the PMD motion control firmware.
     */
    virtual void EnableTriggerOnPosition(
        double startPosition,
        double endPosition,
        double increment,
        bool isBidirectional,
        int pulseWidthNs ) = 0;

    /*!
     *   @brief      Enable trigger on position (TOP). TOP will generate a GPIO pulse every 'increment' of travel while in the window specified by
     *               'startPosition' and 'endPosition'.
     *   @details    When using this feature with a SmartStageXY or SmartStageLinear, the GpioConfiguration in Instrument.cfg must be set to PMDGpioConfiguration::S2 (the default).
     *   @param[in]  startPositionName Named position at which to start generating pulses.
     *   @param[in]  endPositionName   Named position at which to stop generating pulses.
     *   @param[in]  namedDistance     Generate a pulse every 'namedDistance' of travel.
     *   @param[in]  isBidirectional  If true, pulses will also be generated when moving from endPosition back to startPosition.
     *                                If false, pulses will only be generated when moving from startPosition to endPosition.
     *   @param[in]  pulseWidthNs     The width of the pulse to generate at each position in nanoseconds.
     *                                The actual pulse width is a value rounded to the nearest practicable value, which must be a multiple of 50ns and less or equal to 204,800 ns.
     *                                It's ignored by version 1.2 and lower of the PMD motion control firmware.
     */
    virtual void NEnableTriggerOnPosition(
        const std::string& startPositionName,
        const std::string& endPositionName,
        const std::string& namedDistance,
        bool isBidirectional,
        int pulseWidthNs ) = 0;

    /*!
     *   @brief      Disable trigger on position (TOP).
     */
    virtual void DisableTriggerOnPosition() = 0;

    /*!
     *   @brief      Get the state of the trigger on position (TOP) function.
     *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                     false = Return locally cached value.
                                     true = Send request to h/w and wait for value (blocking).
     *   @return     true if enabled, false if disabled.
     */
    virtual bool GetIsTriggerOnPositionEnabled( bool forceRefresh = true ) = 0;

    /*!
     *   @brief      Enable / disable a digital output (AxisOut1) trigger based on the axis motion.
     *   @details    When using this feature with a SmartStageXY or SmartStageLinear, the GpioConfiguration in Instrument.cfg must be set to PMDGpioConfiguration::S2 (the default).
     *   @param[in]  trigger    Specifies a trigger on which to set the digital output (see enum DigitalOutputTrigger for details).
     */
    virtual void SetDigitalOutputTrigger( DigitalOutputTrigger trigger ) = 0;

    /*!
     *   @brief      Get the digital output trigger in effect.
     *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                     false = Return locally cached value.
                                     true = Send request to h/w and wait for value (blocking).
     *   @return     The DigitalOutputTrigger currently in effect (see enum DigitalOutputTrigger for details).
     */
    virtual DigitalOutputTrigger GetDigitalOutputTrigger( bool forceRefresh = true ) = 0;

    /*!
    *   @brief      Get the RMS current being used to drive the motor at this instant.
    *   @details    Due to communication latency with the motor drive chip, the value returned will be lagging.
    *   @param[in]  forceRefresh    Force the device to do a deep call to hardware.
                                    false = Return locally cached value.
                                    true = Send request to h/w and wait for value (blocking).
    *   @returns    Motor current in Amps.
    */
    virtual double GetMotorCurrent( bool forceRefresh = true ) = 0;

    /*!
    *   @brief      Get the recovery operation (if any) required to return the axis to a state where it can successfully perform moves.
    *   @param[in]  forceRefresh    Force the device to do a deep call to hardware (to get the current enabled/disable state of the motor).
                                    false = Return locally cached value.
                                    true = Send request to h/w and wait for value (blocking).
    *   @returns    An enum indicating the type of recovery operation required.
    */
    virtual AxisRecoveryOperation GetRecoveryOperation( bool forceRefresh = true ) = 0;

    /*!
     *   @brief      Get an interface for reading and writing various motion controller settings from/to memory/NVRAM.
     *   @returns    The above interface.
     */
    virtual IMotionControllerConfiguration& GetMotionControllerConfiguration() = 0;

    /*!
     *   @brief      Get an interface for performing User Defined Profile (UDP) moves and updating the associated user defined profile data.
     *   @returns    The above interface.
     */
    virtual IUserDefinedProfile& GetUserDefinedProfile() = 0;

    /*!
     *   @brief      Move axis using the currently selected time-based UDP profile.
     *               A time-based UDP profile must have first been selected by calling SelectMoveProfile().
     *               If a time-based UDP profile is not active, this method will return an error.
     *   @details    This method is obsolete. Call the equivalent IUserDefinedProfile.MoveRelativeUDP instead.
     *   @param[in]  repeat     Number of times to repeat move.
     */
    virtual void MoveRelativeUDP( uint32_t repeat ) = 0;

    /*!
     *   @brief      Moves axis to the specified absolute position in selected units (mm, um, or nm).
     *               Unlike other move commands, this command can be issued while there is a move in progress.
     *   @details    It uses the supplied trajectory parameters (velocity, acceleration, deceleration), not the currently selected move profile.
     *               The trajectory parameters can also be modified during a move.
     *               Note that only trapezoidal moves are supported for continuous moves, not s-curve moves, hence the lack of a jerk parameter.
     *   @param[in]  position     Value of the absolute coordinate to move to in selected units (mm, um, or nm).
     *   @param[in]  acceleration Maximum acceleration to be set in selected units (mm/s^2, um/s^2, or nm/s^2).
     *   @param[in]  velocity     Maximum velocity to be set in selected units (mm/s, um/s, or nm/s).
     *   @param[in]  deceleration Maximum deceleration to be set in selected units (mm/s^2, um/s^2, or nm/s^2).
     */
    virtual void MoveContinuous( double position, double acceleration, double velocity, double deceleration ) = 0;

    /*!
     *   @brief      Set settings necessary before homing routine starts. Any settings replaced can be restored with ExitHomingMode().
     *   @details    Homing mode will save the MotionCompleteMode in buffer memory and set it to CommandedPositionReached.
     *               Initialization should be complete before calling this method, as it resets the homing mode.
     *   @throws     std::runtime_error if homing mode is already active.
     */
    virtual void EnterHomingMode() = 0;

    /*!
     *   @brief      Restore settings changed by EnterHomingMode() and exit homing mode.
     *   @throws     std::runtime_error if homing mode is not active.
     */
    virtual void ExitHomingMode() = 0;
};

} //  namespace imp
