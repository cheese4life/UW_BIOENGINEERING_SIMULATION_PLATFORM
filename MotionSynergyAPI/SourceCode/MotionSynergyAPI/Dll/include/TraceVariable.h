/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "EnumClass.h"

// clang-format off

namespace imp
{

/*!
 *  @brief   Defines the set of trace variables which can be captured during a move.
 *           See EnableTraceCapture().
 */
enum class TraceVariable : int32_t
{
    None = 0,                                   /*!< Used to terminate the list of trace variables to capture. */
    PositionError = 1,                          /*!< The difference between the actual and commanded position. */
    CommandedPosition = 2,                      /*!< The commanded position output from the profile generator. */
    CommandedVelocity = 3,                      /*!< The commanded velocity output from the profile generator. */
    CommandedAcceleration = 4,                  /*!< The commanded acceleration output from the profile generator. */
    ActualPosition = 5,                         /*!< The actual position of the motor. */
    ActualVelocity = 6,                         /*!< The actual velocity (calculated using a simple low-pass filter). */
    ActiveMotorCommand = 7,                     /*!< The instantaneous motor command. */
    MotionControlICTime = 8,                    /*!< The motion control IC time (units of servo cycles). */
    PositionCaptureRegister = 9,                /*!< The contents of the high-speed Capture register. */

    PositionLoopIntegratorSum = 10,             /*!< The integral value used in the position loop PID filter. */
    PositionLoopIntegratorContribution = 57,    /*!< The contribution of the integral portion of the PID filter. */
    PositionLoopDerivative = 11,                /*!< The derivative value of the position loop PID filter. */
    Biquad1Input = 64,                          /*!< The value input to biquad 1 filter. */
    Biquad2Input = 65,                          /*!< The value input to biquad 2 filter. */

    EventStatusRegister = 12,                   /*!< The Event Status register. */
    ActivityStatusRegister = 13,                /*!< The Activity Status register. */
    SignalStatusRegister = 14,                  /*!< The Signal Status register. */
    DriveStatusRegister = 56,                   /*!< The Drive Status register. */
    DriveFaultStatusRegister = 79,              /*!< The Drive Fault Status register. */

    PhaseAngle = 15,                            /*!< The motor phase angle. */
    PhaseOffset = 16,                           /*!< The phase offset. */
    PhaseACommand = 17,                         /*!< The output command for Phase A. */
    PhaseBCommand = 18,                         /*!< The output command for Phase B. */
    PhaseCCommand = 19,                         /*!< The output command for Phase C. */

    AnalogInput0 = 20,                          /*!< The most recently read value from the Analog0 signal. */
    AnalogInput1 = 21,                          /*!< The most recently read value from the Analog1 signal. */
    AnalogInput2 = 22,                          /*!< The most recently read value from the Analog2 signal. */
    AnalogInput3 = 23,                          /*!< The most recently read value from the Analog3 signal. */
    AnalogInput4 = 24,                          /*!< The most recently read value from the Analog4 signal. */
    AnalogInput5 = 25,                          /*!< The most recently read value from the Analog5 signal. */
    AnalogInput6 = 26,                          /*!< The most recently read value from the Analog6 signal. */
    AnalogInput7 = 27,                          /*!< The most recently read value from the Analog7 signal. */
    PhaseAngleScaled = 29,                      /*!< The normalized phase angle, scaled from 0 to 360 deg (0 to 0x7fff) rather than in encoder counts. */

    PhaseAReference = 66,                       /*!< The current loop reference for phase A. */
    PhaseAError = 30,                           /*!< The current loop Error for phase A. */
    PhaseAActualCurrent = 31,                   /*!< The current loop actual current for phase A. */
    PhaseAIntegratorSum = 32,                   /*!< The current loop integral sum for phase A. */
    PhaseAIntegratorContribution = 33,          /*!< The current loop integrator contribution to PI filter for phase A. */
    CurrentLoopAOutput = 34,                    /*!< The current loop output for phase A. */

    PhaseBReference = 67,                       /*!< The current loop reference for phase B. */
    PhaseBError = 35,                           /*!< The current loop Error for phase B. */
    PhaseBActualCurrent = 36,                   /*!< The current loop actual current for phase B. */
    PhaseBIntegratorSum = 37,                   /*!< The current loop integral sum for phase B. */
    PhaseBIntegratorContribution = 38,          /*!< The current loop integrator contribution to PI filter for phase B. */
    CurrentLoopBOutput = 39,                    /*!< he current loop output for phase B. */

    FOCDReference = 40,                         /*!< The FOC reference for D (direct) loop. */
    FOCDError = 41,                             /*!< The FOC D (direct) loop error. */
    FOCDFeedback = 42,                          /*!< The FOC D (direct) feedback current. */
    FOCDIntegratorSum = 43,                     /*!< The FOC integral for D (direct) loop. */
    FOCDIntegratorContribution = 44,            /*!< The FOC integrator contribution for D (direct) loop. */
    FOCDOutput = 45,                            /*!< The FOC output for D (direct) loop. */

