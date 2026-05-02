/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <string>
#include <vector>

/*!
    @brief A useful macro for creating the declaration for the
           ToString(), FromString() and GetValues() methods.
*/
#define ENUM_FUNCTIONS_DECL \
    std::string ToString(Enum value); \
    Enum FromString(std::string const& stringValue); \
    std::vector<Enum> GetValues();

/*!
@brief A useful macro which adds RegisterVarnum() to the
       standard ENUM_FUNCTIONS_DECL methods.
*/
#define VARNUM_FUNCTIONS_DECL ENUM_FUNCTIONS_DECL \
void RegisterVarnum();

