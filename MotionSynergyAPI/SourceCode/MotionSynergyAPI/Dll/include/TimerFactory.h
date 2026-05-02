/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ITimer.h"
#include <functional>
#include <memory>

namespace imp
{

class ITimerService;
using ITimerServiceSPtr = std::shared_ptr<ITimerService>;
using TimerCreateFunction = std::function<std::shared_ptr<ITimer>()>;

/*!
 *  @brief   Factory for creating instances of the Timer class.
 *  @details Ensures that the implementation is not exposed
 *           outside of this library.
 */
class TimerFactory
{
public:
    TimerFactory() = delete;

    /*!
     * @brief    Initialize the timer service, so timer instances can created.
     * @details  Must be called before Create().
     * @param[in] pTimerService Instance of a timer service to use or nullptr
     *                          to use the default timer service provided with
     *                          the framework.
     * @throws A std::logic_error if Initialize has already been called.
     */
    static void Initialize( ITimerServiceSPtr pTimerService = nullptr );

    /*!
     * @brief    Initialize the timer service, so timer instances can created using the supplied
     *           timer create function.
     * @details  Must be called before Create().
     * @param[in] timerCreateFunction Function which will return a new timer instance when Create() is called.
     * @throws A std::logic_error if Initialize has already been called.
     */
    static void Initialize( TimerCreateFunction timerCreateFunction );

    /*!
     * @brief  Prepares for shutdown, releases the timer service pointer.
     */
    static void Destroy();


    /*!
     *   @returns Shared pointer to a new ITimer instance.
     *   @details Initialize() must have been called.
     */
    static std::shared_ptr<ITimer> Create();

private:
    static ITimerServiceSPtr s_pTimerService;
    static TimerCreateFunction s_timerCreateFunction;
};

} //  namespace imp
