/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "SmartStageXY.h"

#include <IAxisDevice.h>
#include <LoggerMacros.h>

const std::string ComponentName( "SmartStageXY" );

SmartStageXY::SmartStageXY( const std::string& productName, imp::IAxisDevice& xAxis, imp::IAxisDevice& yAxis )
    : Product( productName )
    , m_xAxis( xAxis )
    , m_yAxis( yAxis )
{
}

void
SmartStageXY::Initialize() const
{
    LOG_INFO( "Initializing " << Name() << "..." )

    m_xAxis.Initialize();
    m_yAxis.Initialize();

    LOG_INFO( "Homing " << Name() << "..." )

    HomeX_Center();
    HomeY_Center();

    LOG_INFO( Name() << " is ready for use." )
}

void
SmartStageXY::HomeX_Center() const
{
    m_xAxis.EnterHomingMode();
    m_xAxis.Wait();

    m_xAxis.SelectMoveProfile( GetAxisProfileName( m_xAxis, "HomingProfile" ) );
    m_xAxis.Wait();

    const bool isAtPositiveHalf = m_xAxis.GetFlagStatus( "PositiveHalf" );
    const bool isAtPositiveLimit = m_xAxis.GetFlagStatus( "PositiveLimit" );

    // If already on negative limit, move off the negative limit
    if ( isAtPositiveHalf || isAtPositiveLimit )
    {
        m_xAxis.NMoveRelativeToFlag( "MaxNegativeHomingDistance", "NegativeHalf" );
        m_xAxis.Wait();
        m_xAxis.NMoveRelative( "MoveToCenterOffset" );
        m_xAxis.Wait();
    }

    m_xAxis.NMoveRelativeToFlag( "MaxPositiveHomingDistance", "PositiveHalf" );
    m_xAxis.Wait();

    m_xAxis.NResetPosition( "Home" );
    m_xAxis.Wait();

    m_xAxis.ExitHomingMode();
    m_xAxis.Wait();
}

void
SmartStageXY::HomeY_Center() const
{
    m_yAxis.EnterHomingMode();
    m_yAxis.Wait();

    m_yAxis.SelectMoveProfile( GetAxisProfileName( m_yAxis, "HomingProfile" ) );
    m_yAxis.Wait();

    const bool isAtPositiveHalf = m_yAxis.GetFlagStatus( "PositiveHalf" );
    const bool isAtPositiveLimit = m_yAxis.GetFlagStatus( "PositiveLimit" );

    // If already on negative limit, move off the negative limit
    if ( isAtPositiveHalf || isAtPositiveLimit )
    {
        m_yAxis.NMoveRelativeToFlag( "MaxNegativeHomingDistance", "NegativeHalf" );
        m_yAxis.Wait();
        m_yAxis.NMoveRelative( "MoveToCenterOffset" );
        m_yAxis.Wait();
    }

    m_yAxis.NMoveRelativeToFlag( "MaxPositiveHomingDistance", "PositiveHalf" );
    m_yAxis.Wait();

    m_yAxis.NResetPosition( "Home" );
    m_yAxis.Wait();

    m_yAxis.ExitHomingMode();
    m_yAxis.Wait();
}

void
SmartStageXY::HomeX_NegativeLimit() const
{
    m_xAxis.EnterHomingMode();
    m_xAxis.Wait();

    m_xAxis.SelectMoveProfile( GetAxisProfileName( m_xAxis, "HomingProfile" ) );
    m_xAxis.Wait();

    const bool isAtNegativeLimit = m_xAxis.GetFlagStatus( "NegativeLimit" );

    // If already on negative limit, move off the negative limit
    if ( isAtNegativeLimit )
    {
        m_xAxis.NMoveRelative( "OffLimitMove" );
        m_xAxis.Wait();
    }

    m_xAxis.NMoveRelativeToFlag( "MaxNegativeHomingDistance", "NegativeLimit" );
    m_xAxis.Wait();

    m_xAxis.NMoveRelative( "NegativeLimitOffset" );
    m_xAxis.Wait();

    m_xAxis.NResetPosition( "Home" );
    m_xAxis.Wait();

    m_xAxis.ExitHomingMode();
    m_xAxis.Wait();
}
void
SmartStageXY::HomeX_PositiveLimit() const
{
    m_xAxis.EnterHomingMode();
    m_xAxis.Wait();

    m_xAxis.SelectMoveProfile( GetAxisProfileName( m_xAxis, "HomingProfile" ) );
    m_xAxis.Wait();

    const bool isAtPositiveLimit = m_xAxis.GetFlagStatus( "PositiveLimit" );

    // If already on negative limit, move off the negative limit
    if ( isAtPositiveLimit )
    {
        m_xAxis.NMoveRelative( "OffLimitMove" );
        m_xAxis.Wait();
    }

    m_xAxis.NMoveRelativeToFlag( "MaxPositiveHomingDistance", "PositiveLimit" );
    m_xAxis.Wait();

    m_xAxis.NMoveRelative( "PositiveLimitOffset" );
    m_xAxis.Wait();

    m_xAxis.NResetPosition( "Home" );
    m_xAxis.Wait();

    m_xAxis.ExitHomingMode();
    m_xAxis.Wait();
}

