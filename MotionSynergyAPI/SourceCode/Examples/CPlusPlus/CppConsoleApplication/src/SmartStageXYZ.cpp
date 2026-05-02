/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "SmartStageXYZ.h"

#include <IAxisDevice.h>
#include <LoggerMacros.h>

#include <boost/thread.hpp>
#include <chrono>

const std::string ComponentName( "SmartStageXYZ" );

SmartStageXYZ::SmartStageXYZ(
    const std::string& stageProductName,
    const std::string& zAxisProductName,
    imp::IAxisDevice& xAxis,
    imp::IAxisDevice& yAxis,
    imp::IAxisDevice& zAxis )
    : SmartStageXY( stageProductName, xAxis, yAxis )
    , m_dof5( zAxisProductName, zAxis )
{
}

void
SmartStageXYZ::Initialize() const
{
    LOG_INFO( "Initializing " << Name() << " & " << m_dof5.Name() << "..." )

    m_xAxis.Initialize();
    m_yAxis.Initialize();
    m_dof5.m_axis.Initialize();

    LOG_INFO( "Homing " << Name() << " & " << m_dof5.Name() << "..." )

    HomeX_Center();
    HomeY_Center();
    m_dof5.Home();

    LOG_INFO( Name() << " & " << m_dof5.Name() << " is ready for use." )
}

void
SmartStageXYZ::PerformMoves() const
{
    LOG_INFO( "Performing a series of moves on the " << Name() << " & " << m_dof5.Name() << "..." )

    auto& zAxis = m_dof5.m_axis;

    m_xAxis.SelectMoveProfile( GetAxisProfileName( m_xAxis, "StandardMoveProfile" ) );
    m_xAxis.Wait();
    m_yAxis.SelectMoveProfile( GetAxisProfileName( m_yAxis, "StandardMoveProfile" ) );
    m_yAxis.Wait();
    zAxis.SelectMoveProfile( GetAxisProfileName( zAxis, "StandardMoveProfile" ) );
    zAxis.Wait();

    // Move to starting position - this assumes the stage has been homed to the center.
    m_xAxis.MoveAbsolute( -45 );
    m_yAxis.MoveAbsolute( -45 );

    zAxis.NMoveAbsolute( "Home" );

    m_xAxis.Wait();
    m_yAxis.Wait();
    zAxis.Wait();

    // Move the XY stage
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

    // Move the DOF-5

    constexpr int32_t NumberOfMoves = 5;

    for ( int32_t i = 0; i < NumberOfMoves; i++ )
    {
        zAxis.MoveRelative( 0.5 );
        zAxis.Wait();
    }
    boost::this_thread::sleep_for( boost::chrono::milliseconds( 4000 ) );

    for ( int32_t i = NumberOfMoves; i >= 0; i-- )
    {
        zAxis.MoveRelative( -0.5 );
        zAxis.Wait();
    }
}
