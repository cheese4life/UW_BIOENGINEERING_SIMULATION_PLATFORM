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
 * @brief   Lowercase the string
 * @param [in]  source  The source string
 * @return  lowercased string
 */
std::string LowerCase( const std::string& source );

/*!
 * @brief   Uppercase the string
 * @param [in]  source  The source string
 * @return  upper cased string
 */
std::string UpperCase( const std::string& source );

} // namespace imp
