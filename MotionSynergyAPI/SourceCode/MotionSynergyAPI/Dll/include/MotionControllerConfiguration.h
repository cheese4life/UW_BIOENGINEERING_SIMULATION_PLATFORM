/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CachedValue.h"
#include "IMotionControllerConfig.h"

namespace imp
{
class AxisDevice;

/*!
 *  @brief   Provides the implementation for reading and writing various motion controller settings from/to memory/NVRAM.
 */
class MotionControllerConfiguration : public IMotionControllerConfiguration
{
public:
    MotionControllerConfiguration( AxisDevice& axis );

    void Restore( std::string pathname ) override;
    void Save( std::string pathname ) override;
    void RestoreToNVRAM( std::string pathname ) override;
    MotionTrackingSettings GetMotionTracking() override;
    void SetMotionTracking( const MotionTrackingSettings& motionTrackingSettings ) override;
    MotionErrorSettings GetMotionError() override;
    void SetMotionError( const MotionErrorSettings& motionErrorSettings ) override;
    PositionLoopSettings GetPositionLoop() override;
    void SetPositionLoop( const PositionLoopSettings& positionLoopSettings ) override;
    CurrentLoopSettings GetCurrentLoop() override;
    void SetCurrentLoop( const CurrentLoopSettings& currentLoopSettings ) override;
    BiquadFilterSettings GetBiquadFilters() override;
    void SetBiquadFilters( const BiquadFilterSettings& biquadFilterSettings ) override;
    void WriteToNVRAM( std::vector<MotionControllerConfigurationType> configurationTypes ) override;
    CommunicationSettings GetCommunication() override;
    void SetCommunication( const CommunicationSettings& communicationSettings ) override;
    FirmwareVersionInfo GetFirmwareVersion() override;
    PhaseAngleInfo GetPhaseAngle() override;
    void SetPhaseAngle( const uint16_t rawPhaseAngle ) override;
    int32_t GetCommutationParameter1() override;
    void SetCommutationParameter1( const int32_t commutationParameter1 ) override;
    ElectricalProperties GetElectricalProperties() override;

private:
    AxisDevice& m_axis;
    CachedValue<MotionTrackingSettings> m_motionTrackingSettings;
    CachedValue<MotionErrorSettings> m_motionErrorSettings;
    CachedValue<PositionLoopSettings> m_positionLoopSettings;
    CachedValue<CurrentLoopSettings> m_currentLoopSettings;
    CachedValue<BiquadFilterSettings> m_biquadFilterSettings;
    CachedValue<CommunicationSettings> m_communicationSettings;
    CachedValue<FirmwareVersionInfo> m_firmwareVersionInfo;
    CachedValue<PhaseAngleInfo> m_phaseAngleInfo;
    CachedValue<int32_t> m_commutationParameter1;
    CachedValue<ElectricalProperties> m_electricalProperties;
};

} //  namespace imp