    FOCQReference = 46,                         /*!< The FOC reference for Q (quadrature) loop. */
    FOCQError = 47,                             /*!< The FOC Q (quadrature) loop error. */
    FOCQFeedback = 48,                          /*!< The FOC Q (quadrature) feedback current. */
    FOCQIntegratorSum = 49,                     /*!< The FOC integral for Q (quadrature) loop. */
    FOCQIntegratorContribution = 50,            /*!< The FOC integrator contribution for Q (quadrature) loop. */
    FOCQOutput = 51,                            /*!< The FOC output Q (quadrature) loop. */

    FOCAlphaOutput = 52,                        /*!< The FOC output for phase Alpha. */
    FOCBetaOutput = 53,                         /*!< The FOC output for phase Beta. */

    BusVoltage = 54,                            /*!< The bus voltage. */
    Temperature = 55,                           /*!< The temperature of the drive's output stage. */
    FoldbackEnergy = 68,                        /*!< The i2t total foldback energy. */

    LegACurrent = 69,                           /*!< Current1A reading */
    LegBCurrent = 70,                           /*!< Current1B reading */
    LegCCurrent = 71,                           /*!< Current1C reading */
    LegDCurrent = 72,                           /*!< Current1D reading */

    PWMOutputA = 75,                            /*!< Phase A PWM output */
    PWMOutputB = 76,                            /*!< Phase B PWM output (used with Brushless DC motors only) */
    PWMOutputC = 77,                            /*!< Phase C PWM output (used with Brushless DC and step motors only) */

    ContourOutput = 92,                         /*!< UDP active profile output based on current master source index and corresponding position in profile table. */
    ContourOffset = 93,                         /*!< UDP active profile offset (add to ContourOutput to calculate axis target position). */

    // Trace variables from this point on require custom mapping to actual register / bit / axis combinations.
    SignalStatus_EncoderA = 100,                /*!< Signal Status bit 0 on axis 1 */
    SignalStatus_EncoderB,                      /*!< Signal Status bit 1 on axis 1 */
    SignalStatus_EncoderIndex,                  /*!< Signal Status bit 2 on axis 1 */
    SignalStatus_CaptureInput,                  /*!< Signal Status bit 3 on axis 1 */
    SignalStatus_Gate,                          /*!< Signal Status bit 3 on axis 1 (alternative name for TOP use) */
    SignalStatus_EncoderHome,                   /*!< Signal Status bit 3 on axis 1 (treated as a flag) */
    SignalStatus_PositiveLimit,                 /*!< Signal Status bit 4 on axis 1 */
    SignalStatus_NegativeLimit,                 /*!< Signal Status bit 5 on axis 1 */
    SignalStatus_AxisIn,                        /*!< Signal Status bit 6 on axis 1 */
    SignalStatus_AxisOut,                       /*!< Signal Status bit 10 on axis 1 */
    SignalStatus_FaultOut,                      /*!< Signal Status bit 14 on axis 1 */

    SignalStatus_GPI1,                          /*!< Signal Status Encoder A (bit 0) on axis 2 */
    SignalStatus_GPI2,                          /*!< Signal Status Encoder B (bit 1) on axis 2 */
    SignalStatus_Trigger,                       /*!< Signal Status Trigger (bit 2 & 3) on axis 2. */

    ActivityStatus_PhasingInitialized = 1300,   /*!< Activity Status - PhasingInitialized (bit 0) */
    ActivityStatus_AtMaximumVelocity,           /*!< Activity Status - AtMaximumVelocity (bit 0) */
    ActivityStatus_Tracking,                    /*!< Activity Status - Tracking (bit 0) */
    ActivityStatus_CurrentProfileMode,          /*!< Activity Status - Current Profile Mode (bit 3-5) */
    ActivityStatus_Reserved = 1306,             /*!< Activity Status - Reserved */
    ActivityStatus_AxisSettled,                 /*!< Activity Status - Axis Settled (bit 7) */
    ActivityStatus_MotorOn,                     /*!< Activity Status - Position Loop Enabled (bit 8) */
    ActivityStatus_PositionCapture,             /*!< Activity Status - Position Capture (bit 9) */
    ActivityStatus_InMotion,                    /*!< Activity Status - In-motion (bit 10) */
    ActivityStatus_InPositiveLimit,             /*!< Activity Status - In Positive Limit (bit 11) */
    ActivityStatus_InNegativeLimit,             /*!< Activity Status - In Negative Limit (bit 12) */
    ActivityStatus_ProfileSegment = 1313,       /*!< Activity Status - Profile Segment (bit 13-15) */

    LAST_ENUM = 98
};

ENUMCLASS_FUNCTIONS_DECL( TraceVariable )

} //  namespace imp
