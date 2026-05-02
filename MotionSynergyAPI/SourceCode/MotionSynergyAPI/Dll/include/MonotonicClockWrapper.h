/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IClock.h"


namespace imp
{

/*!
 * @brief   Wraps the MonotonicClock to provide the time since the epoch as a chrono duration.
 */
class MonotonicClockWrapper : public virtual IClock
{
public:
    MonotonicClockWrapper() = default;
    ~MonotonicClockWrapper() override = default;

    /*!
     * @brief   Return the current clock time.
     * @returns The current clock time in seconds.
     */
    Seconds now() const override;

    /*!
     * @brief   Return the current clock time.
     * @returns The current clock time in milliseconds.
     */
    Milliseconds now_ms() const override;

    /*!
     * @brief   Return the current clock time.
     * @returns The current clock time in microseconds.
     */
    Microseconds now_us() const override;
};

} //  namespace imp
