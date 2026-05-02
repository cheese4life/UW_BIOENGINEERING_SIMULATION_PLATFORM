/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include "UDPMode.h"

#include <cstdint>
#include <ctime>
#include <sstream>
#include <string>
#include <vector>

namespace imp
{

class UDPProfile
{
public:
    std::string UDPInputStreamFile;

    double RateScalar;   //-32,768         to 32,767 + 65,535/65,536
    std::vector<double> DatastreamIndexInUnits;
    std::vector<double> DatastreamValuesInUnits;

    double SinewaveFrequency;
    double SinewaveAmplitude;
    double SinewaveAngularAcceleration;

    UDPMode UdpMode;
    std::time_t LastWriteTime;

    /*!
     *  @brief  Has the profile data on disk in the csv file changed since it was last loaded?
     *  @return true if the profile data on disk has changed, false if not or if UDP mode is sine wave.
     */
    bool HasProfileDataChanged() const;

    /*!
     *  @brief  Load the profile data from the csv file on disk, or do nothing if UDP mode is sine wave.
     *  @exceptions std::runtime_error if the file cannot be found, or parsing the csv file fails.
     */
    void LoadProfileData();

    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "UDPInputStreamFile=" << UDPInputStreamFile << ", ";

        ss << "UdpMode=" << UdpMode << ", ";
        ss << "RateScalar=" << RateScalar << ", ";

        ss << "SineWave Frequency=" << SinewaveFrequency << ", ";
        ss << "SineWave Amplitude=" << SinewaveAmplitude << ", ";
        ss << "SineWave AngularAcceleration=" << SinewaveAngularAcceleration << ", ";

        return ss.str();
    }

private:
    std::time_t GetLastWriteTime() const;
    bool IsDataFileToBeUsed() const;
};

} //  namespace imp
