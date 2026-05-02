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
 *           various operations such as Initialization, Homing and moving the DOF5.
 */
class DOF5 : public Product
{
public:
    /*!
     *   @brief Takes a reference to the axis.
     *   @details No action is taken in the constructor. Configure() must also be called.
     *
     */
    DOF5( const std::string& productName, imp::IAxisDevice& axis );

    /*!
     *   @brief Initialize the axis.
     *   @details This will connect to the axis, perform analog calibration and
     *            commutation. Initialize must be called before homing.
     *
     *   @exception std::runtime_error if initialize fails on the axis.
     */
    void Initialize() const override;

    /*!
     *   @brief Home the axis to the center travel vane.
     *   @details Initialize must be called before homing.
     *
     *   @exception std::runtime_error if an action fails on the axis.
     */
    void Home() const;

    /*!
     *   @brief Perform a series of moves on the axis.
     *   @details The axis must be Initialized and Homed before general move operations can be performed.
     *
     *   @exception std::runtime_error if an action fails on the axis.
     */
    void PerformMoves() const override;

    /*!
     *   @brief Get the name of the profile for this axis.
     *
     *   @return The axis name concatenated with the profile name.
     */
    std::string GetAxisProfileName( const std::string& profileName ) const;

    /*!
     *   @brief The DOF-5 axis.
     */
    imp::IAxisDevice& m_axis;
};
