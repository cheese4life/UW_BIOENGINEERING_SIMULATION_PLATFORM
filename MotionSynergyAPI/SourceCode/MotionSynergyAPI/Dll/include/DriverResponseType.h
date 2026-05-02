/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "Varnum.h"


namespace imp
{

/*!
 * @brief A Varnum which holds a driver response.
 */
class DriverResponseType : public Varnum<DriverResponseType, int32_t>
{
public:
    DriverResponseType( int32_t const value = 0, bool const isStrict = true )
        : Varnum( value, isStrict )
    {
    }
};

} //  namespace imp
