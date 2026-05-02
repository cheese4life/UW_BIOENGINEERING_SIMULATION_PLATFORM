/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonAssert.h"
#include "IDispatcher.h"
#include <boost/range/adaptor/map.hpp>
#include <boost/uuid/random_generator.hpp>
#include <functional>
#include <map>

namespace imp
{
/*!
 * @brief     Responsible for implementing the publisher/subscriber pattern,
 *            for any payload parameters, for any number of subscribers.
 * @tparam    Args    The payload parameters.
 */
template<typename... Args>
class CallbackContainer
{
public:
    CallbackContainer() = default;
    ~CallbackContainer() = default;

    using SubscriptionId = boost::uuids::uuid;
    using FunctionType = std::function<void( Args... )>;

    /*!
     *   @brief      Subscribes a functor into the container
     *   @param[in]  func         The functor to subscribe
     *   @param[in]  pDispatcher  The dispatcher on which to invoke notifications for the functor.
     *                            Must NOT be nullptr .
     *   @return     The subscription identifier
     */
    SubscriptionId
    Subscribe( FunctionType func, const IDispatcherSPtr& pDispatcher )
    {
        // We enforce the use of a dispatcher to ensure clients
        // are considering the threading implications of their callbacks.
        ASSERT_MSG( pDispatcher, "pDispatcher must not be nullptr!" );

        typename SubscriptionIdToContainerEntryMap::value_type entry(
            m_subscriptionIdGenerator(),
            { func, pDispatcher } );
        const auto insertResult = m_subscriptionMap.insert( entry );
        ASSERT_MSG( insertResult.second, "Attempted to insert an item with a duplicate subscription ID" );

        return entry.first;
    }

    /*!
     *   @brief      Unsubscribes a functor from the container by an identifier
     *   @param[in]  subscriptionId  The subscription ID to unsubscribe
     *   @return     "true" if the functor was unsubscribed
     */
    bool
    Unsubscribe( SubscriptionId subscriptionId )
    {
        typename SubscriptionIdToContainerEntryMap::size_type removed = m_subscriptionMap.erase( subscriptionId );
        return ( removed != 0 );
    }

    /*!
     *   @brief      Calls all functors with the given arguments
     *   @param[in]  args    Variadic function argument listing to pass into all subscribed functors
     *   @return     void
     */
    void
    operator()( Args... args )
    {
        Call( args... );
    }

    /*!
     *   @brief      Calls all functors with the given arguments
     *   @param[in]  args    Variadic function argument listing to pass into all subscribed functors
     *   @return     void
     */
    void
    Call( Args... args )
    {
        for ( const auto& subscription : this->m_subscriptionMap | boost::adaptors::map_values )
        {
            subscription.pDispatcher->BeginInvoke( std::bind( subscription.func, args... ) );
        }
    }

    bool
    IsEmpty() const
    {
        return m_subscriptionMap.empty();
    }

private:
    boost::uuids::random_generator m_subscriptionIdGenerator;
    struct ContainerEntry
    {
        FunctionType func;
        const IDispatcherSPtr pDispatcher;
    };

    using SubscriptionIdToContainerEntryMap = std::map<SubscriptionId, ContainerEntry>;
    SubscriptionIdToContainerEntryMap m_subscriptionMap;
};

} //  namespace imp
