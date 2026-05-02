/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertTypes.h"
#include "VarnumInfo.h"


namespace imp
{

class RawAlertCode;

/*!
 * @brief A VarnumInfo which holds an alert code.
 */
class RawAlertCodeInfo : public VarnumInfo<RawAlertCode, Alert::AlertCode>
{
public:
    RawAlertCodeInfo() = delete;
};

} //  namespace imp
