/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include "Product.h"

#include <string>

/*!
 *   @brief  A class demonstrating how to use the IAxisDevice interface to perform
 *           various operations such as Initialization, Homing and moving the SmartStageXY.
 */
class SmartStageXY : public Product
{
public:
    /*!
     *   @brief Takes a reference to the X and Y axis.
     *   @details No action is taken in the constructor. Configure() must also be called.
     *
     */
    SmartStageXY( const std::string& productName, imp::IAxisDevice& xAxis, imp::IAxisDevice& yAxis );

    /*!
     *   @brief Initialize both axis.
     *   @details This will connect to each axis, perform analog calibration and
     *            commutation. Initialize must be called before homing.
     *
     *   @exception std::runtime_error if initialize fails on either axis.
     */
    void Initialize() const override;

    /*!
     *   @brief Home the XAxis to the stage center.
     *   @details Initialize must be called before homing.
     *
     *   @exception std::runtime_error if an action fails on the X Axis.
     */
    void HomeX_Center() const;

    /*!
     *   @brief Home the XAxis to the negative limit.
     *   @details Initialize must be called before homing.
     *
     *   @exception std::runtime_error if an action fails on the X Axis.
     */
    void HomeX_NegativeLimit() const;

    /*!
     *   @brief Home the XAxis to the Positive limit.
     *   @details Initialize must be called before homing.
     *
     *   @exception std::runtime_error if an action fails on the X Axis.
     */
    void HomeX_PositiveLimit() const;

    /*!
     *   @brief Home the YAxis to the stage center.
     *   @details Initialize must be called before homing.
     *
     *   @exception std::runtime_error if an action fails on the Y Axis.
     */
    void HomeY_Center() const;

    /*!
     *   @brief Home the YAxis to the negative limit.
     *   @details Initialize must be called before homing.
     *
     *   @exception std::runtime_error if an action fails on the Y Axis.
     */
    void HomeY_NegativeLimit() const;

    /*!
     *   @brief Home the YAxis to the positive limit.
     *   @details Initialize must be called before homing.
     *
     *   @exception std::runtime_error if an action fails on the Y Axis.
     */
    void HomeY_PositiveLimit() const;

    /*!
     *   @brief Perform a series of moves on the X and Y Axis.
     *   @details Each axis must be Initialized and Homed before general move operations can be performed.
     *
     *   @exception std::runtime_error if an action fails on either Axis.
     */
    void PerformMoves() const override;

    /*!
     *   @brief Get the name of the profile for the provided axis.
     *
     *   @return The axis name concatenated with the profile name.
     */
    std::string GetAxisProfileName( imp::IAxisDevice& axis, const std::string& profileName ) const;

protected:
    imp::IAxisDevice& m_xAxis;
    imp::IAxisDevice& m_yAxis;
};
