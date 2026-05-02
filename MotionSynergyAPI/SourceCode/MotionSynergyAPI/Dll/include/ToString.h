/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "SmartString.h"

namespace imp
{

/*!
* @brief    Converts values to strings, handling the problem of small ints.
  @details  Performs default boolalpha for a bool; has special handling of pointers.
            Since we can't partially specialise functions, pass everything off to a set of classes
            with partial specialisation to handle things like pointers (eg null, char*, general)
* @returns  All overloads return a std::string.
*/
template<typename T>
std::string
ToString( T val )
{
    auto const stringVal = MakeSmartString( val );
    return stringVal.str();
}
} // namespace imp
