/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonExceptions.h"
#include <mutex>


namespace imp
{

template<class T>
class CachedValue
{
public:
    explicit CachedValue( std::mutex& mutex )
        : m_firstUpdateReceived( false )
        , m_data()
        , m_mutex( mutex )
    {
    }
    explicit CachedValue( std::mutex& mutex, const T& data )
        : m_firstUpdateReceived( true )
        , m_data( data )
        , m_mutex( mutex )
    {
    }
    virtual ~CachedValue() = default;

    void
    SetValue( T const& value )
    {
        std::unique_lock<std::mutex> lock( m_mutex );
        m_firstUpdateReceived = true;
        m_data = value;
    }
    T
    GetValue() const
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        if ( !m_firstUpdateReceived )
        {
            THROW_LOGIC_ERROR( "First update of cached value not yet received" );
        }

        return m_data;
    }
    T
    GetValueOrDefault( const T& defaultValue )
    {
        std::unique_lock<std::mutex> lock( m_mutex );

        if ( !m_firstUpdateReceived )
        {
            return defaultValue;
        }

        return m_data;
    }
    bool
    IsValid() const
    {
        std::unique_lock<std::mutex> lock( m_mutex );
        return m_firstUpdateReceived;
    }
    void
    Reset()
    {
        std::unique_lock<std::mutex> lock( m_mutex );
        m_firstUpdateReceived = false;
    }
    T
    operator()() const
    {
        return GetValue();
    }
    CachedValue&
    operator=( T const& value )
    {
        SetValue( value );
        return *this;
    }

private:
    bool m_firstUpdateReceived;
    T m_data;
    std::mutex& m_mutex;
};

/*!
 *  @brief  Checks whether the cached value object is valid for use.
 *          ie: First update was received.
 *  @param[in]  obj A CachedValue object to check the validity.
 *  @return     True if the object is valid and False otherwise.
 */
template<class T>
bool
IsCachedValueValid( CachedValue<T> const& obj )
{
    return obj.IsValid();
}

} //  namespace imp
