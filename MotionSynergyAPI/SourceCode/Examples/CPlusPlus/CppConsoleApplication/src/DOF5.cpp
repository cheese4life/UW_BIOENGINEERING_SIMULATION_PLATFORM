/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "DOF5.h"

#include <IAxisDevice.h>
#include <LoggerMacros.h>

#include <boost/thread.hpp>
#include <chrono>

const std::string ComponentName( "DOF5" );

DOF5::DOF5( const std::string& productName, imp::IAxisDevice& axis )
    : Product( productName )
    , m_axis( axis )
{
}

void
DOF5::Initialize() const
{
    LOG_INFO( "Initializing " << Name() << "..." )

    m_axis.Initialize();

    LOG_INFO( "Homing " << Name() << "..." )

    Home();

    LOG_INFO( Name() << " is ready for use." )
}

void
DOF5::Home() const
{
    m_axis.EnterHomingMode();
    m_axis.Wait();

    // get HalfTravelFlag and assign it to a constant
    const bool isAtPositiveHalfFlag = m_axis.GetFlagStatus( "HomePositiveHalfFlag" );

    // Configure the Homing Move Profile for the axis
    m_axis.SelectMoveProfile( GetAxisProfileName( "HomingProfile" ) );
    m_axis.Wait();

    //Check the status of the Half Travel Flag to determine where you are
    //If Positive move negative direction until it changes
    if ( isAtPositiveHalfFlag )
    {
        LOG_INFO( Name() << " is in Positive Side." );
        m_axis.NMoveRelativeToFlag( "MaxNegativeHomingDistance", "HomeNegativeHalfFlag" );
        m_axis.Wait();
        //Move away from the flag to approach it from same direction
        m_axis.NMoveRelative( "MoveToCenterOffset" );
        m_axis.Wait();
    }
    // else move positive until it changes
    else
    {
        LOG_INFO( Name() << " is in Negative Side." );
    }
    m_axis.NMoveRelativeToFlag( "MaxPositiveHomingDistance", "HomePositiveHalfFlag" );
    m_axis.Wait();
    m_axis.NResetPosition( "Home" );
    m_axis.Wait();

    m_axis.ExitHomingMode();
    m_axis.Wait();
}

void
DOF5::PerformMoves() const
{
    LOG_INFO( "Performing a series of moves on the " << Name() << "..." )

    m_axis.SelectMoveProfile( GetAxisProfileName( "StandardMoveProfile" ) );
    m_axis.Wait();

    m_axis.NMoveAbsolute( "Home" );
    m_axis.Wait();

    const int32_t NumberOfMoves = 5;

    for ( int32_t i = 0; i < NumberOfMoves; i++ )
    {
        m_axis.MoveRelative( 0.5 );
        m_axis.Wait();
    }
    boost::this_thread::sleep_for( boost::chrono::milliseconds( 4000 ) );

    for ( int32_t i = NumberOfMoves; i >= 0; i-- )
    {
        m_axis.MoveRelative( -0.5 );
        m_axis.Wait();
    }
}

std::string
DOF5::GetAxisProfileName( const std::string& profileName ) const
{
    return m_axis.Name() + profileName;
}
