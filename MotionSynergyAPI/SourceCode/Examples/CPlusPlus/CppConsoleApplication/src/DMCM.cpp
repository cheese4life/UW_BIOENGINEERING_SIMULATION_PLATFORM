/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "DMCM.h"

#include <IAxisDevice.h>
#include <LoggerMacros.h>

#include <boost/thread.hpp>
#include <chrono>

const std::string ComponentName( "DMCM" );

DMCM::DMCM( const std::string& productName, imp::IAxisDevice& axis )
    : Product( productName )
    , m_axis( axis )
{
}

void
DMCM::Initialize() const
{
    LOG_INFO( "Initializing " << Name() << "..." )

    m_axis.Initialize();

    LOG_INFO( "Homing " << Name() << "..." )

    Home();

    LOG_INFO( Name() << " is ready for use." )
}

void
DMCM::Home() const
{
    m_axis.EnterHomingMode();
    m_axis.Wait();

    // Configure the Homing Move Profile for the axis
    m_axis.SelectMoveProfile( GetAxisProfileName( "HomingProfile" ) );
    m_axis.Wait();

    // If Positive move negative direction until it changes
    const bool isAtNegativeLimit = m_axis.GetFlagStatus( "NegativeLimit" );
    if ( !isAtNegativeLimit )
    {
        LOG_INFO( "Moving to the Negative Limit." );
        m_axis.NMoveRelativeToFlag( "MaxNegativeHomingDistance", "NegativeLimit" ); //move to negative limit
        m_axis.Wait();
    }
    // Move to Index
    LOG_INFO( "At Negative Limit." );
    LOG_INFO( "Moving to Encoder Index Flag." );
    m_axis.NMoveRelativeToFlag( "MaxPositiveHomingDistance", "EncoderIndex" );
    m_axis.Wait();

    LOG_INFO( "At Index Flag." );
    m_axis.NResetPosition( "Home" );
    m_axis.Wait();

    m_axis.ExitHomingMode();
    m_axis.Wait();
}

void
DMCM::PerformMoves() const
{
    LOG_INFO( "Performing a series of moves on the " << Name() << "..." )

    m_axis.SelectMoveProfile( GetAxisProfileName( "StandardMoveProfile" ) );
    m_axis.Wait();

    // Move to starting position
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
DMCM::GetAxisProfileName( const std::string& profileName ) const
{
    return m_axis.Name() + profileName;
}
