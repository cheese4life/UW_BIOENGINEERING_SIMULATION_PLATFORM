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
class DllLoader;
using DllLoaderSPtr = std::shared_ptr<DllLoader>;

class IMotionSynergy;
using IMotionSynergySPtr = std::shared_ptr<IMotionSynergy>;

/*!
 *  @brief Singleton responsible for loading the MotionSynergyAPI (DLL) on demand.
 */
class MotionSynergyAPISingleton
{
public:
    MotionSynergyAPISingleton() = delete;

    /*!
     * @brief   Retrieve the singleton IMotionSynergy instance. This will load the DLL if it's
     *          not already loaded.
     * @returns Pointer to the created IMotionSynergy.
     * @throws  std::runtime error if the DLL cannot be loaded
     */
    static IMotionSynergySPtr Get();

    /*!
     * @brief  Prepares for shutdown, releases the IMotionSynergy and unloads the DLL.
     */
    static void Destroy();

private:
    static DllLoaderSPtr s_pDllLoader;
    static IMotionSynergySPtr s_pMotionSynergyAPI;
};

} // namespace MotionSynergyAPI
