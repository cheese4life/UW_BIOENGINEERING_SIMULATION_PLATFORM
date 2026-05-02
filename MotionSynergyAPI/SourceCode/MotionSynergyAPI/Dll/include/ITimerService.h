/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDispatcher.h"
#include <boost/core/noncopyable.hpp>
#include <chrono>


namespace imp
{

typedef std::function<void()> TimerExpiryCallback;

/*!
 *  @brief     Interface used by the Timer class for starting
 *             and stopping timers.
 *  @details   Should not be used directly by client code, use the Timer class instead.
 *             Can be used to define an alternative implementation
 *             for timer services to that provided by the framework.
 */
class ITimerService : boost::noncopyable
{
public:
    virtual ~ITimerService() = default;

    /*! @brief Starts (or restarts) a timer
     *
     *  @param timerId The address used as the unique ID Of this timer
     *  @param delay The period of time before expiry of the timer
     *  @param callback The function to be called upon expiry
     *  @param dispatcher The dispatcher through which the function should be called
     *  @param dispatchPriority The priority at which the callback should be dispatched
     */
    virtual void Start(
        const void* timerId,
        std::chrono::milliseconds delay,
        TimerExpiryCallback callback,
        std::weak_ptr<IDispatcher> dispatcher,
        uint8_t dispatchPriority = IDispatcher::DEFAULT_PRIORITY ) = 0;

    /*! @brief  Stops the timer (if active)
     *  @param  timerId The address used as the unique ID Of this timer
     *  @return True if the timer was active and cancelled
     */
    virtual bool Stop( const void* timerId ) = 0;

    /*! @brief  Queries whether a timer has been started and hasn't
     *  expired/cancelled
     *
     *  @param  timerId The address used as the unique ID Of this timer
     *  @return True if the timer was started and hasn't expired/cancelled
     */
    virtual bool IsActive( const void* timerId ) const = 0;
};

} //  namespace imp
