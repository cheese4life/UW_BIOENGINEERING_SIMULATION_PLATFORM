/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "DigitalIOBasicTypes.h"

#include <memory>

namespace imp
{
// Forward declarations
class IDispatcher;

/*!
 *  @brief   Defines the information required to subscribe for notification
 *           of Digital IO Change
 */
class DigitalIOChangeSubscription
{
public:
    /*!
     *   @brief  Constructor for subscription based on individual bit
     *   @param[in]  pDispatcher  The Dispatcher through which the notification
     *               is to be delivered
     *   @param[in]  bit         The bit for which notification of change is required
     *   @param[in]  callback    The callback to be invoked to notify of change
     */
    DigitalIOChangeSubscription( std::shared_ptr<IDispatcher> pDispatcher, const DigitalIORegisterCallback& callback )
        : m_pDispatcher( pDispatcher )
        , m_isRegisterSubscription( true )
        , m_bit()
        , m_bitCallback()
        , m_registerCallback( callback )
    {
    }

    /*!
     *   @brief  Constructor for subscription based on whole of register change
     *   @param[in]  pDispatcher  The Dispatcher through which the notification
     *               is to be delivered
     *   @param[in]  callback    The callback to be invoked to notify of change
     */
    DigitalIOChangeSubscription(
        std::shared_ptr<IDispatcher> pDispatcher,
        const DigitalIOBit bit,
        const DigitalIOBitCallback& callback )
        : m_pDispatcher( pDispatcher )
        , m_isRegisterSubscription( false )
        , m_bit( bit )
        , m_bitCallback( callback )
        , m_registerCallback()
    {
    }

    DigitalIOBit
    GetBit() const
    {
        return m_bit;
    }

    bool
    IsRegisterSubscription() const
    {
        return m_isRegisterSubscription;
    }

    const std::shared_ptr<imp::IDispatcher>&
    GetDispatcher() const
    {
        return m_pDispatcher;
    }

    const DigitalIOBitCallback&
    GetBitCallback() const
    {
        return m_bitCallback;
    }

    const DigitalIORegisterCallback&
    GetRegisterCallback() const
    {
        return m_registerCallback;
    }

private:
    const std::shared_ptr<IDispatcher> m_pDispatcher;
    const bool m_isRegisterSubscription;
    const DigitalIOBit m_bit;
    const DigitalIOBitCallback m_bitCallback;
    const DigitalIORegisterCallback m_registerCallback;
};

} //  namespace imp
