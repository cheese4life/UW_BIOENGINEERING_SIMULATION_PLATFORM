/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include <functional>
#include <stack>

namespace imp
{

/*!
 * @brief        Holds a list of 'Cleanup' tasks to be performed when this list is deallocated.
 *               Cleanup tasks are executed in reverse order (i.e. LIFO) to
 *               which they were added.
 */
class CleanupTaskList
{
public:
    /*!
     * @brief   Destructor ensures Cleanup() is called to execute all cleanup tasks in reverse order (i.e. LIFO) to
     *          which they were added.
     */
    ~CleanupTaskList() { Cleanup(); }

    /*!
     * @brief   Add a task which calls TSingleton::Destroy().
     *
     * @tparam TSingleton   The type of singleton to call Destroy() for.
     */
    template<typename TSingleton>
    void
    AddInvokeDestroy()
    {
        AddCleanupTask( [] { TSingleton::Destroy(); } );
    }

    /*!
     * @brief   Add a function which performs some sort of cleanup task
     *          when this object instance is deallocated.
     *
     * @param[in] cleanupTask   The lambda function to execute.
     */
    void
    AddCleanupTask( std::function<void()> cleanupTask )
    {
        m_cleanupTaskList.push( cleanupTask );
    }

    /*!
     * @brief   Execute all cleanup tasks in reverse order (i.e. LIFO) to
     *          which they were added.
     */
    void
    Cleanup()
    {
        while ( !m_cleanupTaskList.empty() )
        {
            m_cleanupTaskList.top()();
            m_cleanupTaskList.pop();
        }
    }

private:
    std::stack<std::function<void()>> m_cleanupTaskList;
};


} //  namespace imp
