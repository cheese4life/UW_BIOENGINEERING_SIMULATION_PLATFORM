/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <string>

namespace imp
{
namespace AxisDeviceConfigurationKeys
{
/*! @brief Configuration key for Named Distances. */
const std::string NamedDistancesKey( "NamedDistances" );
/*! @brief Configuration key for Named Positions. */
const std::string NamedPositionsKey( "NamedPositions" );
/*! @brief Configuration key for Named Flags. */
const std::string NamedFlagsKey( "NamedFlags" );
/*! @brief Configuration key for list of distance names. */
const std::string DistanceListKey( "Distances" );
/*! @brief Configuration key for list of position names. */
const std::string PositionListKey( "Positions" );
/*! @brief Configuration key for Named Move Profiles. */
const std::string NamedMoveProfilesKey( "NamedMoveProfiles" );
/*! @brief Configuration key for Named Move Profile value Acceleration. */
const std::string AccelerationKey( "Acceleration" );
/*! @brief Configuration key for Named Move Profile value Deceleration. */
const std::string DecelerationKey( "Deceleration" );
/*! @brief Configuration key for Named Move Profile value Velocity. */
const std::string VelocityKey( "Velocity" );
/*! @brief Configuration key for Named Move Profile value Jerk. */
const std::string JerkKey( "Jerk" );
/*! @brief Configuration key for UDP profile csv file. */
const std::string UDPProfileKey("UDPProfile");
/*! @brief Configuration key for UDP profile the rate scalar. */
const std::string RateScalarKey("RateScalar");
/*! @brief Configuration key for UDP sine wave profile the frequency. */
const std::string FrequencyKey( "Frequency" );
/*! @brief Configuration key for UDP sine wave profile the amplitude. */
const std::string AmplitudeKey( "Amplitude" );
/*! @brief Configuration key for UDP sine wave profile the angular acceleration. */
const std::string AngularAccelerationKey( "AngularAcceleration" );
/*! @brief Configuration key for UDP profile indexes. */
const std::string UDPInputStreamFile("UDPInputStreamFile");
/*! @brief Configuration key for UDP mode selection. */
const std::string UDPModeKey( "UDPMode" );

/*! @brief Configuration key for Named RME Profile value PositiveCompensationData. */
const std::string PositiveCompensationDataKey( "PositiveCompensationData" );
/*! @brief Configuration key for Named RME Profile value NegativeCompensationData. */
const std::string NegativeCompensationDataKey( "NegativeCompensationData" );
/*! @brief Configuration key for Named RME Profile value StepSizeInCounts. */
const std::string StepSizeInCountsKey( "StepSizeInCounts" );
/*! @brief Configuration key for Named RME Profile value OffsetTimeInSamplePeriods. */
const std::string OffsetTimeInSamplePeriodsKey( "OffsetTimeInSamplePeriods" );
/*! @brief Configuration key for Named RME Profile value TriggerDelayNs. */
const std::string TriggerDelayNsKey( "TriggerDelayNs" );
/*! @brief Configuration key for Named RME Profile value TriggerPulseWidthNs. */
const std::string TriggerPulseWidthNsKey( "TriggerPulseWidthNs" );
/*! @brief Configuration key for Named RME Profile value Length. */
const std::string LengthKey( "Length" );
/*! @brief Configuration key for Named RME Profile value Data. */
const std::string DataKey( "Data" );

}; // namespace AxisDeviceConfigurationKeys
} // namespace imp
