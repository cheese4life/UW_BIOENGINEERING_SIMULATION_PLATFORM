/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/


#pragma once

#include "CommonExceptions.h"
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
namespace Colour
{
    enum Enum
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
    ENUM_FUNCTIONS_DECL
}
@endcode

Use the macros defined below to generate an associated mapper (in Colour.cpp)

@code
#define ENUM_NAMESPACE IPLKeystone::CommonServices::Colour

DECLARE_ENUM_MAPPER(ColourEnumMapper)
{
    ENUM_STRING(Red),
    ENUM_STRING(Orange),
    ENUM_STRING(Yellow),
    ENUM_STRING(Green),
    ENUM_STRING(Blue),
    ENUM_STRING(Indigo),
    ENUM_STRING(Violet)
};

namespace IPLKeystone
{
    namespace CommonServices
    {
        namespace Colour
        {
            ENUM_FUNCTIONS_IMPL(ColourEnumMapper)
        }
    }
}

@endcode

* @tparam EnumType The enum type to generate a string mapper for.
*/
template<typename EnumType>
class EnumMapper
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
        std::size_t len = Size();

        for ( std::size_t n = 0; n != len; n++ )
        {
            if ( s_enumMap[ n ].m_enumValue == enumVal )
            {
                return s_enumMap[ n ].m_enumString;
            }
        }

        // If we get here then the enum value was not found
        //  TODO - This will report wrong values if the underlying type is larger than that of int
        THROW_OUT_OF_RANGE( "No such enum value=" << static_cast<int>( enumVal ) );
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
        THROW_OUT_OF_RANGE(
            "EnumMapper::toEnum() failed to convert string '" + std::string( enumString ) +
            "' to a valid enum value." );
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
    @brief Returns the number of enum values.

    @returns The number of enum values.
    */
    static std::size_t
    Size()
    {
        return sizeof( s_enumMap ) / sizeof( EnumMapEntry );
    }


    /*!
    @brief Returns a list of each enum value/string pair.

    @returns A list of each enum value/string pair.
    */
    static EnumMapEntry*
    GetArray()
    {
        return s_enumMap;
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
struct EnumValuesMissing;

template<>
struct EnumValuesMissing<true>
{
};

#define STATIC_CHECK( expr ) ( EnumValuesMissing<( expr ) != 0>() )

#define DECLARE_ENUM_MAPPER( mapper )                                             \
    typedef IPLKeystone::CommonServices::EnumMapper<ENUM_NAMESPACE::Enum> mapper; \
    template<>                                                                    \
    mapper::EnumMapEntry mapper::s_enumMap[] =

/*!
    @brief A useful macro for creating the implementation for the
    ToString(), FromString() and GetValues() methods.
*/
#define ENUM_FUNCTIONS_IMPL( mapper )                                                                                  \
    void DummyWrapper() { STATIC_CHECK( LAST_ENUM == sizeof( mapper::s_enumMap ) / sizeof( mapper::EnumMapEntry ) ); } \
    std::string ToString( Enum value ) { return mapper::ToString( value ); }                                           \
    Enum FromString( std::string const& stringValue ) { return mapper::ToEnum( stringValue.c_str() ); }                \
    std::vector<Enum> GetValues() { return mapper::ToValueList(); }

/*!
@brief A useful macro for creating the implementation to register this enum with a Varnum.
*/
#define VARNUM_FUNCTION_IMPL( varnuminfotype )      \
    void RegisterVarnum()                           \
    {                                               \
        static bool s_isRegistered = false;         \
                                                    \
        if ( !s_isRegistered )                      \
        {                                           \
            varnuminfotype::RegisterValues<Enum>(); \
            s_isRegistered = true;                  \
        }                                           \
    }
