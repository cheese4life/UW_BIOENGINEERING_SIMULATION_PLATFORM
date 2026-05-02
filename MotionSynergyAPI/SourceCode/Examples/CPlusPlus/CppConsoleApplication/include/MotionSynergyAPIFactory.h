/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <memory>

namespace MotionSynergyAPI
{

class IMotionSynergy;
using IMotionSynergySPtr = std::shared_ptr<IMotionSynergy>;

/*!
 *  @brief  Factory class to create/destroy an instance of the MotionSynergyAPI.
 */
class MotionSynergyAPIFactory
{
public:
    /*!
     *  @brief  Create an instance of the MotionSynergyAPI.
     *  @returns   Shared pointer to the MotionSynergyAPI instance.
     */
    static IMotionSynergySPtr Create();

    /*!
     *  @brief  Destroy the MotionSynergyAPI instance.
     */
    static void Destroy();
};

} // namespace MotionSynergyAPI
