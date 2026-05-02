/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <cstdint>


namespace imp
{

/*!
 * @brief   Provides a convenient means of measuring elapsed time.
 *
 * @details This class uses the MonotonicClock to ensure the measured
 *          time is not affected by changes to the system clock time.
 */
class Stopwatch
{
public:
    /*!
     * @constructor Stores a timestamp marking the beginning of the
     *              measured time period.
     */
    Stopwatch();

    /*!
     * @brief    Resets the elapsed time to zero.
     * @details  Takes a new timestamp marking the beginning of
     *           a new time period.
     */
    void Reset();

    /*!
     * @brief    Returns the elapsed time in microseconds since the
     *           constructor was called.
     * @details  This method can be called multiple times to take
     *           multiple measurements.
     * @returns  The elapsed time in microseconds.
     */
    uint64_t GetElapsedMicroseconds() const;

    /*!
     * @brief    Returns the elapsed time in milliseconds since the
     *           constructor was called.
     * @details  This method can be called multiple times to take
     *           multiple measurements.
     * @returns  The elapsed time in milliseconds.
     */
    uint64_t GetElapsedMilliseconds() const;

    /*!
     * @brief    Returns the elapsed time in seconds since the
     *           constructor was called.
     * @details  This method can be called multiple times to take
     *           multiple measurements.
     * @returns  The elapsed time in seconds.
     */
    uint64_t GetElapsedSeconds() const;

private:
    uint64_t m_startTimestamp;
};

} //  namespace imp
