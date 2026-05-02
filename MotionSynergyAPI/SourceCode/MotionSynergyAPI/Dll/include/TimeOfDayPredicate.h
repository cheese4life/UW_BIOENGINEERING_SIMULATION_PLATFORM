/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonAssert.h"
#include "SafeFunctions.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <chrono>


namespace imp
{

/*!
 *   @brief  Constructed with a time of day, each call of the function object returns whether
 *           that time has now passed (and the time then bumps forward to the next day)
 */
class TimeOfDayPredicate
{
public:
    /*!
     *   @brief  Note, hour/min/sec constructed from plain int, since applied to the C struct tm.
     */
    TimeOfDayPredicate( int hour, int minute, int second = 0 ) { SetTime( hour, minute, second ); }

    /*!
     *   @brief  Constructs from a time string of the format hh:mm
     */
    explicit TimeOfDayPredicate( std::string const& time )
    {
        std::vector<std::string> timepieces;
        boost::split( timepieces, time, boost::is_any_of( ":" ) );
        ASSERT( timepieces.size() >= 2 );
        auto const hour = boost::lexical_cast<int>( timepieces[ 0 ] );
        auto const minute = boost::lexical_cast<int>( timepieces[ 1 ] );
        int second = 0;

        if ( timepieces.size() >= 3 )
        {
            second = boost::lexical_cast<int>( timepieces[ 2 ] );
        }

        SetTime( hour, minute, second );
    }


    /*!
     *   @brief  Returns true if called after the constructed time; bumps time forward to the next day.
     *   @details Time after the call is always in the future, no matter how long since last called.
     */
    bool
    operator()()
    {
        if ( std::chrono::system_clock::now() > m_nextTime )
        {
            AdvanceToFuture();
            return true;
        }

        return false;
    }

private:
    //  bumps the next time in day increments until it's in the future.
    void
    AdvanceToFuture()
    {
        while ( std::chrono::system_clock::now() > m_nextTime )
        {
            m_nextTime += std::chrono::hours( 24 );
        }
    }
    void
    SetTime( int hour, int minute, int second = 0 )
    {
        //  chrono now, convert to C time, modify to desired hour/min, convert back to chrono
        auto const now = std::chrono::system_clock::now();
        time_t const tnow = std::chrono::system_clock::to_time_t( now );
        tm buf;
        tm* const pDate = SafeFunctions::localtime_s( &tnow, &buf );
        pDate->tm_hour = hour;
        pDate->tm_min = minute;
        pDate->tm_sec = second;
        m_nextTime = std::chrono::system_clock::from_time_t( std::mktime( pDate ) );

        //  in case that's in the past now, bump it forward a day
        AdvanceToFuture();
    }


private:
    std::chrono::time_point<std::chrono::system_clock> m_nextTime;
};

} // namespace imp
