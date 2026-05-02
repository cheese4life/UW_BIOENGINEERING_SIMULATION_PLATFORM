/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "WaitResult.h"

#include <boost/thread/condition_variable.hpp>
#include <chrono>

namespace imp
{

/*!
 *   @brief  Binds a condition variable and associated mutex with a
 *           boolean state variable.
 */
class Semaphore
{
public:
    /*! @brief Constructor. */
    explicit Semaphore() = default;

    /*!
     *  @brief      Acquire the semaphore.
     */
    void Take();

    /*! @brief Release the semaphore. */
    void Give();

    /*!
     *  @brief      Wait indefinitely until the semaphore is released.
     *  @return     Success (the semaphore was released or not taken), Cancelled if the Wait was cancelled.
     */
    WaitResult Wait();

    /*!
     *  @brief      Wait until the semaphore is released, or the time limit is reached.
     *  @param[in]  timeout_ms  The number of milliseconds to wait before giving up.
     *  @return     Success if the semaphore was released or not taken, Timeout if a timeout occurred, Cancelled if the Wait was cancelled.
     */
    WaitResult Wait( int64_t timeout_ms );

    /*!
     *  @brief      Wait until the semaphore is released, or the time limit is reached.
     *  @param[in]  timeout_ms  The number of milliseconds to wait before giving up.
     *  @return     Success if the semaphore was released or not taken, Timeout if a timeout occurred, Cancelled if the Wait was cancelled.
     */
    WaitResult Wait( std::chrono::milliseconds timeout_ms );

    /*
     *   @brief      Cancels a Wait() operation from another thread.
     *   @details    This method must be called from any thread which isn’t currently blocked on a Wait call.
     */
    void Cancel();

    /*
     *   @brief      Indicates the start of a period where Wait() should not block. If a Wait()
     *               is currently in progress, it will be unblocked and the call will return immediately.
     *   @details    This does not change the state of the Semaphore.
     *               This method must be called from any thread which isn’t currently blocked on a Wait call.
     */
    void BeginPreventWait();

    /*
     *   @brief      Indicates the completion of a period where Wait() should not block. The next
     *               call to Wait() will block until the semaphore is released or a timeout occurs.
     *   @details    This method can be called from any thread.
     */
    void FinishPreventWait();

private:
    WaitResult WaitInternal( boost::chrono::milliseconds timeout_ms );

    static const boost::chrono::milliseconds INFINITE_WAIT;

    // Must use boost here rather than std, as the standard wait_for() is
    // affected by system clock changes but the boost implementation is not.
    boost::condition_variable m_condition;
    boost::mutex m_mutex;

    bool m_inProgress = false;
    bool m_cancelled = false;
    bool m_isPreventWaitActive = false;
};

} //  namespace imp
