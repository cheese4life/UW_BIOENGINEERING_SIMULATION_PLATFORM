/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonExceptions.h"

#include <map>
#include <sstream>

namespace IPLKeystone
{
namespace CommonServices
{
/*!
 * @brief    Provides shared implementation for various other translator classes.
 * @details  Performance will be slightly better converting from the first type to the second type.
 * @tparam   Type1  The   primary type of the translator.
 * @tparam   Type2  The secondary type of the translator.
 */
template<typename Type1, typename Type2>
class TypeTranslatorImpl
{
public:
    typedef std::map<Type1, Type2> MapType;

    // static class
    TypeTranslatorImpl() = delete;
    ~TypeTranslatorImpl() = delete;

    /*!
     * @brief      Translates in either direction between the types.
     * @param[in]  map    The translation map.
     * @param[in]  value  The type to translate from.
     * @returns    The translated object of the other type.
     * @throws     std::logic_error   If 'map' is empty,
     *                                or the translation is non-unique.
     * @throws     std::out_of_range  If no translation exists for 'value'.
     */
    template<typename FromType, typename ToType>
    static ToType
    Translate( const MapType& map, FromType value )
    {
        ToType result;

        if ( Translate( map, value, result ) )
        {
            return result;
        }

        std::ostringstream ostr;
        ostr << "Unable to translate from type '" << typeid( FromType ).name() << "', value '"
             << static_cast<int64_t>( value ) << "' to type '" << typeid( ToType ).name() << "'.";
        THROW_OUT_OF_RANGE( ostr.str() );
    }

    /*!
     * @brief      As per @link Translate @endlink , but with a default return value.
     * @param[in]  map           As per @link Translate @endlink .
     * @param[in]  value         As per @link Translate @endlink .
     * @param[in]  defaultValue  The object to return if no translation exists.
     * @returns    As per @link Translate @endlink , except returns 'defaultValue' if no valid translation exists.
     * @throws     std::logic_error   As per @link Translate @endlink .
     */
    template<typename FromType, typename ToType>
    static ToType
    TranslateWithDefault( const MapType& map, FromType value, ToType defaultValue )
    {
        ToType result;

        if ( Translate( map, value, result ) )
        {
            return result;
        }

        return defaultValue;
    }

    /*!
     * @overload
     * @param[in]   map     As per @link Translate @endlink .
     * @param[in]   value   As per @link Translate @endlink .
     * @param[out]  result  The translated object of the other type.
     * @returns     Whether a valid translation was found and output.
     * @throws      std::logic_error   If 'map' is empty.
     */
    static bool
    Translate( const MapType& map, Type1 value, Type2& result )
    {
        AssertNotEmpty( map );

        auto it = map.find( value );

        if ( it != map.end() )
        {
            result = it->second;
            return true;
        }

        return false;
    }

    /*!
     * @overload
     * @param[in]   map     As per @link Translate @endlink .
     * @param[in]   value   As per @link Translate @endlink .
     * @param[out]  result  The translated object of the other type.
     * @returns     Whether a valid translation was found and output.
     * @throws      std::logic_error   If 'map' is empty,
     *                                 or the translation is non-unique.
     */
    static bool
    Translate( const MapType& map, Type2 value, Type1& result )
    {
        AssertNotEmpty( map );

        const auto predicate = [ value ]( const typename MapType::value_type& pair ) { return pair.second == value; };

        auto it = std::find_if( map.begin(), map.end(), predicate );

        if ( it != map.end() )
        {
            auto dupIt = it;

            // check for non-unique translation
            if ( std::find_if( ++dupIt, map.end(), predicate ) != map.end() )
            {
                THROW_LOGIC_ERROR( "Translation failed - mapping for value " << value << " is non-unique!" );
            }

            result = it->first;
            return true;
        }

        return false;
    }

private:
    static void
    AssertNotEmpty( const MapType& map )
    {
        if ( map.empty() )
        {
            THROW_LOGIC_ERROR(
                "Translation map empty! Map must first be initialized through declaration or registration." );
        }
    }
};

} // namespace CommonServices
} // namespace IPLKeystone
