/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CachedValue.h"
#include "IUDP.h"

namespace imp
{
class AxisDevice;

/*!
 *  @brief   Provides the implementation for performing User Defined Profile (UDP) moves and updating the associated user defined profile data.
 */
class UserDefinedProfile : public IUserDefinedProfile
{
public:
    UserDefinedProfile( AxisDevice& axis );
    uint32_t GetCurrentDataStreamIndex() override;
    void MoveRelativeUDP( uint32_t repeat ) override;
    void UpdateProfileData( uint32_t index, double position ) override;
    void SelectSineWaveMoveProfile( double frequencyHz, double amplitude, double angularAcceleration ) override;

private:
    AxisDevice& m_axis;
    CachedValue<uint32_t> m_currentDataStreamIndex;
};

} //  namespace imp
