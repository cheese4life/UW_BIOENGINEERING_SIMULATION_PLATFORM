/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

/*!
 *  @brief  Defines abstract AsyncCallback classes.
 *          These classes declare the interface to be provided by all
 *          AyncCallbacks, including the means by which the callback
 *          is invoked asynchronously.
 */
#pragma once

#include "Callback.h"

namespace IPLKeystone
{

/*!
 *  @brief Declares the AsyncInvoke() method to be defined by AsyncCallbacks.
 */
template<typename Context>
class AsyncCallback : public virtual Callback<Context>
{
protected:
    AsyncCallback() {}

public:
    virtual ~AsyncCallback() {}

    /*!
     * @brief Asynchronous delegation style BeginInvoke method.
     * @param context - The value to be passed
     */
    virtual void BeginInvoke( const Context& context ) = 0;

    /*!
     * @brief Asynchronous delegation style TryBeginInvoke method.
     * @param context - The value to be passed
     * @return True if the delegated invokation could be queued
     *
     *  NOTE: TryBeginInvoke was added because the existing implementation of
     *  BeginInvoke in some subclasses (e.g. Spectator) actually asserts
     *  success rather than returning false on failure.  Rather than change
     *  the existing behaviour it was decided to add a Try method instead
     *  which would correctly return bool and not assert success.
     */
    virtual bool TryBeginInvoke( const Context& context ) = 0;

    /*!
     * @brief Synchronous delegation style (though still asynchronous) Invoke method.
     * @param context - The value to be passed
     * @return Nothing, asserts if failed to add to queue
     */
    virtual void
    Invoke( const Context& context ) override
    {
        BeginInvoke( context );
    }

private:
    virtual void AsyncInvoke( const Context& context ) = 0;
};

/*!
 * @brief Concrete subclass of AsyncCallback<Context> which takes no action upon notification.
 */
template<typename Context>
class NullAsyncCallback : public AsyncCallback<Context>
{
public:
    NullAsyncCallback() {}
    ~NullAsyncCallback() {}

    void
    BeginInvoke( const Context& )
    {
    }
    bool
    TryBeginInvoke( const Context& )
    {
        return true;
    }

private:
    void
    AsyncInvoke( const Context& )
    {
    }
};

/*!
 *  @brief Declares the AsyncInvoke() method to be defined by AsyncCallbacks.
 */
template<>
class AsyncCallback<void> : public virtual Callback<void>
{
protected:
    AsyncCallback() {}

public:
    virtual ~AsyncCallback() {}


    /*!
     * @brief Asynchronous delegation style BeginInvoke method.
     */
    virtual void BeginInvoke() = 0;

    /*!
     * @brief Asynchronous delegation style TryBeginInvoke method.
     * @return True if the delegated invokation could be queued
     *
     *  NOTE: TryBeginInvoke was added because the existing implementation of
     *  BeginInvoke in some subclasses (e.g. Spectator) actually asserts
     *  success rather than returning false on failure.  Rather than change
     *  the existing behaviour it was decided to add a Try method instead
     *  which would correctly return bool and not assert success.
     */
    virtual bool TryBeginInvoke() = 0;

    /*!
     * @brief Synchronous delegation style (though still asynchronous) Invoke method.
     * @return Nothing, silently ignoring failures to queue
     */
    virtual void
    Invoke() override
    {
        BeginInvoke();
    }

private:
    virtual void AsyncInvoke() = 0;
};

/*!
 * @brief Concrete subclass of AsyncCallback<void> which takes no action upon notification.
 */
template<>
class NullAsyncCallback<void> : public AsyncCallback<void>
{
public:
    NullAsyncCallback() {}
    ~NullAsyncCallback() {}

    void
    BeginInvoke() override
    {
    }
    bool
    TryBeginInvoke() override
    {
        return true;
    }

private:
    void
    AsyncInvoke() override
    {
    }
};

} // namespace IPLKeystone
