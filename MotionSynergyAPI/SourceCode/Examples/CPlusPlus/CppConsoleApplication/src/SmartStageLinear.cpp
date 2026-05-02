/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "SmartStageLinear.h"

#include <IAxisDevice.h>
#include <LoggerMacros.h>

const std::string ComponentName( "SmartStageLinear" );

SmartStageLinear::SmartStageLinear( const std::string& productName, imp::IAxisDevice& axis )
    : Product( productName )
    , m_axis( axis )
{
}

void
SmartStageLinear::Initialize() const
{
    LOG_INFO( "Initializing " << Name() << "..." )

    m_axis.Initialize();

    LOG_INFO( "Homing " << Name() << "..." )

    Home_Center();

    LOG_INFO( Name() << " is ready for use." )
}

void
SmartStageLinear::Home_Center() const
{
    m_axis.EnterHomingMode();
    m_axis.Wait();

    m_axis.SelectMoveProfile( GetAxisProfileName( "HomingProfile" ) );
    m_axis.Wait();

    const bool isAtPositiveHalf = m_axis.GetFlagStatus( "PositiveHalf" );
    const bool isAtPositiveLimit = m_axis.GetFlagStatus( "PositiveLimit" );

    // If already on negative limit, move off the negative limit
    if ( isAtPositiveHalf || isAtPositiveLimit )
    {
        m_axis.NMoveRelativeToFlag( "MaxNegativeHomingDistance", "NegativeHalf" );
        m_axis.Wait();
        m_axis.NMoveRelative( "MoveToCenterOffset" );
        m_axis.Wait();
    }

    m_axis.NMoveRelativeToFlag( "MaxPositiveHomingDistance", "PositiveHalf" );
    m_axis.Wait();

    m_axis.NResetPosition( "Home" );
    m_axis.Wait();

    m_axis.ExitHomingMode();
    m_axis.Wait();
}

void
SmartStageLinear::Home_NegativeLimit() const
{
    m_axis.EnterHomingMode();
    m_axis.Wait();

    m_axis.SelectMoveProfile( GetAxisProfileName( "HomingProfile" ) );
    m_axis.Wait();

    const bool isAtNegativeLimit = m_axis.GetFlagStatus( "NegativeLimit" );

    // If already on negative limit, move off the negative limit
    if ( isAtNegativeLimit )
    {
        m_axis.NMoveRelative( "OffLimitMove" );
        m_axis.Wait();
    }

    m_axis.NMoveRelativeToFlag( "MaxNegativeHomingDistance", "NegativeLimit" );
    m_axis.Wait();

    m_axis.NMoveRelative( "NegativeLimitOffset" );
    m_axis.Wait();

    m_axis.NResetPosition( "Home" );
    m_axis.Wait();

    m_axis.ExitHomingMode();
    m_axis.Wait();
}
void
SmartStageLinear::Home_PositiveLimit() const
{
    m_axis.EnterHomingMode();
    m_axis.Wait();

    m_axis.SelectMoveProfile( GetAxisProfileName( "HomingProfile" ) );
    m_axis.Wait();

    const bool isAtPositiveLimit = m_axis.GetFlagStatus( "PositiveLimit" );

    // If already on negative limit, move off the negative limit
    if ( isAtPositiveLimit )
    {
        m_axis.NMoveRelative( "OffLimitMove" );
        m_axis.Wait();
    }

    m_axis.NMoveRelativeToFlag( "MaxPositiveHomingDistance", "PositiveLimit" );
    m_axis.Wait();

    m_axis.NMoveRelative( "PositiveLimitOffset" );
    m_axis.Wait();

    m_axis.NResetPosition( "Home" );
    m_axis.Wait();

    m_axis.ExitHomingMode();
    m_axis.Wait();
}

void
SmartStageLinear::PerformMoves() const
{
    LOG_INFO( "Performing a series of moves on the " << Name() << "..." )

    m_axis.SelectMoveProfile( GetAxisProfileName( "StandardMoveProfile" ) );
    m_axis.Wait();

    // Move to starting position - this assumes the stage has been homed to the center.
    m_axis.MoveAbsolute( -45 );

    m_axis.Wait();

    constexpr int32_t countX = 8;
    for ( int32_t j = 0; j < countX; j++ )
    {
        m_axis.MoveRelative( 5 );
        m_axis.Wait();
    }
}

std::string
SmartStageLinear::GetAxisProfileName( const std::string& profileName ) const
{
    return m_axis.Name() + profileName;
}
