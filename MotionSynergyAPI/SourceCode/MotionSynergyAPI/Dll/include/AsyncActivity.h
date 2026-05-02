/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "Semaphore.h"

#include <mutex>

namespace imp
{

enum class WaitResult;

/*
 * @brief Defines a thread - safe state machine for tracking the progress of an
 *        Asynchronous Activity, which is an activity begun in one thread and
 *        finished in another.
 */
class AsyncActivity
{
public:
    /*!
     *   @brief  Constructor (empty).
     */
    explicit AsyncActivity();

    /*!
     *   @brief  Virtual destructor (empty).
     */
    ~AsyncActivity() = default;

    /*
     * @brief Begin an async activity. Once started it MUST be finished with a call to Finished
     */
    void Begin();


    /*
     * @brief Begin an async activity or extend the existing async activity. Once started it MUST be finished with a call to ExtendedFinished
     */
    void Extend();

    /*
     * @brief Begin an interrupting activity. It must be finished with a call to FinishedInterrupt
     */
    void BeginInterrupt();

    /*
     * @brief Begin an EStop activity. It must be finished with a call to FinishedEStop
     */
    void BeginEStop();

    /*
     * @brief Finish an async activity.
     */
    void Finished();

    /*
     * @brief Finish an extended async activity.
     */
    void ExtendedFinished();

    /*
     * @brief Finish an interrupting activity.
     */
    void FinishedInterrupt();

    /*
     * @brief Finish an EStop activity.
     */
    void FinishedEStop();

    /*
     * @brief Blocks (with a timeout) the calling thread, waiting for the AsyncActivity to finish
     *        Calling Wait is optional.
     * @returns The result of the Wait call.
     */
    WaitResult Wait( int32_t timeout_ms = -1 );

    /*
     *   @brief      Indicates the start of a period where Wait() should not block. If a Wait()
     *               is currently in progress, it will be unblocked and the call will return immediately.
     *   @details    This does not change the state of the activity. Wait() must be called again
     *               (after FinishPreventWait()) to ensure the asynchronous activity has completed.
     *               This method must be called from any thread which isn’t currently blocked on a Wait call.
     */
    void BeginPreventWait();

    /*
     *   @brief      Indicates the completion of a period where Wait() should not block. The next
     *               call to Wait() will block until the outstanding asynchronous activity has completed.
     *   @details    This method can be called from any thread.
     */
    void FinishPreventWait();

    /*
     * @brief       Gets the action progress state of the activity object.
     * @returns     True if an action is in progress.
     */
    bool IsActionInProgress();

    /*! @brief Function prototype for AsyncActivity completion callback. */
    using CompletionCallback = std::function<void()>;

    void SubscribeCompletionCallback( CompletionCallback callback );
    void UnsubscribeCompletionCallback();

private:
    void DoBegin( bool throwIfAlreadyInProgress );
    void DoFinished( bool throwIfNotInProgress );

    Semaphore m_semaphore;                // Used for blocking Wait calls
    Semaphore m_interruptActionSemaphore; // Used for blocking on an interrupt action.
    Semaphore m_estopActionSemaphore;     // Used for blocking on an EStop action.

    // Mutex to protect the m_inProgress flag
    std::mutex m_mutex;
    bool m_inProgress;
    bool m_inProgressInterruption;
    bool m_inProgressEStop;
    CompletionCallback m_CompletionCallback;
};

} //  namespace imp
