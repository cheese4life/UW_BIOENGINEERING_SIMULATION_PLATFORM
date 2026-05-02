/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/


#pragma once

#include "ItemDetails.h"
#include <list>
#include <memory>
#include <string>


namespace imp
{

class IConfiguration;


/*!
 * @brief Parsing utility that can be used by derived projects to parse configuration to set up reported alert/event details.
 */
class DetailerParser
{
public:
    DetailerParser() = delete; // static class

    /*!
     * @brief Parses configuration to locate details for building by a detailer.
     * @param pConfig Pointer to config object.
     * @param section The section name that contains the detailer configuration.
     * @param countProperty The name of the parameter within the project config file used to determine how many detailer configurations need to be loaded.
     * @param filePrefix The prefix used within the section of the config to locate the files that will be parsed for the reported details.
     * @param groupProperty The name of the parameter within the detailer configuration for the group sections in a detailer configuration file.
     * @throws IConfigurationException If an error occurs whilst parsing the configuration.
     * @returns The list of items successfully parsed from configuration.
     */
    static std::list<ItemDetails<std::string>> Parse(
        std::shared_ptr<IConfiguration> pConfig,
        const std::string& section,
        const std::string& countProperty,
        const std::string& filePrefix,
        const std::string& groupProperty );

private:
    static std::string BuildConfigurationPath( const std::string& section, const std::string& parameter );
};


} //  namespace imp
