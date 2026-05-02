/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <map>
#include <mutex>
#include <string>


namespace imp
{

/*!
 *  @brief   PrettyPrinter performs a synchronous threadsafe coloured write to stdout
 *
 */
class PrettyPrinter
{
public:
    /*!
     * @brief Constructor.
     * @param[in] idStr          String prefix for any message printed with this instance.
     * @param[in] colorIdStr     Name of the color to print with.
     * @param[in] coutProtection Mutex to lock before writing to std::cout
     */
    explicit PrettyPrinter( std::string const& idStr, std::string const& colorIdStr, std::mutex& coutProtection );

    virtual ~PrettyPrinter() = default;

    /*!
     *  @brief   Prints the msg in a form specific to id strings of device and driver tests
     */
    void Print( std::string const& msg );

private:
    std::string m_IdStr;
    std::mutex& m_printMutex;
    std::map<std::string, std::string> m_colors;
    std::string m_colorCode;
};

} // namespace imp
