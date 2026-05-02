/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonAssert.h"
#include <string>


namespace imp
{

/*!
 *   @brief  Given a string, returns a copy without any surrounding quotes.
 *           Accepts quoted and Unquoted strings. Error on input of mismatched quotes.
 */
inline std::string
Unquoted( std::string const& val )
{
    if ( val.length() > 1 )
    {
        if ( val[ 0 ] == '\"' )
        {
            ASSERT_MSG( val[ val.length() - 1 ] == '\"', "String has mismatched quotes: " + val );
            return val.substr( 1, val.length() - 2 );
        }
    }

    ASSERT_MSG( val != "\"", "String is but a single quote" );

    return val;
}

} // namespace imp
