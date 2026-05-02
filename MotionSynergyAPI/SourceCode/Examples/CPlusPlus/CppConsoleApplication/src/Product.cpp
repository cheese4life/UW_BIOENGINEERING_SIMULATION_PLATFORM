/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "Product.h"

#include <DigitalIOChangeSubscription.h>
#include <IAxisDevice.h>
#include <IDigitalInputDevice.h>
#include <LoggerMacros.h>

const std::string ComponentName( "Product" );

void
Product::StartMonitoringDigitalInput( imp::IDispatcherSPtr pDispatcher, imp::IDigitalInputDevice& digitalInput )
{
    auto inputName = digitalInput.Name();
    auto callback = [ inputName ]( uint8_t registerValue )
    { LOG_INFO( inputName << " digital input state change: " << static_cast<uint16_t>( registerValue ) ) };

    LOG_INFO( "Monitoring " << inputName << "." )

    // Subscribe for digital input state changes
    auto subscriptionId =
        digitalInput.SubscribeChangeNotification( imp::DigitalIOChangeSubscription( pDispatcher, callback ) );
    m_digitalInputSubscriptions.emplace_back( digitalInput, subscriptionId );

    // Enable digital input change notifications
    digitalInput.EnableChangeNotification();
    digitalInput.Wait();
}

void
Product::StopMonitoringDigitalInputs()
{
    for ( auto& subscriber : m_digitalInputSubscriptions )
    {
        subscriber.first.UnsubscribeChangeNotification( subscriber.second );
    }
    m_digitalInputSubscriptions.clear();
}
