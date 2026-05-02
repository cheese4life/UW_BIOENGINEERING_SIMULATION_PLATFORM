/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <cstdint>


namespace imp
{

/*!
 *   @brief  Constructed with a max call value, each call of the function object returns whether
 *           this value has been hit (and the object then resets its count)
 */
class CounterPredicate
{
public:
    explicit CounterPredicate( uint32_t maxCount )
        : m_maxCount{ maxCount }
    {
    }

    /*!
     *   @brief  Returns true after being called maxCount times; resets current count.
     */
    bool
    operator()()
    {
        if ( m_currentCount++ > m_maxCount )
        {
            m_currentCount = 0;
            return true;
        }

        return false;
    }

private:
    uint32_t m_maxCount;
    uint32_t m_currentCount = 0;
};

} // namespace imp
