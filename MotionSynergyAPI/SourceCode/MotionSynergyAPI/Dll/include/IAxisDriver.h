/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "BiquadFilterSettings.h"
#include "CommunicationSettings.h"
#include "CurrentLoopSettings.h"
#include "ElectricalProperties.h"
#include "FirmwareVersionInfo.h"
#include "IDeviceDriver.h"
#include "MotionErrorSettings.h"
#include "MotionTrackingSettings.h"
#include "PhaseAngleInfo.h"
#include "PositionLoopSettings.h"

#include <vector>

namespace imp
{
enum class MotionControllerConfigurationType : int32_t;

enum class TraceTrigger : int32_t;
enum class TraceVariable : int32_t;
enum class DigitalOutputTrigger : int32_t;

class RMEProfile;
using RMEProfileSPtr = std::shared_ptr<RMEProfile>;

class UDPProfile;
using UDPProfileSPtr = std::shared_ptr<UDPProfile>;

/*! @brief  Shorthand for creating shared pointers of IAxisDriver. */
class IAxisDriver;
using IAxisDriverSPtr = std::shared_ptr<IAxisDriver>;

/*!
 *  @brief   Defines the interface an Axis driver must adhere to for building motion control systems
 *           within CCP
 */
class IAxisDriver : public virtual IDeviceDriver
{
public:
    IAxisDriver() = default;
    virtual ~IAxisDriver() = default;

    /*!
     *   @brief      Get the actual position of the axis as reported by the encoder.
     *   @details    Callback will return Encoder position in selected units (mm, um, or nm).
                     On successful completion of a move, the actual position will match the commanded position (within the axis resolution).
                     If the move failed (e.g. a position error occurred), the actual position will not match the commanded position.
                     The commanded position will be set to the actual position when:
                     * A move is interrupted due to a Stop or EStop command being issued.
                     * While the axis is disabled and when the Enable command is issued to transition the axis from the disabled to enabled state.
     *   @param[in]  completionCb    Function to return success/failure and value when call is complete.
     */
    virtual void GetActualPosition( std::function<void( DriverResponseType, double )> completionCb ) = 0;

    /*!
     *   @brief      Get the position the axis was commanded to move to.
     *   @details    Commanded position in selected units (mm, um, or nm).
                     On successful completion of a move, the actual position will match the commanded position (within the axis resolution).
                     If the move failed (e.g. a position error occurred), the actual position will not match the commanded position.
                     The commanded position will be set to the actual position when:
                     * A move is interrupted due to a Stop or EStop command being issued.
                     * While the axis is disabled and when the Enable command is issued to transition the axis from the disabled to enabled state.
     *   @param[in]  completionCb    Function to return success/failure and value when call is complete.
     */
    virtual void GetCommandedPosition( std::function<void( DriverResponseType, double )> completionCb ) = 0;

    /*!
     *   @brief      Get the current position error of the axis.
     *   @details    Position error is the difference between the commanded position and the actual position of the axis, in the selected units (mm, um, or nm).
     *               A non-zero value indicates that the axis did not reach the commanded position, which can occur due to overshoot, mechanical limits, or interruptions.
     *               This function can be used for monitoring motion accuracy and detecting issues during moves.
     *   @param[in]  completionCb    Function to return success/failure and the position error value when the call is complete.
     */
    virtual void GetPositionError( std::function<void( DriverResponseType, double )> completionCb ) = 0;

