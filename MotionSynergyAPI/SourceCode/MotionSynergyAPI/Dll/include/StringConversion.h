/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonExceptions.h"

#include <boost/algorithm/string.hpp>
#include <string>
#include <type_traits>


namespace imp
{

namespace StringConversion
{

/*!
 * @brief    General conversion function from source type T to a string
 * @details  This function will convert the source type into a string using the default
 *           streaming operator provided by the Standard C++ library.
 *           Target types that require different types of conversion (because the above
 *           procedure would invalidate the output) require specialised conversion
 *           functions.
 */
template<typename T>
static std::string
ToString( const T& value )
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

/*!
 * @brief    General conversion function from a string to target type T
 * @details  This function primarily targets the conversion of strings to integral and
 *           boolean types. It will strip all leading and trailing whitespace around
 *           the source string, and then convert it to lower case before parsing is
 *           attempted.
 *           Target types that require different types of conversion (because the above
 *           procedure would invalidate the parsing process) require specialised
 *           conversion functions.
 */
template<typename T>
static T
FromString( const std::string& strValue )
{
    std::stringstream strTest;
    // Enable exceptions and strip whitespace around the value
    strTest.exceptions( std::stringstream::badbit | std::stringstream::failbit );
    strTest << boost::to_lower_copy( boost::trim_copy( strValue ) );

#ifdef _MSC_VER
    #pragma warning( push )
    #pragma warning( disable : 4127 ) // silence warnings about constant conditional expressions
#endif

    // detect if a negative value is attempted to be parsed into an unsigned integral type
    if ( std::is_integral<T>::value && std::is_unsigned<T>::value && !strValue.empty() && strValue[ 0 ] == '-' )
    {
        THROW_OUT_OF_RANGE( "Attempted to parse negative value into an unsigned type" );
    }

#ifdef _MSC_VER
    #pragma warning( pop )
#endif

    T retVal;

    try
    {
        // This is important to make sure that
        // boolean is casted correctly
        // std::setbase(0) sets a prefix-dependent parse which supports hex numbers as 0x...
        strTest >> std::boolalpha >> std::setbase( 0 ) >> retVal;
    }
    catch ( std::exception& ex )
    {
        // recast all exceptions as out-of-range
        THROW_OUT_OF_RANGE( "Unable to cast to target type: " + std::string( ex.what() ) );
    }

    return retVal;
}

} // namespace StringConversion

/*!
 * @brief    Template specialisation for string
 * @details  As opposed to the ToString<T> template that will attempt to
 *           cast to the target type via a stream, return the string value
 *           as-is (preserving whitespace).
 */
template<>
inline std::string
StringConversion::ToString( const std::string& value )
{
    return value;
}

/*!
 * @brief    Template specialisation for booleans
 * @details  As opposed to the ToString<T> template that will convert a boolean to
 *           the string equivalents of 0 and 1, return "false" and "true" instead
 */
template<>
inline std::string
StringConversion::ToString( const bool& value )
{
    return value ? std::string( "true" ) : std::string( "false" );
}

/*!
 * @brief    Template specialisation for signed 8-bit integer types
 * @details  Stream redirect operator attempts to parse 8-bit types as a char
 *           instead of a value.
 */
template<>
inline std::string
StringConversion::ToString( const int8_t& value )
{
    return ToString<int16_t>( static_cast<int16_t>( value ) );
}

/*!
 * @brief    Template specialisation for unsigned 8-bit integer types
 * @details  Stream redirect operator attempts to parse 8-bit types as a char
 *           instead of a value.
 */
template<>
inline std::string
StringConversion::ToString( const uint8_t& value )
{
    return ToString<uint16_t>( static_cast<uint16_t>( value ) );
}

/*!
 * @brief    Template specialisation for strings
 * @details  As opposed to the FromString<T> template that will attempt to
 *           cast to the target type via a stream, return the string value
 *           as-is (preserving whitespace).
 */
template<>
inline std::string
StringConversion::FromString( const std::string& strValue )
{
    return strValue;
}

/*!
 * @brief    Template specialisation for single-precision floating point types
 * @details  As opposed to the FromString<T> template that will attempt to
 *           cast to the target type via a stream, attempt to perform a
 *           lexical cast from string to float. The special strings "inf"
 *           and "-inf" are interpreted as positive and negative infinity,
 *           exponent notation values are allowed (like "1.234e5"), and
 *           the "NaN" is parsed as a not-a-number value
 */
template<>
inline float
StringConversion::FromString( const std::string& strValue )
{
    try
    {
        return boost::lexical_cast<float>( strValue );
    }
    catch ( boost::bad_lexical_cast& )
    {
        THROW_OUT_OF_RANGE( "Could not cast the value \"" + strValue + "\" to a float" );
    }
}

/*!
 * @brief    Template specialisation for double-precision floating point types
 * @details  As opposed to the FromString<T> template that will attempt to
 *           cast to the target type via a stream, attempt to perform a
 *           lexical cast from string to float. The special strings "inf"
 *           and "-inf" are interpreted as positive and negative infinity,
 *           exponent notation values are allowed (like "1.234e5"), and
 *           the "NaN" is parsed as a not-a-number value
 */
template<>
inline double
StringConversion::FromString( const std::string& strValue )
{
    try
    {
        return boost::lexical_cast<double>( strValue );
    }
    catch ( boost::bad_lexical_cast& )
    {
        THROW_OUT_OF_RANGE( "Could not cast the value \"" + strValue + "\" to a double" );
    }
}

/*!
 * @brief    Template specialisation for signed 8-bit integer types
 * @details  Stream redirect operator attempts to parse 8-bit types as a char
 *           instead of a value.
 */
template<>
inline int8_t
StringConversion::FromString( const std::string& strValue )
{
    // cast it to a larger type first, then compare against tighter numeric limits
    const int16_t value = FromString<int16_t>( strValue );

    // Note the notation for the use of numeric_limits. Particularly the location
    // of the parenthesis. This is done to prevent a clash with windows.h macro
    // definitions for min/max when including this file on a project built
    // under Windows.
    if ( value < ( std::numeric_limits<int8_t>::min )() || value > ( std::numeric_limits<int8_t>::max )() )
    {
        THROW_OUT_OF_RANGE( "Value \"" + strValue + "\" is outside of int8_t range" );
    }

    return static_cast<int8_t>( value );
}

/*!
 * @brief    Template specialisation for unsigned 8-bit integer types
 * @details  Stream redirect operator attempts to parse 8-bit types as a char
 *           instead of a value.
 */
template<>
inline uint8_t
StringConversion::FromString( const std::string& strValue )
{
    // cast it to a larger type first, then compare against tighter numeric limits
    const uint16_t value = FromString<uint16_t>( strValue );

    // Note the notation for the use of numeric_limits. Particularly the location
    // of the parenthesis. This is done to prevent a clash with windows.h macro
    // definitions for min/max when including this file on a project built
    // under Windows.
    if ( value < ( std::numeric_limits<uint8_t>::min )() || value > ( std::numeric_limits<uint8_t>::max )() )
    {
        THROW_OUT_OF_RANGE( "Value \"" + strValue + "\" is outside of uint8_t range" );
    }

    return static_cast<uint8_t>( value );
}

} //  namespace imp
