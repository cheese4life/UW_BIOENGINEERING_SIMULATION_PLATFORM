/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include "DOF5.h"
#include "SmartStageXY.h"

/*!
 *   @brief  A class demonstrating how to use the IAxisDevice interface to perform
 *           various operations such as Initialization, Homing and moving a SmartStageXY and a DOF-5.
 */
class SmartStageXYZ : public SmartStageXY
{
public:
    /*!
     *   @brief Takes a reference to the X, Y and Z axes.
     *   @details No action is taken in the constructor. Configure() must also be called.
     *
     */
    SmartStageXYZ(
        const std::string& stageProductName,
        const std::string& zAxisProductName,
        imp::IAxisDevice& xAxis,
        imp::IAxisDevice& yAxis,
        imp::IAxisDevice& zAxis );

    /*!
     *   @brief Initialize all axes.
     *   @details This will connect to each axis, perform analog calibration and
     *            commutation. Initialize must be called before homing.
     *
     *   @exception std::runtime_error if initialize fails on either axis.
     */
    void Initialize() const override;

    /*!
     *   @brief Perform a series of moves on the X, Y and Z Axes.
     *   @details Each axis must be Initialized and Homed before general move operations can be performed.
     *
     *   @exception std::runtime_error if an action fails on either Axis.
     */
    void PerformMoves() const override;

private:
    DOF5 m_dof5;
};
