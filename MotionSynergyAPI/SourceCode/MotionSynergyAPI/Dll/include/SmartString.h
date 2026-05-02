/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "NamedPointer.h"
#include <cstdint>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <vector>


// Note: Thread is not supported in Managed C++
#if !( ( _MANAGED == 1 ) || ( _M_CEE == 1 ) )
    #include <thread>
#endif

namespace imp
{

/*!
 * @brief Convert an integral value to a hex string, padded with zeros and sized to the appropriate width.
 */
template<typename T>
std::string
ToHexString( T val )
{
    static_assert( std::is_integral<T>::value, "T must be an integral type" );

    // calculate number of hex digits
    constexpr std::streamsize hexdigits =
        ( ( std::numeric_limits<T>::digits - 1 ) / ( std::numeric_limits<uint8_t>::digits >> 1 ) ) + 1;

    // determine value casting type - need to up-cast 8-bit integrals to 16-bit integrals to ensure that they are
    // not treated as "characters" when using the stream operators
    using CastType = typename std::conditional<
        sizeof( uint8_t ) < sizeof( T ),
        T,
        typename std::conditional<std::is_signed<T>::value, int16_t, uint16_t>::type>::type;

    // convert to HEX string
    std::ostringstream os;
    os << "0x" << std::setfill( '0' ) << std::setw( hexdigits ) << std::hex << static_cast<CastType>( val );
    return os.str();
}

/*!
 * @brief Convert a sequence of integral values to a hex string, padded with zeros and sized to the appropriate width.
 */
template<typename T>
std::string
ToHexString( const T* values, size_t valuesLength, const std::string& separator = " " )
{
    static_assert( std::is_integral<T>::value, "T must be an integral type" );

    // calculate number of hex digits
    constexpr std::streamsize hexdigits =
        ( ( std::numeric_limits<T>::digits - 1 ) / ( std::numeric_limits<uint8_t>::digits >> 1 ) ) + 1;

    // determine value casting type - need to up-cast 8-bit integrals to 16-bit integrals to ensure that they are
    // not treated as "characters" when using the stream operators
    using CastType = typename std::conditional<
        sizeof( uint8_t ) < sizeof( T ),
        T,
        typename std::conditional<std::is_signed<T>::value, int16_t, uint16_t>::type>::type;

    // convert to HEX string sequence
    std::ostringstream os;
    for ( size_t ix = 0; ( values != nullptr ) && ( ix != valuesLength ); ix++ )
    {
        if ( ix != 0 )
        {
            os << separator;
        }
        os << "0x" << std::setfill( '0' ) << std::setw( hexdigits ) << std::hex
           << static_cast<CastType>( *( values + ix ) );
    }

    return os.str();
}

/*!
 * @brief Convert a sequence of integral values to a hex string, padded with zeros and sized to the appropriate width.
 */
template<typename T>
std::string
ToHexString( const std::vector<T>& values, const std::string& separator = " " )
{
    return ToHexString( values.data(), values.size(), separator );
}

/*!
 * @brief The SmartString classes are a more fine-grained, lower level version of ToString
 *        They exist to better distinguish between inputs, as functions (eg ToString) can't partially specialise
 *        All types not explicitly handled by the partial specialisations are stringified as per standard
 *        streaming operators on a stringstream.
 */
template<typename T>
class SmartString
{
public:
    explicit SmartString( const T& val ) { m_stream << val; }


    std::string
    str() const
    {
        return m_stream.str();
    }

private:
    std::ostringstream m_stream;
};

/*!
 * @brief   Generic pointers undergo NamedPointer stringification
 */
template<typename T>
class SmartString<T*>
{
public:
    explicit SmartString( T const* val ) { m_stream << NamedPointer( val ); }

    std::string
    str() const
    {
        return m_stream.str();
    }

private:
    std::ostringstream m_stream;
};


/*!
 * @brief   Shared pointers undergo NamedPointer stringification
 */
template<typename T>
class SmartString<std::shared_ptr<T>>
{
public:
    explicit SmartString( const std::shared_ptr<T>& val ) { m_stream << NamedPointer( val.get() ); }