    /*!
     *   @brief      Set the maximum velocity of the axis.
     *   @param[in]  velocity        Maximum velocity to be set in selected units (mm/s, um/s, or nm/s).
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void SetVelocity( double velocity, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Set the maximum acceleration of the axis.
     *   @param[in]  acceleration    Maximum acceleration to be set in selected units (mm/s^2, um/s^2, or nm/s^2).
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void SetAcceleration( double acceleration, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Set the maximum jerk of the axis.
     *   @param[in]  jerk            Maximum jerk to be set in selected units (mm/s^3, um/s^3, or nm/s^3).
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void SetJerk( double jerk, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Set the maximum deceleration of the axis.
     *   @param[in]  deceleration    Maximum deceleration to be set in selected units (mm/s^2, um/s^2, or nm/s^2).
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void SetDeceleration( double deceleration, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Get the maximum velocity of the axis.
     *   @details    Maximum velocity in selected units (mm/s, um/s, or nm/s).
     *   @param[in]  completionCb    Function to return success/failure and value when call is complete.
     */
    virtual void GetVelocity( std::function<void( DriverResponseType, double )> completionCb ) = 0;

    /*!
     *   @brief      Get the maximum acceleration of the axis.
     *   @details    Maximum acceleration in selected units (mm/s^2, um/s^2, or nm/s^2).
     *   @param[in]  completionCb    Function to return success/failure and value when call is complete.
     */
    virtual void GetAcceleration( std::function<void( DriverResponseType, double )> completionCb ) = 0;

    /*!
     *   @brief      Get the maximum jerk of the axis.
     *   @details    Maximum jerk in selected units (mm/s^3, um/s^3, or nm/s^3).
     *   @param[in]  completionCb    Function to return success/failure and value when call is complete.
     */
    virtual void GetJerk( std::function<void( DriverResponseType, double )> completionCb ) = 0;

    /*!
     *   @brief      Get the maximum deceleration of the axis.
     *   @details    Maximum deceleration in selected units (mm/s^2, um/s^2, or nm/s^2).
     *   @param[in]  completionCb    Function to return success/failure and value when call is complete.
     */
    virtual void GetDeceleration( std::function<void( DriverResponseType, double )> completionCb ) = 0;

