/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <cstdint>
#include <memory>

namespace imp
{
class AlertFilterByCodeAndSource;
using AlertFilterByCodeAndSourceSPtr = std::shared_ptr<AlertFilterByCodeAndSource>;
} // namespace imp

namespace MotionSynergyAPI
{

/*!
 *  @brief   Class to temporarily ignore a specific alert code. It uses RAII to add the
 *           filter during construction and remove the filter during destruction.
 *  @details All AlertIgnore instances must be destroyed before the AlertDistribution
 *           singleton is destroyed.
 */
class AlertIgnore
{
public:
    /*!
     *  @brief  Constructor creates and adds a filter to AlertDistribution.
     *   @param[in]  alertCode    The AlertCode to ignore.
     *
     *   @exception Throws a std::logic_error if the AlertDistribution singleton
     *              has not been created yet or has been destroyed.
     */
    AlertIgnore( uint16_t alertCode );

    /*!
     *  @brief   Destructor removes the filter from AlertDistribution.
     *  @details Must be called before the AlertDistribution singleton
     *           is destroyed, otherwise an exception will be thrown and
     *           the behaviour from this point on will be undefined.
     */
    virtual ~AlertIgnore();

private:
    imp::AlertFilterByCodeAndSourceSPtr m_pFilter;
};

} // namespace MotionSynergyAPI
