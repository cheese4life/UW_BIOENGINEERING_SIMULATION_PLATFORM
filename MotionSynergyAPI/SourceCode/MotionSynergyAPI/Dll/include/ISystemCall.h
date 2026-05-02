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
 * @brief   Interface for calling out to the system to run commands.
 * @details Simple interface for running things from the filesystem,
            enabling such calls to be mocked for testing.
 */
class ISystemCall
{
public:
    virtual ~ISystemCall() = default;

    /*!
     * @brief   Does the equivalent of a system call.
     */
    virtual int Call( std::string const& commandString ) = 0;

    /*!
     * @brief   Does the equivalent of a system call with a string of arguments
     */
    virtual int Call( std::string const& commandString, std::string const& args ) = 0;
};

} // namespace imp