void
SmartStageXY::HomeY_NegativeLimit() const
{
    m_yAxis.EnterHomingMode();
    m_yAxis.Wait();

    m_yAxis.SelectMoveProfile( GetAxisProfileName( m_yAxis, "HomingProfile" ) );
    m_yAxis.Wait();

    const bool isAtNegativeLimit = m_yAxis.GetFlagStatus( "NegativeLimit" );

    // If already on negative limit, move off the negative limit
    if ( isAtNegativeLimit )
    {
        m_yAxis.NMoveRelative( "OffLimitMove" );
        m_yAxis.Wait();
    }

    m_yAxis.NMoveRelativeToFlag( "MaxNegativeHomingDistance", "NegativeLimit" );
    m_yAxis.Wait();

    m_yAxis.NMoveRelative( "NegativeLimitOffset" );
    m_yAxis.Wait();

    m_yAxis.NResetPosition( "Home" );
    m_yAxis.Wait();

    m_yAxis.ExitHomingMode();
    m_yAxis.Wait();
}

void
SmartStageXY::HomeY_PositiveLimit() const
{
    m_yAxis.EnterHomingMode();
    m_yAxis.Wait();

    m_yAxis.SelectMoveProfile( GetAxisProfileName( m_yAxis, "HomingProfile" ) );
    m_yAxis.Wait();

    const bool isAtPositiveLimit = m_yAxis.GetFlagStatus( "PositiveLimit" );

    // If already on negative limit, move off the negative limit
    if ( isAtPositiveLimit )
    {
        m_yAxis.NMoveRelative( "OffLimitMove" );
        m_yAxis.Wait();
    }

    m_yAxis.NMoveRelativeToFlag( "MaxPositiveHomingDistance", "PositiveLimit" );
    m_yAxis.Wait();

    m_yAxis.NMoveRelative( "PositiveLimitOffset" );
    m_yAxis.Wait();

    m_yAxis.NResetPosition( "Home" );
    m_yAxis.Wait();

    m_yAxis.ExitHomingMode();
    m_yAxis.Wait();
}


void
SmartStageXY::PerformMoves() const
{
    LOG_INFO( "Performing a series of moves on the " << Name() << "..." )

    m_xAxis.SelectMoveProfile( GetAxisProfileName( m_xAxis, "StandardMoveProfile" ) );
    m_xAxis.Wait();
    m_yAxis.SelectMoveProfile( GetAxisProfileName( m_yAxis, "StandardMoveProfile" ) );
    m_yAxis.Wait();

    // Move to starting position - this assumes the stage has been homed to the center.
    m_xAxis.MoveAbsolute( -45 );
    m_yAxis.MoveAbsolute( -45 );

    m_xAxis.Wait();
    m_yAxis.Wait();

    constexpr int32_t countX = 8;
    for ( int32_t j = 0; j < countX; j++ )
    {

        constexpr int32_t countY = 8;
        for ( int32_t i = 0; i < countY; i++ )
        {
            // Move Y axis Positive
            m_yAxis.MoveRelative( 5 );
            m_yAxis.Wait();
        }

        m_xAxis.MoveRelative( 5 );
        m_xAxis.Wait();

        for ( int32_t i = countY; i > 0; i-- )
        {
            // Move Y axis Negative
            m_yAxis.MoveRelative( -5 );
            m_yAxis.Wait();
        }

        m_xAxis.MoveRelative( 5 );
        m_xAxis.Wait();
    }
}

std::string
SmartStageXY::GetAxisProfileName( imp::IAxisDevice& axis, const std::string& profileName ) const
{
    return axis.Name() + profileName;
}
