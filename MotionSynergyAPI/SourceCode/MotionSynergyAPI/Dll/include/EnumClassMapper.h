/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/


#pragma once

#include "CommonExceptions.h"
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace IPLKeystone
{
namespace CommonServices
{
/*!
* @brief Utility class used to convert enum values to/from strings.


This template class should not be used directly. Instead, given an enum defined as follows (in Colour.h):

Note that LAST_ENUM is used to perform a compile-time check that the correct number of ENUM_STRING()
definitions exist. If the enum values do not start from zero or are not contiguous,
LAST_ENUM must be manually set (e.g. LAST_ENUM=7).

@code

enum class Colour : int32_t
{
    Red,
    Orange,
    Yellow,
    Green,
    Blue,
    Indigo,
    Violet,
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( Colour )

@endcode

Use the macros defined below to generate an associated mapper (in Colour.cpp)

@code
#define ENUM_NAMESPACE IPLKeystone::CommonServices::CommonServicesTest::Colour
DECLARE_ENUMCLASS_MAPPER( Colour )
{
ENUM_STRING( Red ),
ENUM_STRING( Orange ),
ENUM_STRING( Yellow ),
ENUM_STRING( Green ),
ENUM_STRING( Blue ),
ENUM_STRING( Indigo ),
ENUM_STRING( Violet )
};

namespace IPLKeystone
{
namespace CommonServices
{
namespace Colour
{
ENUMCLASS_FUNCTIONS_IMPL( Colour )
}
}
}

@endcode

* @tparam EnumType The enum type to generate a string mapper for.
*/
template<typename EnumType>
class EnumClassMapper
{
public:
    struct EnumMapEntry
    {
        EnumType m_enumValue;
        std::string m_enumString;
    };

    /*!
    @brief Returns the string represented by an enumeration.
    If there is more than one match, returns the one that has the lowest
    enumerated position (NOT the lowest value).

    @param enumVal the enumeration value to convert to a string

    @throws std::out_of_range If there is no valid mapping from the enum to the string
    */
    static std::string
    ToString( EnumType const& enumVal )
    {
        // Walk the list looking for the matching enum
        const std::size_t len = Size();

        for ( std::size_t n = 0; n != len; n++ )
        {
            if ( s_enumMap[ n ].m_enumValue == enumVal )
            {
                return s_enumMap[ n ].m_enumString;
            }
        }

        // If we get here then the enum value was not found
        std::ostringstream err;
        err << "No such enum value=" << static_cast<int64_t>( enumVal );
        THROW_OUT_OF_RANGE( err.str().c_str() );
    }

    /*!
    @brief Returns the string represented by an enumeration.
    If there is more than one match, returns the one that has
    the lowest enumerated position (NOT the lowest value).

    @param enumVal The enum value to convert
    @param isValid Will be set to false if there is no valid mapping from the enum to the string; true otherwise.
    */
    static std::string
    ToString( EnumType const& enumVal, bool& isValid )
    {
        isValid = true;

        try
        {
            return ToString( enumVal );
        }
        catch ( std::out_of_range const& )
        {
            isValid = false;
            return "";
        }
    }

    /*!
    @brief Returns the enumeration represented by a string
    If there is more than one match, returns the one that has
    the lowest enumerated position (NOT the lowest value).

    @param enumString the string to convert to an enum value.

    @throws std::out_of_range exception if there is no valid mapping from the string to the enum
    */
    static EnumType
    ToEnum( char const* enumString )
    {
        const std::size_t len = Size();

        // Walk the list looking for the matching string
        for ( std::size_t n = 0; n != len; n++ )
        {
            if ( s_enumMap[ n ].m_enumString == enumString )
            {
                return s_enumMap[ n ].m_enumValue;
            }
        }

        // if we get here then string was not found
        std::ostringstream err;
        err << "EnumClassMapper::toEnum() failed to convert string '" << std::string( enumString )
            << "' to a valid enum value.";
        THROW_OUT_OF_RANGE( err.str().c_str() );
    }

    /*!
    @brief Returns the enumeration represented by a string
    If there is more than one match, returns the one that has
    the lowest enumerated position (NOT the lowest value).

    @param value the string to convert
    @param isValid Will be set to false if there is no valid mapping from the string to the enum; true otherwise.
    */
    static EnumType
    ToEnum( std::string const& value, bool& isValid )
    {
        isValid = true;

        try
        {
            return ToEnum( value.c_str() );
        }
        catch ( std::out_of_range& )
        {
            isValid = false;
            return EnumType();
        }
    }

    /*!
    @brief Returns the enumeration represented by an integer value.
    If there is no matching enumeration, an exception will be thrown.

    @param value the integer to convert
    @throws std::out_of_range exception if there is no valid mapping from the integer to the enum
    */
    static EnumType
    ToEnum( int64_t value )
    {
        const std::size_t len = Size();

        // Walk the list looking for the matching value
        for ( std::size_t n = 0; n != len; n++ )
        {
            if ( static_cast<int64_t>( s_enumMap[ n ].m_enumValue ) == value )
            {
                return s_enumMap[ n ].m_enumValue;
            }
        }

        // if we get here then value was not found
        std::ostringstream err;
        err << "EnumClassMapper::toEnum() failed to convert value '" << value << "' to a valid enum value.";
        THROW_OUT_OF_RANGE( err.str().c_str() );
    }

    /*!
    @brief Returns the enumeration represented by an integer value.
    If there is no matching enumeration or the value is less than
    the minimum valid enum value, an exception will be thrown.

    @param value the integer to convert
    @param minValidValue the minimum valid enum value
    @throws std::out_of_range exception if there is no valid mapping from the integer to the enum or the value is less than the minimum valid enum value.
    */
    static EnumType
    ToEnum( int64_t const value, EnumType minValidValue )
    {
        EnumType validValue = ToEnum( value );

        if ( validValue >= minValidValue )
        {
            return validValue;
        }

        // if we get here then value was valid but < minValidValue
        std::ostringstream err;
        err << "EnumClassMapper::toEnum() value '" << ToString( validValue ) << "' is less than the minimum of '"
            << ToString( minValidValue ) << "'.";
        THROW_OUT_OF_RANGE( err.str().c_str() );
    }


    /*!
    @brief Returns a list of all valid enum values.

    @returns A vector containing all valid enum values.
    */
    static std::vector<EnumType>
    ToValueList()
    {
        // Build list of all enum values
        const std::size_t len = Size();
        std::vector<EnumType> values;
        values.reserve( len );

        for ( std::size_t n = 0; n < len; n++ )
        {
            values.push_back( s_enumMap[ n ].m_enumValue );
        }

        return values;
    }

    /*!
    @brief Returns the collection of values in this mapper as string-to-value pairs.

    @returns The string-to-value map.
    */
    static std::map<std::string, EnumType>
    ToStringValueMap()
    {
        // Build list of all enum string-to-value pairs (using the std::transform function)
        std::map<std::string, EnumType> stringToValueMap;
        std::transform(
            std::begin( s_enumMap ),
            std::end( s_enumMap ),
            std::inserter( stringToValueMap, stringToValueMap.end() ),
            []( const EnumMapEntry& entry ) { return std::make_pair( entry.m_enumString, entry.m_enumValue ); } );

        return stringToValueMap;
    }

    /*!
    @brief Returns the number of enum values.

    @returns The number of enum values.
    */
    static std::size_t
    Size()
    {
        return sizeof( s_enumMap ) / sizeof( EnumMapEntry );
    }

    static EnumMapEntry s_enumMap[];
};
} // namespace CommonServices
} // namespace IPLKeystone

