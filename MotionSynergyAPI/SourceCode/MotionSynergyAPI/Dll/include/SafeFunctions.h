/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <cstdio>
#include <ctime>
#include <memory>
#include <string>

namespace imp
{

/*!
 * @brief A collection of safe versions of standard C library functions.
 */
class SafeFunctions
{
public:
    /*!
     * @brief   Safe version of gmtime.
     * @param   timer   Pointer to time_t object representing the time to convert.
     * @param   buf     Pointer to tm structure to receive the converted time.
     * @return  Pointer to tm structure (same as buf) on success, nullptr on failure.
     */
    static tm* gmtime_s( const time_t* timer, tm* buf );

    /*!
     * @brief   Safe version of localtime.
     * @param   timer   Pointer to time_t object representing the time to convert.
     * @param   buf     Pointer to tm structure to receive the converted time.
     * @return  Pointer to tm structure (same as buf) on success, nullptr on failure.
     */
    static tm* localtime_s( const time_t* timer, tm* buf );

    /*!
     * @brief   Get a string description of an error number.
     * @param   errorNumber     The error number (e.g. errno).
     * @return  String description of the error.
     */
    static std::string GetErrorString( int errorNumber );

    /*!
     * @brief Opens a file and returns a shared pointer to the FILE object. The
     *        file handle will be automatically closed when the shared pointer goes out of scope.
     *
     * @param filename  The name of the file to open.
     * @param mode      The mode in which to open the file (e.g. "r", "wb").
     * @return          A shared pointer to the opened FILE object or an empty pointer on failure.
     */
    static std::shared_ptr<FILE> Open( const char* filename, const char* mode );
};

} //  namespace imp
