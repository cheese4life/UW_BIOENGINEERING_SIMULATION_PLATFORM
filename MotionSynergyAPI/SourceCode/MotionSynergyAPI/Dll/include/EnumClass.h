/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <string>
#include <vector>   // Required for GetValues().
#include <map>      // Required for GetStringValueMap().

/*!
@brief A useful macro for creating the declaration for the
streaming output operator,
ToString(), FromString(), GetValues() and Size() methods.
Note that streaming and GetValues() is not supported on
embedded platforms due to library requirements (streams)
or dynamic memory allocation (std:vector<>).
*/
#define ENUMCLASS_FUNCTIONS_DECL(EnumType) \
    std::ostream & operator<<(std::ostream& stream, const EnumType& value); \
    namespace EnumType##Enum { \
    std::string ToString(EnumType value); \
    EnumType FromString(std::string const& stringValue); \
    EnumType FromInt(int64_t value); \
    EnumType FromInt(int64_t value, EnumType minValidValue); \
    std::vector<EnumType> GetValues(); \
    std::map<std::string, EnumType> GetStringValueMap(); \
    std::size_t Size(); \
    }

