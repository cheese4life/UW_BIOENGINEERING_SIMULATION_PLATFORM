/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "SingletonLifetime.h"

#include <mutex>

namespace imp
{

class IPathResolution;
using IPathResolutionSPtr = std::shared_ptr<IPathResolution>;

/*!
 * @class   PathResolutionSingleton
 * @brief   Maintains the singleton instance of the IPathResolution.
 */
class PathResolutionSingleton
{
public:
    PathResolutionSingleton() = delete;

    /*!
     * @brief   Retrieve the statically-created path resolution singleton instance.
     * @returns Pointer to the created IPathResolution.
     */
    static IPathResolutionSPtr Get();

    /*!
     * @brief   Destroy the currently registered (i.e. static) path resolution.
     * @details Intended for unit tests and clean application shutdown.
     */
    static void Destroy();

private:
    static SingletonLifetime<IPathResolution> s_pathResolution;
    static std::mutex s_mutex;
};

} //  namespace imp