    std::string
    str() const
    {
        return m_stream.str();
    }

private:
    std::ostringstream m_stream;
};


/*!
 * @brief   Weak pointers undergo NamedPointer stringification
 */
template<typename T>
class SmartString<std::weak_ptr<T>>
{
public:
    explicit SmartString( const std::weak_ptr<T>& val ) { m_stream << NamedPointer( val.lock().get() ); }

    std::string
    str() const
    {
        return m_stream.str();
    }

private:
    std::ostringstream m_stream;
};


/*!
 * @brief   Unique pointers undergo NamedPointer stringification
 */
template<typename T>
class SmartString<std::unique_ptr<T>>
{
public:
    explicit SmartString( const std::unique_ptr<T>& val ) { m_stream << NamedPointer( val.get() ); }

    std::string
    str() const
    {
        return m_stream.str();
    }

private:
    std::ostringstream m_stream;
};


/*!
 * @brief   String literals (pointers to characters) are treated as a string, unless a nullptr
 */
template<>
class SmartString<char const*>
{
public:
    explicit SmartString( char const* val )
    {
        if ( val == nullptr )
        {
            m_str = NamedPointer( nullptr );
        }
        else
        {
            m_str = val;
        }
    }

    std::string
    str() const
    {
        return m_str;
    }

private:
    std::string m_str;
};


/*!
 * @brief   Null pointers are treated as named pointers
 */
template<>
class SmartString<decltype( nullptr )>
{
public:
    explicit SmartString( decltype( nullptr ) )
        : m_str( NamedPointer( nullptr ) )
    {
    }

    std::string
    str() const
    {
        return m_str;
    }

private:
    std::string m_str;
};


/*!
 * @brief   Small ints are treated as integers, not characters
 */
template<>
class SmartString<int8_t>
{
public:
    explicit SmartString( int8_t const& val )
        : m_str( std::to_string( val ) )
    {
    }

    std::string
    str() const
    {
        return m_str;
    }

private:
    std::string m_str;
};

/*!
 * @brief   Small ints are treated as integers, not characters
 */
template<>
class SmartString<uint8_t>
{
public:
    explicit SmartString( uint8_t const& val )
        : m_str( std::to_string( static_cast<uint32_t>( val ) ) )
    {
    }

    std::string
    str() const
    {
        return m_str;
    }

private:
    std::string m_str;
};


/*!
 * @brief   Boolean values are treated like default boolalpha
 */
template<>
class SmartString<bool>
{
public:
    explicit SmartString( bool const& val )
        : m_str( val ? "true" : "false" )
    {
    }

    std::string
    str() const
    {
        return m_str;
    }

private:
    std::string m_str;
};


/*!
 * @brief   Given a string, return the same string.
 */
template<>
class SmartString<std::string>
{
public:
    explicit SmartString( std::string const& val )
        : m_str( val )
    {
    }

    std::string
    str() const
    {
        return m_str;
    }

private:
    std::string m_str;
};


#if !( ( _MANAGED == 1 ) || ( _M_CEE == 1 ) )

/*!
 * @brief   Output a threadid in exactly the same format as output by the logging infrastructure.
 * @details <thread> is not supported in Managed C++
 */
template<>
class SmartString<std::thread::id>
{
public:
    explicit SmartString( std::thread::id const id )
    {
        m_stream << "0x" << std::setfill( '0' ) << std::setw( 8 ) << std::right << std::hex << id;
    }

    std::string
    str() const
    {
        return m_stream.str();
    }

private:
    std::ostringstream m_stream;
};

#endif


/*!
 *   @brief  Helper function: use this to construct a SmartString, to avoid specifying template arguments (ala make_pair etc)
 */
template<typename T>
SmartString<T>
MakeSmartString( const T& val )
{
    return SmartString<T>( val );
}

} // namespace imp
