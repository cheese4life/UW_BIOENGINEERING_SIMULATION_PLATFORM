/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <string>

namespace imp
{

/*!
 * @class   IPathResolution
 * @brief   Provides an interface for resolving template items in pathnames
 */
class IPathResolution
{
public:
    IPathResolution() = default;
    virtual ~IPathResolution() = default;

    /*!
     * @brief Sets the runtime path for a template item name
     * @param [in] name The template item name, the format is a string enclosed in [ and ], e.g. "[Config]"
     * @param [in] path The runtime path
     */
    virtual void SetRuntimePath( const std::string& name, const std::string& path ) = 0;

    /*!
     * @brief Parses a pathname by replacing any template items with their runtime paths
     * @param [in] path The pathname string, e.g. "[Config]/Device.cfg"
     * @return The pathname string containing resolved template items
     */
    virtual std::string ResolvePathname( std::string path ) const = 0;
};

} //  namespace imp
