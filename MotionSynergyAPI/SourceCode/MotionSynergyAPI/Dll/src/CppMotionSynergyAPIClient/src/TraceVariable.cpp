/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "TraceVariable.h"
#include "EnumClassMapper.h"

#define ENUM_NAMESPACE imp::TraceVariable

// clang-format off

DECLARE_ENUMCLASS_MAPPER( TraceVariable )
{
    ENUM_STRING(None),
    ENUM_STRING(PositionError),
    ENUM_STRING(CommandedPosition),
    ENUM_STRING(CommandedVelocity),
    ENUM_STRING(CommandedAcceleration),
    ENUM_STRING(ActualPosition),
    ENUM_STRING(ActualVelocity),
    ENUM_STRING(ActiveMotorCommand),
    ENUM_STRING(MotionControlICTime),
    ENUM_STRING(PositionCaptureRegister),

    ENUM_STRING(PositionLoopIntegratorSum),
    ENUM_STRING(PositionLoopIntegratorContribution),
    ENUM_STRING(PositionLoopDerivative),
    ENUM_STRING(Biquad1Input),
    ENUM_STRING(Biquad2Input),

    ENUM_STRING(EventStatusRegister),
    ENUM_STRING(ActivityStatusRegister),
    ENUM_STRING(SignalStatusRegister),
    ENUM_STRING(DriveStatusRegister),
    ENUM_STRING(DriveFaultStatusRegister),

    ENUM_STRING(PhaseAngle),
    ENUM_STRING(PhaseOffset),
    ENUM_STRING(PhaseACommand),
    ENUM_STRING(PhaseBCommand),
    ENUM_STRING(PhaseCCommand),

    ENUM_STRING(AnalogInput0),
    ENUM_STRING(AnalogInput1),
    ENUM_STRING(AnalogInput2),
    ENUM_STRING(AnalogInput3),
    ENUM_STRING(AnalogInput4),
    ENUM_STRING(AnalogInput5),
    ENUM_STRING(AnalogInput6),
    ENUM_STRING(AnalogInput7),
    ENUM_STRING(PhaseAngleScaled),

    ENUM_STRING(PhaseAReference),
    ENUM_STRING(PhaseAError),
    ENUM_STRING(PhaseAActualCurrent),
    ENUM_STRING(PhaseAIntegratorSum),
    ENUM_STRING(PhaseAIntegratorContribution),
    ENUM_STRING(CurrentLoopAOutput),

    ENUM_STRING(PhaseBReference),
    ENUM_STRING(PhaseBError),
    ENUM_STRING(PhaseBActualCurrent),
    ENUM_STRING(PhaseBIntegratorSum),
    ENUM_STRING(PhaseBIntegratorContribution),
    ENUM_STRING(CurrentLoopBOutput),

    ENUM_STRING(FOCDReference),
    ENUM_STRING(FOCDError),
    ENUM_STRING(FOCDFeedback),
    ENUM_STRING(FOCDIntegratorSum),
    ENUM_STRING(FOCDIntegratorContribution),
    ENUM_STRING(FOCDOutput),

    ENUM_STRING(FOCQReference),
    ENUM_STRING(FOCQError),
    ENUM_STRING(FOCQFeedback),
    ENUM_STRING(FOCQIntegratorSum),
    ENUM_STRING(FOCQIntegratorContribution),
    ENUM_STRING(FOCQOutput),

    ENUM_STRING(FOCAlphaOutput),
    ENUM_STRING(FOCBetaOutput),

    ENUM_STRING(BusVoltage),
    ENUM_STRING(Temperature),
    ENUM_STRING(FoldbackEnergy),

    ENUM_STRING(LegACurrent),
    ENUM_STRING(LegBCurrent),
    ENUM_STRING(LegCCurrent),
    ENUM_STRING(LegDCurrent),

    ENUM_STRING(PWMOutputA),
    ENUM_STRING(PWMOutputB),
    ENUM_STRING(PWMOutputC),

    ENUM_STRING(ContourOutput),
    ENUM_STRING(ContourOffset),


    ENUM_STRING(SignalStatus_EncoderA),
    ENUM_STRING(SignalStatus_EncoderB),
    ENUM_STRING(SignalStatus_EncoderIndex),
    ENUM_STRING(SignalStatus_CaptureInput),
    ENUM_STRING(SignalStatus_Gate),
    ENUM_STRING(SignalStatus_EncoderHome),
    ENUM_STRING(SignalStatus_PositiveLimit),
    ENUM_STRING(SignalStatus_NegativeLimit),
    ENUM_STRING(SignalStatus_AxisIn),
    ENUM_STRING(SignalStatus_AxisOut),
    ENUM_STRING(SignalStatus_FaultOut),

    ENUM_STRING(SignalStatus_Trigger),
    ENUM_STRING(SignalStatus_GPI1),   
    ENUM_STRING(SignalStatus_GPI2),

    ENUM_STRING(ActivityStatus_PhasingInitialized),
    ENUM_STRING(ActivityStatus_AtMaximumVelocity), 
    ENUM_STRING(ActivityStatus_Tracking),
    ENUM_STRING(ActivityStatus_CurrentProfileMode),
    ENUM_STRING(ActivityStatus_Reserved),
    ENUM_STRING(ActivityStatus_AxisSettled),
    ENUM_STRING(ActivityStatus_MotorOn),      
    ENUM_STRING(ActivityStatus_PositionCapture),
    ENUM_STRING(ActivityStatus_InMotion),  
    ENUM_STRING(ActivityStatus_InPositiveLimit),
    ENUM_STRING(ActivityStatus_InNegativeLimit),  
    ENUM_STRING(ActivityStatus_ProfileSegment)
};

namespace imp
{

ENUMCLASS_FUNCTIONS_IMPL( TraceVariable )

} //  namespace imp
