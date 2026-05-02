/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertSeverity.h"
#include "AlertTypes.h"

#include <chrono>
#include <iosfwd>
#include <memory>


namespace imp
{
class IConfiguration;
typedef std::shared_ptr<IConfiguration> IConfigurationSPtr;

class RawAlertCode;
class ReportedAlert;

typedef std::shared_ptr<ReportedAlert> ReportedAlertSPtr;
typedef std::shared_ptr<const ReportedAlert> ReportedAlertConstSPtr;

typedef std::chrono::system_clock::time_point AlertTimestamp;

class IAlertDistribution;
typedef std::shared_ptr<IAlertDistribution> IAlertDistributionSPtr;

std::ostream& operator<<( std::ostream& stream, const Alert::Severity::Enum& value );

} //  namespace imp
