/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/


#pragma once

#include "CommonAssert.h"
#include "CommonExceptions.h"
#include "EnumMapper.h"
#include "ToString.h"
#include <map>


namespace imp
{

// Forward declaration
template<typename TVarnumType, typename TValue>
class Varnum;

/*!
 * @brief A 'Variable Enum' type which allows valid enum values to be registered at runtime.
 *
 * @details The VarnumInfo class is a static class used to store the valid set of values and
 *          associated strings for a particular Varnum type.
 *          In order to register a set of values, they must first be defined using the EnumMapper class.
 *          Multiple sets of enum values can be registered with the VarnumInfo class (the whole point of the Varnum).
 * @tparam TVarnumType A class type representing a specific Varnum type.
 * @tparam TValue      A numeric type used to store the value (e.g. int32_t).
 */
template<typename TVarnumType, typename TValue>
class VarnumInfo
{
public:
    /*!
     * @brief    Register a set of EnumMapper values with this Varnum type.
     * @details  This method does not need to be called directly. Instead use the VARNUM_FUNCTION_IMPL macro.
     * @tparam TEnumTypeToRegister  The C++ enum type of the values being registered.
     * @throws   A std::runtime_error exception if any of the values being registered already exist
     * @throws   A std::logic_error exception if any of the values are too large for the numeric type.
     */
    template<typename TEnumTypeToRegister>
    static void
    RegisterValues()
    {
        auto pEntry = IPLKeystone::CommonServices::EnumMapper<TEnumTypeToRegister>::GetArray();
        std::size_t len = IPLKeystone::CommonServices::EnumMapper<TEnumTypeToRegister>::Size();

        for ( ; len > 0; --len, ++pEntry )
        {
            TValue value = pEntry->m_enumValue;
            ASSERT_MSG(
                value == pEntry->m_enumValue,
                "VarnumInfo::RegisterValues() - Value " << ToString( pEntry->m_enumValue ) << ", '"
                                                        << pEntry->m_enumString << "' truncated." );

            auto it = s_valueStringMap.find( value );

            if ( it == s_valueStringMap.end() )
            {
                s_valueStringMap.insert( it, std::make_pair( value, pEntry->m_enumString ) );
            }
            else
            {
                THROW_RUNTIME_ERROR(
                    "VarnumInfo: Value " << value << ", '" << pEntry->m_enumString << "' already exists." );
            }
        }
    }

private:
    friend class Varnum<TVarnumType, TValue>;

    static std::string
    ToString( TValue value )
    {
        auto it = s_valueStringMap.find( value );

        if ( it != s_valueStringMap.end() )
        {
            return it->second;
        }
        else
        {
            std::ostringstream ss;
            ss << "Unknown(" << value << ")";
            return ss.str();
        }
    }

    static TValue
    FromString( const std::string& stringValue )
    {
        for ( auto& entry : s_valueStringMap )
        {
            if ( entry.second == stringValue )
            {
                return entry.first;
            }
        }

        THROW_OUT_OF_RANGE( "VarnumInfo: Unknown string value '" << stringValue << "'." )
    }

    static bool
    IsValid( TValue value )
    {
        return s_valueStringMap.find( value ) != s_valueStringMap.end();
    }

    static bool
    IsValid( const std::string& stringValue )
    {
        for ( auto& entry : s_valueStringMap )
        {
            if ( entry.second == stringValue )
            {
                return true;
            }
        }

        return false;
    }

    struct ValueEntry
    {
        TValue m_value;
        std::string m_string;
    };

    typedef std::map<TValue, std::string> ValueContainer;

    static ValueContainer s_valueStringMap;
};

template<typename TVarnumType, typename TValue>
std::map<TValue, std::string> VarnumInfo<TVarnumType, TValue>::s_valueStringMap;

} //  namespace imp
