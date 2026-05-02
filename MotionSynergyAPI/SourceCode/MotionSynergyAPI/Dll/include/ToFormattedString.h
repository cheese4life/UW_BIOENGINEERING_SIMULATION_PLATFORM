/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonAssert.h"
#include "DefaultOutputFormats.h"
#include "DefaultValueMaps.h"
#include <map>
#include <string>
namespace imp
{
/*
 * The maximum formatted string size
 */
const int MAX_SMART_FORMATTED_SIZE = 1024;

/*
 * @brief Convert input value to formatted string with given format. The maximum length is @link MAX_SMART_FORMATTED_SIZE @endlink.
 */
template<
    typename T,
    std::enable_if_t<!std::is_same<T, std::string>::value && !std::is_same<T, float>::value, bool> = true>
std::string
Snprintf( const T& val, const std::string& format )
{
    ASSERT_ARG_VALID_MSG( !format.empty(), "Format is empty." );
    char buf[ MAX_SMART_FORMATTED_SIZE ];
#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
    std::snprintf( buf, MAX_SMART_FORMATTED_SIZE, format.c_str(), val );
#ifdef __GNUC__
    #pragma GCC diagnostic pop
#endif
    return std::string( buf );
}

std::string
Snprintf( const std::string& val, const std::string& format )
{
    ASSERT_ARG_VALID_MSG( !format.empty(), "Format is empty." );
    char buf[ MAX_SMART_FORMATTED_SIZE ];
#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
    std::snprintf( buf, MAX_SMART_FORMATTED_SIZE, format.c_str(), val.c_str() );
#ifdef __GNUC__
    #pragma GCC diagnostic pop
#endif
    return std::string( buf );
}

std::string
Snprintf( const float& val, const std::string& format )
{
    // snprintf doesn't support float type.
    return Snprintf<double>( static_cast<double>( val ), format );
}


/*!
 * @brief      Converts values to formatted strings
 * @details    Covert the values to formatted strings with given format. If the value is defined in the value map,
               it will be converted to the matching string in given format. Otherwise, the value will be converted
               to the string in given format.
 * @param[in]  val The value to be converted
 * @param[in]  format If the value is not found in value map, it will be converted to string with this format
 * @param[in]  valueMap Specific strings the value shall be converted to
 * @param[in]  mapingValueFormat The format of the mapping string
 * @returns    All overloads return a std::string.
 */

template<typename T>
std::string
ToFormattedString(
    const T& val,
    const std::string& format,
    const std::map<T, std::string>& valueMap,
    const std::string& mapingValueFormat )
{
    std::string result = "";
    if ( valueMap.find( val ) != valueMap.end() )
    {
        result = Snprintf( valueMap.at( val ), mapingValueFormat );
    }
    else
    {
        result = Snprintf( val, format );
    }
    return result;
}

template<typename T, std::enable_if_t<std::is_integral<T>::value && !std::is_same<T, bool>::value, bool> = true>
std::string
ToFormattedString( const T& val )
{
    return ToFormattedString(
        val,
        DefaultOutputFormats::DefaultIntOutputFormats,
        std::map<T, std::string>{},
        DefaultOutputFormats::DefaultStringOutputFormats );
}

template<typename T, std::enable_if_t<std::is_integral<T>::value && !std::is_same<T, bool>::value, bool> = true>
std::string
ToFormattedString( const T& val, const std::string& format )
{
    return ToFormattedString(
        val,
        format,
        std::map<T, std::string>{ {} },
        DefaultOutputFormats::DefaultStringOutputFormats );
}

template<typename T, std::enable_if_t<std::is_same<T, bool>::value, bool> = true>
std::string
ToFormattedString( const T& val )
{
    return ToFormattedString(
        val,
        DefaultOutputFormats::DefaultBoolOutputFormats,
        DefaultBoolValueMap,
        DefaultOutputFormats::DefaultStringOutputFormats );
}

template<typename T, std::enable_if_t<std::is_same<T, bool>::value, bool> = true>
std::string
ToFormattedString( const T& val, const std::map<T, std::string>& valueMap )
{
    return ToFormattedString(
        val,
        DefaultOutputFormats::DefaultBoolOutputFormats,
        valueMap,
        DefaultOutputFormats::DefaultStringOutputFormats );
}

template<typename T, std::enable_if_t<std::is_same<T, bool>::value, bool> = true>
std::string
ToFormattedString( const T& val, const std::map<T, std::string>& valueMap, const std::string& valueMapFormat )
{
    return ToFormattedString( val, DefaultOutputFormats::DefaultBoolOutputFormats, valueMap, valueMapFormat );
}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
std::string
ToFormattedString( const T& val )
{
    return ToFormattedString(
        val,
        DefaultOutputFormats::DefaultFloatingPointOutputFormats,
        std::map<T, std::string>{},
        DefaultOutputFormats::DefaultStringOutputFormats );
}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
std::string
ToFormattedString( const T& val, const std::string& format )
{
    return ToFormattedString(
        val,
        format,
        std::map<T, std::string>{},
        DefaultOutputFormats::DefaultStringOutputFormats );
}

template<typename T, std::enable_if_t<std::is_same<T, std::string>::value, bool> = true>
std::string
ToFormattedString( const T& val )
{
    return ToFormattedString(
        val,
        DefaultOutputFormats::DefaultStringOutputFormats,
        std::map<T, std::string>{},
        DefaultOutputFormats::DefaultStringOutputFormats );
}

template<typename T, std::enable_if_t<std::is_same<T, std::string>::value, bool> = true>
std::string
ToFormattedString( const T& val, const std::string& format )
{
    return ToFormattedString(
        val,
        format,
        std::map<T, std::string>{},
        DefaultOutputFormats::DefaultStringOutputFormats );
}
} // namespace imp
