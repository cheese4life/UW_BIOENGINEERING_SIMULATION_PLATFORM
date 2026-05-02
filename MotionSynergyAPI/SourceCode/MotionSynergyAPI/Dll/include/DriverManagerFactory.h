/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <memory>

namespace imp
{

class IDriverManager;
using IDriverManagerSPtr = std::shared_ptr<IDriverManager>;
class IDriverFactory;
using IDriverFactorySPtr = std::shared_ptr<IDriverFactory>;

/*!
 *   @brief  Factory to create an IDriverManager
 */
class DriverManagerFactory
{
public:
    /*!
     * @brief   Create a IDriverManager
     *
     * @param[in] pDriverFactory Factory used by the IDriverManager to create new driver instances.
     *
     * @returns A pointer to the IDriverManager.
     */
    static IDriverManagerSPtr Create( IDriverFactorySPtr pDriverFactory );
};

} //  namespace imp
