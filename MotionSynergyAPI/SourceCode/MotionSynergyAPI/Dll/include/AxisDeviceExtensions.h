/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IAxisDevice.h"

namespace imp
{

/*!
 *   @brief      Update a named position for this axis to the provided value.
 *               The value will be persisted to configuration.
 *   @param[in]  axis                The axis instance to have it's position set.
 *   @param[in]  namedPosition       The named position to be set.
 *   @param[in]  value               Tha value to persist/add for the named position.
 *   @param[in]  positionMustExist   If true, if the named position does not exist a std::runtime_error is thrown.
 */
void
SetNamedPosition( IAxisDevice& axis, const std::string& namedPosition, double value, bool positionMustExist = true );

/*!
 *   @brief      Update a named position for this axis to the axis position most recently commanded to move to.
 *               The value will be persisted to configuration.
 *   @param[in]  axis                The axis instance to have it's position set.
 *   @param[in]  namedPosition       The named position to be set.
 *   @param[in]  positionMustExist   If true, if the named position does not exist a std::runtime_error is thrown.
 */
void SetCommandedPositionAsNamedPosition(
    IAxisDevice& axis,
    const std::string& namedPosition,
    bool positionMustExist = true );


/*!
 *   @brief      Remove a named position for this axis and it's persisted configuration.
 *   @param[in]  axis                The axis instance to have it's position set.
 *   @param[in]  namedPosition       The named position to be removed.
 *   @param[in]  positionMustExist   If true, if the named position does not exist a std::runtime_error is thrown.
 */
void RemoveNamedPosition( IAxisDevice& axis, const std::string& namedPosition, bool positionMustExist = true );

/*!
 *   @brief      Update a named distance for this axis to the provided value.
 *               The value will be persisted to configuration.
 *   @param[in]  axis                The axis instance to have it's position set.
 *   @param[in]  namedDistance       The named distance to be set.
 *   @param[in]  value               Tha value to persist/add for the named distance.
 *   @param[in]  positionMustExist   If true, if the named distance does not exist a std::runtime_error is thrown.
 */
void
SetNamedDistance( IAxisDevice& axis, const std::string& namedDistance, double value, bool distanceMustExist = true );

/*!
 *   @brief      Update a named move profile for this axis to the provided value.
 *               The value will be persisted to configuration.
 *   @param[in]  axis                   The axis instance to have it's position set.
 *   @param[in]  namedMoveProfile       The named move profile to be set.
 *   @param[in]  params                 Tha params to persist/add for the named profile. If the params is incompelete, a std::runtime_error will be thrown.
 *   @param[in]  moveProfileMustExist   If true, if the named profile does not exist a std::runtime_error is thrown.
 */
void SetNamedMoveProfile(
    IAxisDevice& axis,
    const std::string& namedMoveProfile,
    std::map<std::string, double> params,
    bool moveProfileMustExist = true );
} //  namespace imp
