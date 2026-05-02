/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "CmdLineArgumentParser.h"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

bool
CmdLineArgumentParser::Parse(
    int argc,
    const char* const argv[],
    std::string& instrumentConfigPathname,
    std::string& logConfigPathname )
{
    const char* HelpArgName = "help";
    const char* InstrumentArgName = "instrument";
    const char* LogArgName = "log";
    const char* EventsArgName = "events";

    std::string InstrumentArgHelp =
        "The instrument configuration file to use (default = " + instrumentConfigPathname + ").";
    std::string LogArgHelp = "The log configuration file to use (default = " + logConfigPathname + ").";

    po::options_description desc( "Allowed options" );
    desc.add_options()( HelpArgName, "Display this help message." )(
        InstrumentArgName,
        po::value<std::string>(),
        InstrumentArgHelp
            .c_str())(LogArgName, po::value<std::string>(), LogArgHelp.c_str())( EventsArgName, po::value<std::string>() );

    po::variables_map vm;

    try
    {
        po::store( po::parse_command_line( argc, argv, desc ), vm );
    }
    catch ( po::error& e )
    {
        std::cout << e.what() << std::endl;
        return false;
    }

    po::notify( vm );

    if ( vm.count( HelpArgName ) )
    {
        std::cout << desc << std::endl;
        return false;
    }

    if ( vm.count( InstrumentArgName ) )
    {
        instrumentConfigPathname = vm[ InstrumentArgName ].as<std::string>();
    }

    if ( vm.count( LogArgName ) )
    {
        logConfigPathname = vm[ LogArgName ].as<std::string>();
    }

    return true;
}
