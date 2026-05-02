/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <string>
#include <vector>

namespace imp
{
class IDigitalInputDevice;
class IAxisDevice;
class IDispatcher;
using IDispatcherSPtr = std::shared_ptr<IDispatcher>;
} // namespace imp

using SubscriptionId = boost::uuids::uuid;

/*!
 *   @brief  A base class used to demonstrate how to use the IAxisDevice interface to perform
 *           various operations such as Initialization, Homing and moving for each product type.
 */
class Product
{
public:
    Product( const std::string& productName )
        : m_name( productName )
    {
    }

    virtual ~Product() = default;

    /*!
     *   @brief Name of the product.
     */
    const std::string&
    Name() const
    {
        return m_name;
    }

    /*!
     *   @brief Initialize the axis.
     *   @details This will connect to each axis, perform analog calibration and
     *            commutation. Initialize must be called before homing.
     *
     *   @exception std::runtime_error if initialize fails on an axis.
     */
    virtual void Initialize() const = 0;

    /*!
     *   @brief Perform a series of moves on each axis.
     *   @details Each axis must be Initialized and Homed before general move operations can be performed.
     *
     *   @exception std::runtime_error if an action fails on either Axis.
     */
    virtual void PerformMoves() const = 0;

    /*!
     *   @brief Start monitoring the digital input device associated with an axis.
     *   @details Each axis has up to two general purpose inputs which can be monitored.
     *
     *   @exception std::runtime_error if an action fails.
     */
    void StartMonitoringDigitalInput( imp::IDispatcherSPtr pDispatcher, imp::IDigitalInputDevice& digitalInput );

    /*!
     *   @brief Stop monitoring all digital input devices (see StartMonitoringDigitalInput)
     */
    void StopMonitoringDigitalInputs();

private:
    std::string m_name;
    std::vector<std::pair<imp::IDigitalInputDevice&, SubscriptionId>> m_digitalInputSubscriptions;
};
