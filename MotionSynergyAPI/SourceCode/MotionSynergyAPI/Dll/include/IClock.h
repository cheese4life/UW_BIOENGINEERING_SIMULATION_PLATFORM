/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <chrono>


namespace imp
{

/*!
 * @brief   Provides a common interface to access a clock's time as a chrono duration
 */
class IClock
{
public:
    IClock() = default;
    virtual ~IClock() = default;

    using Seconds = std::chrono::seconds;
    using Milliseconds = std::chrono::milliseconds;
    using Microseconds = std::chrono::microseconds;
    // Can be removed when we move to C++20
    using Days = std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>::type>;

    /*!
     * @brief   Return the current clock time.
     * @returns The current clock time in seconds.
     */
    virtual Seconds now() const = 0;

    /*!
     * @brief   Return the current clock time.
     * @returns The current clock time in milliseconds.
     */
    virtual Milliseconds now_ms() const = 0;

    /*!
     * @brief   Return the current clock time.
     * @returns The current clock time in microseconds.
     */
    virtual Microseconds now_us() const = 0;
};

} //  namespace imp
