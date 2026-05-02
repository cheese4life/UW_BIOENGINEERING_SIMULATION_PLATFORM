/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <condition_variable>
#include <functional>
#include <memory>
#include <thread>

namespace imp
{

/*!
 * @brief        Execute a function in a new thread.
 * @details      This class can be used to create a new thread, optionally wait for the thread to start execution,
 *               executes the supplied function and wait for the function to complete (and thread to exit).
 *               The supplied function must return. This is not a mechanism to start a background task.
 *               This class is very useful for some types of units tests which need to test thread
 *               synchronisation mechanisms. It should not be overused in production code to spawn random threads.
 *               Louganis-based architectures normally have a strict threading model in which all necessary threads are
 *               created at startup. Consult the project architect before use.
 */
class ExecuteFunctionInNewThread
{
public:
    /*!
     * @brief The constructor does not start the thread.
     */
    ExecuteFunctionInNewThread();

    /*!
     * @brief The destructor ensures the function/thread has completed (using a thread join operation) before completing.
     */
    virtual ~ExecuteFunctionInNewThread();

    /*!
     * @brief Execute a function in a new thread. This will create a new thread to execute the function.
     * @details Only one function at a time can be executed.
     *          WaitForFunctionCompletion() must be called before executing the next function.
     *          The function to execute must return.
     * @param[in] functionToExecuteInNewThread The function to execute in the new thread.
     * @param[in] waitForThreadToStart If true, StartFunction() will not return until the new thread has begun execution.
     *                                 If false, StartFunction() may return prior to the new thread beginning execution.
     *
     * @throws std::runtime_error If there is already a function being executed.
     */
    void StartFunction( std::function<void()> functionToExecuteInNewThread, bool waitForThreadToStart = true );

    /*!
     * @brief Waits for the function started by StartFunction() to complete.
     * @details WaitForFunctionCompletion() is safe to call multiple times or even if StartFunction() is not called.
     *          This function will perform a thread join on the new thread (if started).
     */
    void WaitForFunctionCompletion();

    /*!
     * @brief Convenience method to execute a function in a new thread and wait for it to complete.
     * @details This method simply calls StartFunction() followed by WaitForFunctionCompletion().
     */
    void ExecuteFunction( std::function<void()> functionToExecuteInNewThread );

private:
    void NotifyThreadStarted();

    std::shared_ptr<std::thread> m_pThread;
    std::condition_variable m_threadStartedCondition;
    std::mutex m_threadStartedMutex;
    bool m_isThreadStarted;
};

} //  namespace imp
