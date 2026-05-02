/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <cstdint>
#include <string>


namespace imp
{

/*!
 * @brief  This templated class and its specialisations provide names for types.
 */
template<typename T>
struct TypenameTraits
{
    // If there is a compile error here, i.e.: "must return a value",
    // then there is a type, which has no template specialisation.
    // Provide a specialisation for that type and DO NOT fix the
    // compile error here.
};


//----------------------------------------------------------------------------
// TypenameTraits<> specialisations
//----------------------------------------------------------------------------

template<>
struct TypenameTraits<bool>
{
    static constexpr char const value[] = "bool";
};

template<>
struct TypenameTraits<uint8_t>
{
    static constexpr char const* value{ "uint8_t" };
};

template<>
struct TypenameTraits<int8_t>
{
    static constexpr char const* value{ "int8_t" };
};

template<>
struct TypenameTraits<uint16_t>
{
    static constexpr char const* value{ "uint16_t" };
};

template<>
struct TypenameTraits<int16_t>
{
    static constexpr char const* value{ "int16_t" };
};

template<>
struct TypenameTraits<uint32_t>
{
    static constexpr char const* value{ "uint32_t" };
};

template<>
struct TypenameTraits<int32_t>
{
    static constexpr char const* value{ "int32_t" };
};

template<>
struct TypenameTraits<uint64_t>
{
    static constexpr char const* value{ "uint64_t" };
};

template<>
struct TypenameTraits<int64_t>
{
    static constexpr char const* value{ "int64_t" };
};

template<>
struct TypenameTraits<float>
{
    static constexpr char const* value{ "float" };
};

template<>
struct TypenameTraits<double>
{
    static constexpr char const* value{ "double" };
};

template<>
struct TypenameTraits<std::string>
{
    static constexpr char const* value{ "string" };
};

template<>
struct TypenameTraits<std::wstring>
{
    static constexpr char const* value{ "wstring" };
};

} //  namespace imp
