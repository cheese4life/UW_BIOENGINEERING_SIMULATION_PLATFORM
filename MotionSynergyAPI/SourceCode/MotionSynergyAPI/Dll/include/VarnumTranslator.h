/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "TypeTranslatorImpl.h"


namespace imp
{

/*!
 * @brief    Utility class used to convert between two @link Varnum @endlink types in either direction.
 *           This is useful when converting between types in different layers or
 *           converting from an external interface type to an internal implementation type.
 * @details  Performance will be slightly better converting from the first type to the second type.
 *
 * @tparam VarnumType1 The first  of the @link Varnum @endlink types to translate between.
 * @tparam VarnumType2 The second of the @link Varnum @endlink types to translate between.
 */
template<typename VarnumType1, typename VarnumType2>
class VarnumTranslator
{
public:
    typedef IPLKeystone::CommonServices::TypeTranslatorImpl<VarnumType1, VarnumType2> Translator;

    // static class
    VarnumTranslator() = delete;
    ~VarnumTranslator() = delete;

    /*!
     * @brief      Translates @p value to @link VarnumType2 @endlink .
     * @details    This method cannot be combined into a single template method with
     *             @link TranslateBack @endlink because both their input parameters
     *             are usually provided as un-scoped enumeration types that can be
     *             implicitly converted to both @link VarnumType1 @endlink and
     *             @link VarnumType2 @endlink , which results in an ambiguous
     *             method call being asserted by the compiler.
     * @param[in]  value  The @link Varnum @endlink to translate.
     * @returns    The translated @link Varnum @endlink object.
     * @throws     std::logic_error   If the translation map is empty.
     * @throws     std::out_of_range  If there is no valid translation for 'value' .
     */
    static VarnumType2
    Translate( VarnumType1 value )
    {
        return Translator::template Translate<VarnumType1, VarnumType2>( s_translationMap, value );
    }

    /*!
     * @brief      Translates @p value to @link VarnumType1 @endlink .
     * @details    This method cannot be combined into a single template method with
     *             @link Translate @endlink ; see @link Translate @endlink for more details.
     * @param[in]  value  The @link Varnum @endlink to translate.
     * @returns    The translated @link Varnum @endlink object.
     * @throws     std::logic_error   If the translation map is empty,
     *                                or the translation is non-unique.
     * @throws     std::out_of_range  If there is no valid translation for 'value' .
     */
    static VarnumType1
    TranslateBack( VarnumType2 value )
    {
        return Translator::template Translate<VarnumType2, VarnumType1>( s_translationMap, value );
    }

    /*!
     * @brief      As per @link Translate @endlink , but with a default return value.
     * @details    This method cannot be combined into a single template method with
     *             @link TranslateBackWithDefault @endlink ; see @link Translate @endlink for more details.
     * @param[in]  value         As per @link Translate @endlink .
     * @param[in]  defaultValue  The @link Varnum @endlink object to return if no valid translation exists.
     * @returns    As per @link Translate @endlink , except returns 'defaultValue' if no valid translation exists.
     * @throws     std::logic_error   As per @link Translate @endlink.
     */
    static VarnumType2
    TranslateWithDefault( VarnumType1 value, VarnumType2 defaultValue )
    {
        return Translator::TranslateWithDefault( s_translationMap, value, defaultValue );
    }

    /*!
     * @brief      As per @link TranslateBack @endlink , but with a default return value.
     * @details    This method cannot be combined into a single template method with
     *             @link TranslateWithDefault @endlink ; see @link Translate @endlink for more details.
     * @param[in]  value         As per @link TranslateBack @endlink .
     * @param[in]  defaultValue  The @link Varnum @endlink object to return if no valid translation exists.
     * @returns    As per @link TranslateBack @endlink , except returns 'defaultValue' if no valid translation exists.
     * @throws     std::logic_error   As per @link TranslateBack @endlink .
     */
    static VarnumType1
    TranslateBackWithDefault( VarnumType2 value, VarnumType1 defaultValue )
    {
        return Translator::TranslateWithDefault( s_translationMap, value, defaultValue );
    }

    /*!
     * @overload
     * @param[in]   value   As per @link Translate @endlink .
     * @param[out]  result  Will be set to the translated @link Varnum @endlink object if a valid translation exists.
     * @returns     true if a valid translation exists, false if no match could be found.
     * @throws      std::logic_error   As per @link Translate @endlink .
     */
    static bool
    Translate( VarnumType1 value, VarnumType2& result )
    {
        return Translator::Translate( s_translationMap, value, result );
    }

    /*!
     * @overload
     * @param[in]   value   As per @link TranslateBack @endlink .
     * @param[out]  result  Will be set to the translated @link Varnum @endlink object if a valid translation exists.
     * @returns     true if a valid translation exists, false if no match could be found.
     * @throws      std::logic_error   If the translation map is empty,
     *                                 or the translation is non-unique.
     */
    static bool
    TranslateBack( VarnumType2 value, VarnumType1& result )
    {
        return Translator::Translate( s_translationMap, value, result );
    }

    /*!
     * @brief      Registers a map of @link Varnum @endlink translations.
     * @details    In the same way that multiple enums are registered with a @link VarnumInfo @endlink type, it's
     *             expected that multiple sets of translation maps will be registered with a @link VarnumTranslator @endlink type.
     * @param[in]  translations  A map of @link Varnum @endlink translations.
     * @throws     std::runtime_error if a translation already exists in the translator.
     */
    static void
    RegisterTranslations( typename Translator::MapType translations )
    {
        for ( auto& entry : translations )
        {
            auto it = s_translationMap.find( entry.first );

            if ( it == s_translationMap.end() )
            {
                s_translationMap.insert( it, entry );
            }
            else
            {
                THROW_RUNTIME_ERROR( "VarnumTranslator: Value '" << entry.first << "' already exists." );
            }
        }
    }

private:
    static typename Translator::MapType s_translationMap;
};

template<typename VarnumType1, typename VarnumType2>
typename IPLKeystone::CommonServices::TypeTranslatorImpl<VarnumType1, VarnumType2>::MapType
    VarnumTranslator<VarnumType1, VarnumType2>::s_translationMap{};

} //  namespace imp
