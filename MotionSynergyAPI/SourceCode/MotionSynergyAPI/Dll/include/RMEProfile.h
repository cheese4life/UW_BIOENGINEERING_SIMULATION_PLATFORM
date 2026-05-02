/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace imp
{

class RMEProfile
{
public:
    std::vector<int32_t> PositiveCompensationData;
    std::vector<int32_t> NegativeCompensationData;
    uint32_t StepSizeInCounts;
    uint32_t OffsetTimeInSamplePeriods;
    uint32_t TriggerDelayNs;
    uint32_t TriggerPulseWidthNs;

    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "PositiveCompensationData.Length=" << PositiveCompensationData.size() << ", ";
        ss << "NegativeCompensationData.Length=" << NegativeCompensationData.size() << ", ";
        ss << "StepSizeInCounts=" << StepSizeInCounts << ", ";
        ss << "OffsetTimeInSamplePeriods=" << OffsetTimeInSamplePeriods << ", ";
        ss << "TriggerDelayNs=" << TriggerDelayNs << ", ";
        ss << "TriggerPulseWidthNs=" << TriggerPulseWidthNs;

        return ss.str();
    }
};

} //  namespace imp
