/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonExceptions.h"
#include "VarnumInfo.h"
#include <string>


namespace imp
{

/*!
 * @brief A 'Variable Enum' type which allows valid enum values to be registered at runtime.
 *
 * @details The Varnum is useful when there are a generic set of values related to a base type
 *          and a set of values specific to a derived type (e.g. event codes, alert codes,
 *          device response types, etc).
 *          The Varnum supports validation of values and conversion to/from strings.
 *          Each Varnum type must have an associated VarnumInfo class which stores the
 *          set of valid values and their associated string representation.
 * @tparam TVarnumType A class type representing a specific Varnum type.
 * @tparam TValue      A numeric type used to store the value (e.g. int32_t).
 */
template<typename TVarnumType, typename TValue>
class Varnum
{
public:
    /*!
     * @constructor Varnum constructor
     * @param[in] value      The initial value of this instance (default = 0).
     * @param[in] isStrict   Flag indicating whether this instance will ensure that only valid values can be assigned (default = true).
     */
    explicit Varnum( TValue value = 0, bool isStrict = true )
        : m_isStrict( isStrict )
        , m_value( value )
    {
        CheckValue( value );
    }

    /*!
     * @brief    Converts the current value to a string.
     * @returns  A string representation of the current value.
     */
    std::string
    ToString() const
    {
        return VarnumInfo<TVarnumType, TValue>::ToString( m_value );
    }

    /*!
     * @brief    Converts the string to a value and assigns the value to this instance.
     * @param[in] stringValue  The string value to convert and assign.
     * @throws   A std::out_of_range exception if the string does not match a valid value.
     */
    void
    FromString( const std::string& stringValue )
    {
        SetValue( VarnumInfo<TVarnumType, TValue>::FromString( stringValue ) );
    }

    /*!
     * @brief    Returns the current value as its underlying numeric type (e.g. int32_t).
     */
    operator TValue() const { return m_value; }

    /*!
     * @brief    Assignment operator. Note that only the value is assigned.
     *           The 'm_isStrict' flag is preserved.
     * @param[in] rhs  The Varnum instance to assign.
     * @returns  A reference to this Varnum instance.
     */
    Varnum&
    operator=( const Varnum& rhs )
    {
        if ( &rhs != this )
        {
            // This will preserve the 'm_isStrict' flag of this Varnum instance.
            SetValue( rhs );
        }

        return *this;
    }

    /*!
     * @brief    Test if the supplied value is valid for this Varnum type.
     * @param[in] value  The value to test.
     * @returns  True if the supplied value is valid, false if not.
     */
    bool
    IsValid( TValue value ) const
    {
        return VarnumInfo<TVarnumType, TValue>::IsValid( value );
    }

    /*!
     * @brief    Test if the supplied string value is valid for this Varnum type.
     * @param[in] stringValue  The string value to test.
     * @returns  True if the supplied string value is valid, false if not.
     */
    bool
    IsValid( const std::string& stringValue ) const
    {
        return VarnumInfo<TVarnumType, TValue>::IsValid( stringValue );
    }

    /*!
     * @brief    Test if the current value of this Varnum type is valid. This
     *           will always be true for Varnum instances where 'm_isStrict' is true.
     * @returns  True if the current value is valid, false if not.
     */
    bool
    IsValid() const
    {
        return IsValid( m_value );
    }

private:
    void
    SetValue( TValue value )
    {
        if ( m_value != value )
        {
            CheckValue( value );
            m_value = value;
        }
    }

    void
    CheckValue( TValue value )
    {
        if ( m_isStrict && !IsValid( value ) )
        {
            THROW_OUT_OF_RANGE( "Varnum: Value " << value << " is not a valid value." );
        }
    }

    bool m_isStrict;
    TValue m_value;
};

/*!
 * @brief    Streaming operator to automatically turn any Varnum into a string when output to a stream.
 */
template<typename TVarnumType, typename TValue>
std::ostream&
operator<<( std::ostream& stream, const Varnum<TVarnumType, TValue>& value )
{
    stream << value.ToString();
    return stream;
}

/*!
 * @brief    Streaming operator to automatically turn a string into a Varnum when read from a stream.
 */
template<typename TVarnumType, typename TValue>
std::istream&
operator>>( std::istream& stream, Varnum<TVarnumType, TValue>& value )
{
    std::string stringValue;
    stream >> stringValue;
    value.FromString( stringValue );
    return stream;
}

} //  namespace imp
