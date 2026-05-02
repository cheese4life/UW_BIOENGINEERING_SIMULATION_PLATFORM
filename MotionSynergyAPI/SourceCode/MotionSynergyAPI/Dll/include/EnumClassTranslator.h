/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "TypeTranslatorImpl.h"

namespace IPLKeystone
{
namespace CommonServices
{
/*!
* @brief    Utility class used to convert between two enum types in either direction.
*           This is useful when converting between types in different layers or
*           converting from an external interface type to an internal implementation type.
* @details  Performance will be slightly better converting from the first type to the second type.

@code

// Use the following macro to define the mapping between enum values.
// This must be done in a .cpp file in the IPLKeystone::CommonServices namespace.

namespace IPLKeystone
{
namespace CommonServices
{
DECLARE_ENUM_TRANSLATOR( GreenColour, RedColour )
{
    { GreenColour::Light,  RedColour::Light  },
    { GreenColour::Medium, RedColour::Medium },
    { GreenColour::Dark,   RedColour::Dark   },
};
} // namespace CommonServices
} // namespace IPLKeystone

// Perform a translation. An exception will be thrown if there is no match.
auto toRedValue   = EnumClassTranslator<GreenColour,RedColour>::Translate(GreenColour::Light);
auto toGreenValue = EnumClassTranslator<GreenColour,RedColour>::Translate(RedColour::Light);

// Perform a translation, returning true/false to indicate the success of the operation.
RedColour toRedValue;
bool toRedResult   = EnumClassTranslator<GreenColour,RedColour>::Translate(GreenColour::Teal, toRedValue);
GreenColour toGreenValue;
bool toGreenResult = EnumClassTranslator<GreenColour,RedColour>::Translate(RedColour::Burgundy, toGreenValue);

// Perform a translation, returning the supplied default value if there is no match.
auto toRedValue   = EnumClassTranslator<GreenColour,RedColour>::TranslateWithDefault(GreenColour::Light, RedColour::Light);
auto toGreenValue = EnumClassTranslator<GreenColour,RedColour>::TranslateWithDefault(RedColour::Light, GreenColour::Light);

@endcode

* @tparam EnumType1 The first  of the enum types to translate between.
* @tparam EnumType2 The second of the enum types to translate between.
*/
template<typename EnumType1, typename EnumType2>
class EnumClassTranslator
{
public:
    typedef TypeTranslatorImpl<EnumType1, EnumType2> Translator;

    // static class
    EnumClassTranslator() = delete;
    ~EnumClassTranslator() = delete;

    /*!
     * @brief      Translates @p value to the other template type.
     * @details    This wrapper method facilitates automatic type deduction.
     * @param[in]  value  The object to translate from.
     * @returns    The translated enum object.
     * @throws     std::logic_error   If the translation map is empty.
     * @throws     std::out_of_range  If there is no valid translation for 'value'.
     */
    static EnumType2
    Translate( EnumType1 value )
    {
        return Translator::template Translate<EnumType1, EnumType2>( s_enumMap, value );
    }

    /*!
     * @overload
     * @throws     std::logic_error   If the translation map is empty,
     *                                or the translation is non-unique.
     * @throws     std::out_of_range  As per @link Translate @endlink .
     */
    static EnumType1
    Translate( EnumType2 value )
    {
        return Translator::template Translate<EnumType2, EnumType1>( s_enumMap, value );
    }

    /*!
     * @brief      As per @link Translate @endlink , but with a default return value.
     * @param[in]  value         As per @link Translate @endlink .
     * @param[in]  defaultValue  The object to return if no valid translation exists.
     * @returns    As per @link Translate @endlink , except returns 'defaultValue' if no valid translation exists.
     * @throws     std::logic_error   If the translation map is empty.
     */
    static EnumType2
    TranslateWithDefault( EnumType1 value, EnumType2 defaultValue )
    {
        return Translator::TranslateWithDefault( s_enumMap, value, defaultValue );
    }

    /*!
     * @overload
     * @throws     std::logic_error   If the translation map is empty, or
     *                                the translation is non-unique.
     */
    static EnumType1
    TranslateWithDefault( EnumType2 value, EnumType1 defaultValue )
    {
        return Translator::TranslateWithDefault( s_enumMap, value, defaultValue );
    }

    /*!
     * @overload
     * @param[in]   value   As per @link Translate @endlink .
     * @param[out]  result  Will be set to the translated object if a valid translation exists.
     * @returns     true if a valid translation exists, false if no match could be found.
     * @throws      std::logic_error   If the translation map is empty.
     */
    static bool
    Translate( EnumType1 value, EnumType2& result )
    {
        return Translator::Translate( s_enumMap, value, result );
    }

    /*!
     * @overload
     * @param[in]   value   As per @link Translate @endlink .
     * @param[out]  result  Will be set to the translated object if a valid translation exists.
     * @returns     true if a valid translation exists, false if no match could be found.
     * @throws      std::logic_error   If the translation map is empty,
     *                                 or the translation is non-unique.
     */
    static bool
    Translate( EnumType2 value, EnumType1& result )
    {
        return Translator::Translate( s_enumMap, value, result );
    }

    static typename Translator::MapType s_enumMap;
};

} // namespace CommonServices
} // namespace IPLKeystone

// Convenience macro used to initialize the std::map<EnumType1, EnumType2>
// to define the enum translation table used by the EnumClassTranslator.
#define DECLARE_ENUM_TRANSLATOR( EnumType1, EnumType2 ) \
    template<>                                          \
    std::map<EnumType1, EnumType2> IPLKeystone::CommonServices::EnumClassTranslator<EnumType1, EnumType2>::s_enumMap =