/*!
@brief  A useful macro for defining the enum map - see below
*/
#define ENUM_STRING( arg )        \
    {                             \
        ENUM_NAMESPACE::arg, #arg \
    }

template<bool>
struct EnumClassValuesMissing;

template<>
struct EnumClassValuesMissing<true>
{
};

#define ENUMCLASS_STATIC_CHECK( expr ) ( EnumClassValuesMissing<( expr ) != 0>() )

#define DECLARE_ENUMCLASS_MAPPER( EnumType )                                                        \
    typedef IPLKeystone::CommonServices::EnumClassMapper<ENUM_NAMESPACE> EnumType##EnumClassMapper; \
    template<>                                                                                      \
    EnumType##EnumClassMapper::EnumMapEntry EnumType##EnumClassMapper::s_enumMap[] =

/*!
@brief A useful macro for creating the implementation for the
streaming output operator,
ToString(), FromString(), GetValues() and Size() methods.
Note that streaming and GetValues() is not supported on
embedded platforms due to library requirements (streams)
or dynamic memory allocation (std:vector<>).
*/
#define ENUMCLASS_FUNCTIONS_IMPL( EnumType )                                                                          \
    std::ostream& operator<<( std::ostream& stream, const ENUM_NAMESPACE& value )                                     \
    {                                                                                                                 \
        stream << EnumType##EnumClassMapper::ToString( value );                                                       \
        return stream;                                                                                                \
    }                                                                                                                 \
    namespace EnumType##Enum                                                                                          \
    {                                                                                                                 \
        void DummyWrapper()                                                                                           \
        {                                                                                                             \
            ENUMCLASS_STATIC_CHECK(                                                                                   \
                static_cast<size_t>( ENUM_NAMESPACE::LAST_ENUM ) ==                                                   \
                sizeof( EnumType##EnumClassMapper::s_enumMap ) / sizeof( EnumType##EnumClassMapper::EnumMapEntry ) ); \
        }                                                                                                             \
        std::string ToString( ENUM_NAMESPACE value ) { return EnumType##EnumClassMapper::ToString( value ); }         \
        ENUM_NAMESPACE FromString( std::string const& stringValue )                                                   \
        {                                                                                                             \
            return EnumType##EnumClassMapper::ToEnum( stringValue.c_str() );                                          \
        }                                                                                                             \
        ENUM_NAMESPACE FromInt( int64_t value ) { return EnumType##EnumClassMapper::ToEnum( value ); }                \
        ENUM_NAMESPACE FromInt( int64_t value, ENUM_NAMESPACE minValidValue )                                         \
        {                                                                                                             \
            return EnumType##EnumClassMapper::ToEnum( value, minValidValue );                                         \
        }                                                                                                             \
        std::vector<ENUM_NAMESPACE> GetValues() { return EnumType##EnumClassMapper::ToValueList(); }                  \
        std::map<std::string, ENUM_NAMESPACE> GetStringValueMap()                                                     \
        {                                                                                                             \
            return EnumType##EnumClassMapper::ToStringValueMap();                                                     \
        }                                                                                                             \
        std::size_t Size() { return EnumType##EnumClassMapper::Size(); }                                              \
    }
