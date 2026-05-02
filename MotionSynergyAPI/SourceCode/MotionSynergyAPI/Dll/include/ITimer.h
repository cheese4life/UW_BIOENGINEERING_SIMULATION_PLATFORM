/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IRunnableDispatcher.h"
#include "ITimerService.h"


namespace imp
{

/*!
 * @brief  Defines an interface for timer instances.
 */
class ITimer
{
public:
    /*! @brief  The destructor must cancel the timer if still active.
     */
    virtual ~ITimer() = default;

    /*! @brief  Sets (or resets) a timer
     *
     *  @param delay The period of time before expiry of the timer
     *  @param callback The function to be called upon expiry
     *  @param dispatcher The dispatcher through which the function should be called
     *  @param dispatchPriority The priority at which the callback should be dispatched
     */
    virtual void
    Set( std::chrono::milliseconds delay,
         TimerExpiryCallback callback,
         std::weak_ptr<IDispatcher> dispatcher,
         IRunnableDispatcher::Priority dispatchPriority = IDispatcher::DEFAULT_PRIORITY ) = 0;

    /*! @brief  Cancels the timer (if active)
     *  @return True if the timer was active and cancelled
     */
    virtual bool Cancel() = 0;

    /*! @brief  Queries whether a timer has been set and hasn't yet expired
     *  @return True if the timer is set and hasn't expired
     */
    virtual bool IsActive() const = 0;
};

} //  namespace imp
