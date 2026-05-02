/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include <string>

/*!
 *   @brief  Parse the command line arguments accepted by this application.
 */
class CmdLineArgumentParser
{
public:
    /*!
     *   @brief  Parse the command line arguments accepted by this application.
     *
     *   @param[in]  argc      Parameter passed into main().
     *   @param[in]  argv      Parameter passed into main().
     *   @param[out]  instrumentConfigPathname   Updated to use the main 'instrument' configuration file specified on the command line (if specified).
     *   @param[out]  logConfigPathname          Updated to use the logging configuration file specified on the command line (if specified).
     *
     *   @return true if the command line options are valid and the application should proceed, false if the command line arguments are invalid and the application should exit.
     */
    static bool
    Parse( int argc, const char* const argv[], std::string& instrumentConfigPathname, std::string& logConfigPathname );
};
