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
namespace DateTimeFormat
{
const std::string YearStringFormat = "%Y";
const std::string MonthStringFormat = "%m";
const std::string DayStringFormat = "%d";
const std::string HourStringFormat = "%H";
const std::string MinuteStringFormat = "%M";
const std::string SecondStringFormat = "%S";
const std::string MillisecondStringFormat = "%SSS";
const std::string UTCSuffix = "Z";
const std::string DefaultTimeStringFormat = YearStringFormat + "-" + MonthStringFormat + "-" + DayStringFormat + "T" +
                                            HourStringFormat + ":" + MinuteStringFormat + ":" + SecondStringFormat;
/*! @brief  The maximum buffer size for asctime_s output. */
const int MaxOutputBufferSize = 32;
} // namespace DateTimeFormat
} // namespace imp
