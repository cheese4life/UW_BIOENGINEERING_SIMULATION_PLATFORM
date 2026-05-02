/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <cstdint>
#include <string>

namespace imp
{

/*! @class StackTrace
 *   @brief Maintains a stack of function names (and parameters) for each thread.
 *   @details Instances must be created on the stack so the destructor is automatically called
 *           on function exit.
 */
class StackTrace
{
public:
    /*! @fn StackTrace
     *   @brief Push a function onto the current thread stack trace.
     *   @param[in] functionName: function name to record in stack trace.
     *   @param[in] params: function parameters to record in stack trace, leave empty if none.
     */
    StackTrace( const std::string& functionName, const std::string& params );

    /*! @fn ~StackTrace
     *   @brief Pops the current function off the stack.
     */
    virtual ~StackTrace();

    /*! @fn GetCurrentFunctionName
     *   @returns The current function name only (no parameters).
     */
    static std::string GetCurrentFunctionName();

    /*! @fn GetCurrentFunctionWithParameters
     *   @returns The current function name and any parameters.
     */
    static std::string GetCurrentFunctionWithParameters();

    /*! @fn GetStackTrace
     *   @returns The current stack trace for this thread, one function per line.
     */
    static std::string GetStackTrace();

    /*! @fn GetIndentationString
     *   @brief returns a string of spaces, the length of which reflects the indentation level
     *          based on the current stack trace for this thread.
     *   @param[in] indentationSize: The number of spaces to use per indentation level.
     */
    static std::string GetIndentationString( uint32_t indentationSize = 3 );
};

} //  namespace imp