    /*!
     *   @brief      Move axis to absolute position.
     *   @details    Moves axis to the specified absolute position.
     *   @param[in]  position     Value of the absolute coordinate to move to in selected units (mm, um, or nm).
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void MoveAbsolute( double position, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Move axis to relative position.
     *   @details    Moves axis from the current position by the specified value.
     *   @param[in]  distance     Amount by which to move from the current position in selected units (mm, um, or nm).
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void MoveRelative( double distance, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Move axis to the given flag searching up to a maximum distance
     *   @details    Moves axis to the flag if it can be detected, or the maximum distance which would
     *               raise an exception
     *   @param[in]  maximumDistance     Maximum distance to search for the flag in selected units (mm, um, or nm)
     *   @param[in]  flagIndex           Driver-dependent flag index to search for
     *   @param[in]  completionCb        Function to return success or failure when call is complete.
     */
    virtual void MoveRelativeToFlag(
        double maximumDistance,
        uint8_t flagIndex,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Move axis to the given named flag in open loop mode.
     *   @details    This command can only be used prior to initialization. It can be used to move an axis to
     *               a known location prior to initialization and homing.
     *   @param[in]  flagIndex           Driver-dependent flag index to search for
     *   @param[in]  offsetFromFlag      An offset in selected units (mm, um, or nm) to move away from the flag
     *                                   once the flag has been reached. e.g. supports backing off the positive
     *                                   or negative limit.
     *   @param[in]  completionCb        Function to return success or failure when call is complete.
     */
    virtual void MoveRelativeToFlagOpenLoop(
        uint8_t flagIndex,
        double offsetFromFlag,
        std::function<void( DriverResponseType )> completionCb ) = 0;


    /*!
     *   @brief      Returns the activation state of all flags on the axis
     *   @param[in]  completionCb        Function to return success or failure when call is complete.
     */
    virtual void GetFlagStatuses( std::function<void( DriverResponseType, uint32_t )> completionCb ) = 0;

    /*!
     *   @brief      Resets the commanded position to match the current encoder position.
     *   @param[in]  completionCb        Function to return success or failure when call is complete (the actual position is returned).
     */
    virtual void ResetToActualPosition( std::function<void( DriverResponseType, double )> completionCb ) = 0;

    /*!
     *   @brief      Resets the current (and if present, the encoder) position of the motor
     *   @param[in]  position            New position of the motor in selected units (mm, um, or nm).
     *   @param[in]  completionCb        Function to return success or failure when call is complete.
     */
    virtual void ResetPosition( double position, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Stop the current movement action without error
     *   @details    Will stop the current profile/velocity move action and return immediately.
     *               The original move command will complete once the axis has idled. If there
     *               is no movement in progress, the operation will complete without error.
     *   @param[in]  completionCb        Function to return success or failure when call is complete.
     */
    virtual void Stop( std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Enable / energize the axis motor.
     *   @details    This will restore power to the axis motor (after Disable() has been called) so it can be
     *               commanded to move.
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void Enable( std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Disable / de-energize the axis motor.
     *   @details    This will remove power from the axis so it can be
     *               moved manually. If an encoder is present, it will continue to update as the
     *               axis is manually moved.
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void Disable( std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Get the enabled (energized) / disabled (de-energized) state the axis motor.
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void GetIsEnabled( std::function<void( DriverResponseType, bool )> completionCb ) = 0;

    /*!
     *   @brief      Enable / disable control of the axis via an external pulse signal.
     *   @param[in]  moveDistancePerPulse    Move the axis by this amount in selected units (mm, um, or nm)
     *                                       per positive pulse signal
     *                                       and by the negative amount per negative pulse signal.
     *                                       Setting this value to zero will disable external control.
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void
    SetExternalControl( double moveDistancePerPulse, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Get the currently configured move distance per pulse.
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               moveDistancePerPulse in selected units (mm, um, or nm). Zero indicates
     *               external control is disabled, a non-zero value indicates external control
     *               is enabled.
     */
    virtual void GetExternalControl( std::function<void( DriverResponseType, double )> completionCb ) = 0;

    /*!
     *   @brief      Enable capture of trace data.
     *   @details    If the trigger is OnMotionStart, trace capture will be stopped automatically once the move completes.
     *               If the trigger is OnMotionStartNoEnd, trace capture will be automatically started when the next move begins and must be stopped by calling StopTraceCapture().
     *               If the trigger is Immediate, trace capture must be stopped by calling StopTraceCapture().
     *   @param[in]  trigger   Trigger to start the trace capture (e.g. Immediate or OnMotionStart).
     *   @param[in]  period    Period at which the data is sampled in terms of the motion chip cycles (e.g. 1 = every cycle).
     *   @param[in]  variables A list of variables to capture. Up to 4 variables can be captured.
     *   @param[in]  waitMotionStabilizeUs Time in us to wait for motion to stabilize. Only applies when the trigger is OnMotionStartDelayed.
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void EnableTraceCapture(
        TraceTrigger trigger,
        uint16_t period,
        const std::vector<TraceVariable>& variables,
        uint64_t waitMotionStabilizeUs,
        std::function<void( DriverResponseType )> completionCb) = 0;

    /*!
     *   @brief      Stop the capture of trace data. This will stop any trace capture currently in progress.
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void StopTraceCapture( std::function<void( DriverResponseType )> completionCb ) = 0;


    /*!
     *   @brief      Is trace capture enabled for the next axis move? Does not detect immediate trace capture.
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               return true if trace capture is enabled for the next axis move,
     *               false if not enabled for the next axis move or was enabled using the Immediate trigger.
     */
    virtual void GetIsTraceCaptureEnabled( std::function<void( DriverResponseType, bool )> completionCb ) = 0;

    /*!
     *   @brief      Save the trace data captured during the last move (see EnableTraceCapture() and NEnableTraceCapture()) as a csv file.
     *   @param[in]  pathname    The path to the file to save the data to.
     *   @param[in]  traceCount    The maximum number of trace samples to save to the file.
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void
    SaveTraceCapture( const std::string& pathname, std::int32_t traceCount, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Set callback to be invoked when in idle or electronic gearing and an error is observed.
     *   @param[in]  axisErrorCb    Callback to be invoked when an error is detected. This will be invoked on a different thread to the main thread.
     */
    virtual void SetIdleErrorCallback( std::function<void( DriverResponseType )> axisErrorCb ) = 0;

    /*!
     *   @brief      Enable trigger on position (TOP). TOP will generate a GPIO pulse every 'increment' of travel while in the window specified by
     *               'startPosition' and 'endPosition'.
     *   @param[in]  startPosition    Position at which to start generating pulses in selected units (mm, um, or nm).
     *   @param[in]  endPosition      Position at which to stop generating pulses in selected units (mm, um, or nm).
     *   @param[in]  increment        Generate a pulse every 'increment' of travel in selected units (mm, um, or nm).
     *   @param[in]  isBidirectional  If true, pulses will also be generated when moving from endPosition back to startPosition.
     *                                If false, pulses will only be generated when moving from startPosition to endPosition.
     *   @param[in]  pulseWidthNs     The width of the pulse to generate at each position in nanoseconds.
     *                                The actual pulse width is a value rounded to the nearest practicable value, which must be a multiple of 50ns and less or equal to 204,800 ns.
     *                                It's ignored by version 1.2 and lower of the PMD motion control firmware.
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void EnableTriggerOnPosition(
        double startPosition,
        double endPosition,
        double increment,
        bool isBidirectional,
        int pulseWidthNs,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Disable trigger on position (TOP).
     *   @param[in]  completionCb    Function to return success or failure when call is complete.
     */
    virtual void DisableTriggerOnPosition( std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Get the state of the trigger on position (TOP) function.
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               return true if TOP is enabled, false if not.
     */
    virtual void GetIsTriggerOnPositionEnabled( std::function<void( DriverResponseType, bool )> completionCb ) = 0;

    /*!
     *   @brief      Enable / disable a digital output trigger (AxisOut1) based on the axis motion.
     *   @param[in]  trigger        Specifies a trigger on which the digital output will be set high or disable triggering.
     *   @param[in]  completionCb   Function to return success or failure when call is complete.
     */
    virtual void
    SetDigitalOutputTrigger( DigitalOutputTrigger trigger, std::function<void( DriverResponseType )> completionCb ) = 0;


    /*!
     *   @brief      Get the digital output trigger in effect.
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               return the current digital output trigger in effect.
     */
    virtual void
    GetDigitalOutputTrigger( std::function<void( DriverResponseType, DigitalOutputTrigger )> completionCb ) = 0;

    /*!
     *   @brief      Get the RMS current being used to drive the motor at this instant.
     *   @details    Due to communication latency with the motor drive chip, the value returned will be lagging.
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               return the motor current in Amps.
     */
    virtual void GetMotorCurrent( std::function<void( DriverResponseType, double )> completionCb ) = 0;

    /*!
     *   @brief      Set the RME parameters associated with the currently selected move profile.
     *   @param[in]  pProfile A pointer to a set of RME profile parameters or a nullptr if the currently selected move
     *                        profile is not an RME profile.
     *   @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void SetRMEProfile( RMEProfileSPtr pProfile, std::function<void( DriverResponseType )> completionCb ) = 0;

    // MotionControllerConfiguration methods

    /*!
     *   @brief      Read the current motion tracking settings from the motion controller's memory.
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               return the current motion tracking settings.
     */
    virtual void
    GetMotionTracking( std::function<void( DriverResponseType, MotionTrackingSettings )> completionCb ) = 0;

    /*!
     *  @brief      Set the motion tracking settings in the motion controller's memory.
     *  @param[in]  motionTrackingSettings The new motion tracking settings to apply.
     *  @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void SetMotionTracking(
        MotionTrackingSettings motionTrackingSettings,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Read the current motion error settings from the motion controller's memory.
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               return the current motion error settings.
     */
    virtual void GetMotionError( std::function<void( DriverResponseType, MotionErrorSettings )> completionCb ) = 0;

    /*!
     *  @brief      Set the motion error settings in the motion controller's memory.
     *  @param[in]  motionErrorSettings The new motion error settings to apply.
     *  @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void SetMotionError(
        MotionErrorSettings motionErrorSettings,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Read the position loop settings from the motion controller's memory.
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               return the position loop settings.
     */
    virtual void GetPositionLoop( std::function<void( DriverResponseType, PositionLoopSettings )> completionCb ) = 0;

    /*!
     *  @brief      Set the position loop settings in the motion controller's memory.
     *  @param[in]  positionLoopSettings The new position loop settings to apply.
     *  @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void SetPositionLoop(
        PositionLoopSettings positionLoopSettings,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Read the current loop settings from the motion controller's memory.
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               return the current loop settings.
     */
    virtual void GetCurrentLoop( std::function<void( DriverResponseType, CurrentLoopSettings )> completionCb ) = 0;

    /*!
     *  @brief      Set the current loop settings in the motion controller's memory.
     *  @param[in]  currentLoopSettings The new current loop settings to apply.
     *  @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void SetCurrentLoop(
        CurrentLoopSettings currentLoopSettings,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Read the biquad filter settings from the motion controller's memory.
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               return the biquad filter settings.
     */
    virtual void GetBiquadFilters( std::function<void( DriverResponseType, BiquadFilterSettings )> completionCb ) = 0;

    /*!
     *  @brief      Set the biquad filter settings in the motion controller's memory.
     *  @param[in]  biquadFilterSettings The new biquad filter settings to apply.
     *  @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void SetBiquadFilters(
        BiquadFilterSettings biquadFilterSettings,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Read the current communication settings.
     *   @details    These will correspond to the settings in the motion controller's memory and NVRAM, unless there
     *               has been a call to SetCommunication(). In this case, it will return the updated settings supplied to SetCommunication(),
     *               which are buffered, pending a call to WriteToNVRAM().
     *   @param[in]  completionCb    Function to return success or failure when call is complete and
     *               return the current communication settings.
     */
    virtual void GetCommunication( std::function<void( DriverResponseType, CommunicationSettings )> completionCb ) = 0;

    /*!
     *  @brief      Set the communication settings to be written to NVRAM (note the motion controller's memory is NOT updated).
     *  @param[in]  communicationSettings The new communication settings to apply when WriteToNVRAM() is called.
     *  @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void SetCommunication(
        CommunicationSettings communicationSettings,
        std::function<void( DriverResponseType )> completionCb ) = 0;


    /*!
     *  @brief      Transfer the specified types of configuration settings from the motion controller's memory to NVRAM so they will persist across power cycles.
     *  @details    Note that the axis needs to be de-energized (disabled) before calling this function.
     *              If the active communication settings are changed, the communications link needs to be re-established (i.e. disconnect followed by a connect or initialize).
     *  @param[in]  configurationTypes The types of configuration settings to persist.
     *  @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void WriteToNVRAM(
        std::vector<MotionControllerConfigurationType> configurationTypes,
        std::function<void( DriverResponseType )> completionCb ) = 0;


    /*!
     *   @brief      Restore from a PMD configuration script to the Magellan IC's working memory.
     *               Note that the axis needs to be de-energized (disabled) before calling this function.
     *   @param[in]  pathname The path to the PMD configuration script file to read.
     *   @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void RestoreMotionControllerConfiguration(
        std::string pathname,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Save the Magellan IC's NVRAM configuration as a PMD configuration script.
     *               Note that the axis needs to be de-energized (disabled) before calling this function.
     *   @param[in]  pathname The path to the PMD configuration script to write.
     *   @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void SaveMotionControllerConfiguration(
        std::string pathname,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Restore from a PMD configuration script to the Magellan IC's NVRAM configuration.
     *               Note that the axis needs to be de-energized (disabled) before calling this function.
     *   @param[in]  pathname The path to the PMD configuration script file to read.
     *   @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void RestoreMotionControllerConfigurationToNVRAM(
        std::string pathname,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Gets the firmware version of the PMD controller.
     *   @param[in]  completionCb    Function to return success/failure and value when call is complete.
     */
    virtual void GetFirmwareVersion( std::function<void( DriverResponseType, FirmwareVersionInfo )> completionCb ) = 0;

    /*!
     *   @brief      Gets the current phase angle from the PMD controller.
     *   @param[in]  completionCb    Function to return success/failure and value when call is complete.
     */
    virtual void GetPhaseAngle( std::function<void( DriverResponseType, PhaseAngleInfo )> completionCb ) = 0;

    /*!
     * @brief     Set the phase angle to be written to the PMD controller.
     * @param[in] rawPhaseAngle The raw phase angle to set.
     * @param[in] completionCb Function to return success or failure when call is complete.
     */
    virtual void
    SetPhaseAngle( PhaseAngleInfo phaseAngleInfo, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     * @brief     Get the current value associated with commutation parameter 1 from the PMD controller.
     * @param[in] completionCb Function to return success or failure when call is complete, and the value.
     */
    virtual void GetCommutationParameter1( std::function<void( DriverResponseType, int32_t )> completionCb ) = 0;

    /*!
     * @brief     Set the commutation parameter 1 value to be written to the PMD controller.
     * @param[in] value The value to set.
     * @param[in] completionCb Function to return success or failure when call is complete.
     */
    virtual void SetCommutationParameter1( int32_t value, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Gets the electrical properties from the PMD controller.
     *   @param      completionCb    Function to return success/failure and value when call is complete.
     */
    virtual void
    GetElectricalProperties( std::function<void( DriverResponseType, ElectricalProperties )> completionCb ) = 0;

    /*!
     *   @brief      Set the UDP parameters associated with the currently selected move profile.
     *   @param[in]  pProfile A pointer to a set of RME profile parameters or a nullptr if the currently selected move
     *                        profile is not an RME profile.
     *   @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void SetUDPProfile( UDPProfileSPtr pProfile, std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     *   @brief      Move axis using the currently selected time-based UDP profile.
     *               A time-based UDP profile must have first been selected by calling SelectMoveProfile().
     *               If a time-based UDP profile is not active, this method will return an error.
     *   @param[in]  repeat     Number of times to repeat move.
     *   @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void MoveRelativeUDP( uint32_t repeat, std::function<void( DriverResponseType )> completionCb ) = 0;

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
     *   @param[in]  completionCb Function to return success or failure when call is complete.
     */
    virtual void MoveContinuous(
        double position,
        double acceleration,
        double velocity,
        double deceleration,
        std::function<void( DriverResponseType )> completionCb ) = 0;

    /*!
     * @brief     Get the current data stream index being used by the motion controller to calculate the target position of the axis during a UDP move.
     * @param[in] completionCb Function to return success or failure when call is complete, and the value.
     */
    virtual void GetCurrentDataStreamIndex( std::function<void( DriverResponseType, uint32_t )> completionCb ) = 0;

    /*!
     *   @brief      Update a single position at the specified index in the currently selected user defined profile.
     *   @param[in]  index          Zero-based index into the profile data array.
     *   @param[in]  position       The new position in selected units (mm, um, or nm) to write to this index in the profile data array.
     *   @param[in]  completionCb   Function to return success or failure when call is complete.
     */
    virtual void
    UpdateProfileData( uint32_t index, double position, std::function<void( DriverResponseType )> completionCb ) = 0;
};

} //  namespace imp
