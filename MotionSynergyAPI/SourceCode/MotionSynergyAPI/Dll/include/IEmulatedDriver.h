/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDriver.h"
#include <memory>

namespace imp
{

class IEmulatedDriver;
using IEmulatedDriverSPtr = std::shared_ptr<IEmulatedDriver>;
class DriverResponseType;

/*!
 * @brief   Interface all emulated drivers must conform to. Concrete emulated driver
 *          implementations should not implement this interface directly. Instead they should
 *          inherit from an interface that implements IEmulatedDriver as appropriate, and
 *          a second interface that implements IDeviceDriver as appropriate.
 */
class IEmulatedDriver : public virtual IDriver
{
public:
    virtual ~IEmulatedDriver() = default;

    /*!
     * @brief   Set the response code to be returned for all subsequent actions.
     *          This provides a simple mechanism for injecting errors.
     *          Note that this method will also clear the current response list
     *          if present.
     * @param[in] response The response code the driver will return for all subsequent actions.
     */
    virtual void SetResponse( DriverResponseType response ) = 0;

    /*!
     * @brief   Insert a response code at the head of the sequence of responses.
     * @param[in] response The response code the driver will return for the next action.
     */
    virtual void InsertAtHeadOfResponseList( DriverResponseType response ) = 0;

    /*!
     * @brief   Set the response code to be returned for the next 'count' actions.
     *          This provides a simple mechanism for specifying a sequence of responses.
     *          Can be called multiple times to build up a varying sequence of responses.
     *          Note that once all responses have been used, the fixed response code specified
     *          by SetResponse() will be used.
     * @param[in] response The response code the driver will return for the next 'count' actions.
     * @param[in] count    The number of actions to return the response code for.
     */
    virtual void AddToResponseList( DriverResponseType response, uint32_t count ) = 0;

    /*!
     * @brief   Specifies whether a response is discarded from the response list on action
     *          completion or re-added to the end of the list.
     * @param[in] isLoopingEnabled If true, the response code will be added to the end
     *                             of the list. If false, the response code will be discarded.
     */
    virtual void SetResponseListLooping( bool isLoopingEnabled ) = 0;

    /*!
     * @brief   Gets the setting indicating if response list looping is enabled or not.
     * @returns True if response list looping is enabled, false if not.
     */
    virtual bool GetResponseListLooping() const = 0;
};

} //  namespace imp
