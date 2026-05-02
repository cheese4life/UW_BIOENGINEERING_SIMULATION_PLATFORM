/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "DeviceDriverBase.h"

namespace imp
{
class IDispatcherThread;
using IDispatcherThreadSPtr = std::shared_ptr<IDispatcherThread>;

/*!
 * @brief   Base implementation for drivers which need to
 *          execute long-running asynchronous
 *          actions (i.e. > 50ms) off the shared driver thread.
 * @details This class creates its own private thread and dispatcher.
 *          Long-running actions can be executed on this thread
 *          by calling the BeginInvoke() method. This ensure the
 *          shared driver thread is never blocked for any significant
 *          period of time.
 */
class AsyncDeviceDriverBase : public DeviceDriverBase
{
public:
    /*! @brief  Destructor. Shuts down the private dispatcher and thread. */
    virtual ~AsyncDeviceDriverBase();

    // overrides from IDeviceDriver

    /*! @brief  Return the dispatcher associated with this driver's private thread. */
    IDispatcherSPtr GetDriverDispatcher() override;

    /*! @brief  Ignores the share driver dispatcher as the private dispatcher is used. */
    void SetDriverDispatcher( IDispatcherSPtr pDriverDispatcher ) override;

protected:
    /*! @brief  Constructor. Creates a private thread and associated dispatcher. */
    explicit AsyncDeviceDriverBase( const std::string& name, const std::string& type );
    explicit AsyncDeviceDriverBase(
        const std::string& name,
        const std::string& type,
        IDispatcherThreadSPtr pDispatcherThread );

private:
    // Private dispatcher and associated thread for exclusive use by this driver.
    IDispatcherThreadSPtr m_pDispatcherThread;
};

} //  namespace imp
