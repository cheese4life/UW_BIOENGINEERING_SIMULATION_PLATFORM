/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertTypes.h"
#include "Varnum.h"

namespace imp
{

/*!
 * @brief A Varnum which holds an alert code.
 */
class RawAlertCode : public Varnum<RawAlertCode, ::Alert::AlertCode>
{
public:
    /*!
     * @brief Construct a RawAlertCode.
     * @param[in] value    The alert code this RawAlertCode represents.
     * @param[in] isStrict Flag indicating whether this instance will ensure that only valid values can be assigned.
     */
    RawAlertCode( ::Alert::AlertCode value = 1u, bool isStrict = true )
        : Varnum( value, isStrict )
    {
    }
};

} //  namespace imp
