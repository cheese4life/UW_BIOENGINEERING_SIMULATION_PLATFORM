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

/*!
 * @brief   Replaces the first matched substring of a target string with a replacement string.
 * @param [in,out] target The string to modify if a match is found
 * @param [in] match The substring in target to replace
 * @param [in] replacement The string to replace match with
 */
inline void
ReplaceString( std::string& target, const std::string& match, const std::string& replacement )
{
    auto offset = target.find( match );

    if ( offset != std::string::npos )
    {
        target.replace( offset, match.length(), replacement );
    }
}

} // namespace imp
