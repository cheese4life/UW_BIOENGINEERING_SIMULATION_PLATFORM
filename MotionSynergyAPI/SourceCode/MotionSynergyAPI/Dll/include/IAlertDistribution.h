/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "AlertDeclarations.h"
#include "AsyncCallback.h"
#include <functional>
#include <vector>


namespace imp
{

// Typedef the alert callback for convenience
typedef std::function<void( ReportedAlertConstSPtr )> OnAlertCallback;
typedef IPLKeystone::AsyncCallback<::Alert::ReportId> AlertCreatorCallback;

class IAlertFilter;
using IAlertFilterSPtr = std::shared_ptr<IAlertFilter>;

/*!---------------------------------------------------------------------------
 * @class   IAlertDistribution
 * @brief   Provides interface to raise and clear alerts
 */
class IAlertDistribution
{
public:
    using RegistrationId = uint32_t;

    IAlertDistribution() = default;
    virtual ~IAlertDistribution() = default;

    /*!---------------------------------------------------------------------------
     * @brief  Configures the distribution and its children (AlertDetailer)
     */
    virtual void Configure() = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Registers a callback to be notified when an alert is RAISED
     * @param  AlertCallback - the function that will be called upon a alert being raised
     * @return A unique id that identifies the registration, should be stored and used to deregister
     */
    virtual RegistrationId RegisterAlertRaised( OnAlertCallback AlertCallback ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Registers a callback to be notified when an alert is RAISED
     * @param  AlertCallback - the function that will be called upon a alert being raised
     * @param  source - Only Alerts from this source will trigger the AlertCallback.
     * @return A unique id that identifies the registration, should be stored and used to deregister
     */
    virtual RegistrationId RegisterAlertRaised( OnAlertCallback AlertCallback, const std::string& source ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Deregister the callback corresponding to the provided id
     * @param  registrationId - id of the registration to be canceled
     */
    virtual void DeregisterAlertRaised( RegistrationId registrationId ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Registers a callback to be notified when an alert is CLEARED
     * @param  AlertCallback - the function that will be called upon a alert being cleared
     * @return A unique id that identifies the registration, should be stored and used to deregister
     */
    virtual RegistrationId RegisterAlertCleared( OnAlertCallback AlertCallback ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Registers a callback to be notified when an alert is CLEARED
     * @param  AlertCallback - the function that will be called upon a alert being cleared
     * @param  source - Only Alerts from this source will trigger the AlertCallback.
     * @return A unique id that identifies the registration, should be stored and used to deregister
     */
    virtual RegistrationId RegisterAlertCleared( OnAlertCallback AlertCallback, const std::string& source ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Deregister the callback corresponding to the provided id
     * @param  registrationId - id of the registration to be canceled
     */
    virtual void DeregisterAlertCleared( RegistrationId registrationId ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Clears all the current 'live' alerts. Called on initialize for example.
     * @param[in]  notifySubscribers - Should alert cleared subscribers be notified of the clears.
     * @details 'Live' alerts are those raised and not cleared
     * @returns true if one or more 'live' alerts existed and were cleared, false if there
     *          were no alerts to clear.
     */
    virtual bool ClearAllAlerts( bool notifySubscribers = true ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Clears all the current 'live' alerts for the subsystem specified.
     * @param[in]  source - results are filtered to those Alerts raised by this source.
     * @param[in]  notifySubscribers - Should alert cleared subscribers be notified of the clears.
     * @details 'Live' alerts are those raised and not cleared
     * @returns true if one or more 'live' alerts raised by the source were cleared,
     *          false if there were no alerts raised by the source to clear.
     */
    virtual bool ClearAllAlerts( const std::string& source, bool notifySubscribers = true ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Raises an Alert
     * @param  timeStamp - epoch timestamp in seconds.
     * @param  source - the unique identifier of the component that raised the alert
     * @param  code - the alert code to be raised
     * @param  severity - the alert severity to be raised
     * @param  additionalDetails - free form extra detail added by the 'raiser'
     * @param  contextID - the id that represent the context that the alert occurred in. Eg. the sample id
     * @param  callback - callback to notify the source when Alert is cleared (implies Source wll not clear).
     * @returns An Alert ReportId which can be passed to ClearAlert() to clear the alert if required.
     */
    virtual ::Alert::ReportId RaiseAlert(
        AlertTimestamp timeStamp,
        const std::string& source,
        const RawAlertCode& code,
        ::Alert::Severity::Enum severity,
        const std::string& additionalDetails,
        uint16_t contextID,
        AlertCreatorCallback* callback ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Raises an Alert, using the alert details to look up the severity.
     * @param  timeStamp - epoch timestamp in seconds.
     * @param  source - the unique identifier of the component that raised the alert
     * @param  code - the alert code to be raised
     * @param  additionalDetails - free form extra detail added by the 'raiser'
     * @param  contextID - the id that represent the context that the alert occurred in. Eg. the sample id
     * @param  callback - callback to notify the source when Alert is cleared (implies Source wll not clear).
     * @returns An Alert ReportId which can be passed to ClearAlert() to clear the alert if required.
     */
    virtual ::Alert::ReportId RaiseAlert(
        AlertTimestamp timeStamp,
        const std::string& source,
        const RawAlertCode& code,
        const std::string& additionalDetails,
        uint16_t contextID,
        AlertCreatorCallback* callback ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Clears a specific alert instance by id
     * @param  reportID - the unique identifier of the alert instance to be cleared
     * @return True if the alert existed and was cleared
     */
    virtual bool ClearAlert( ::Alert::ReportId reportID ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Returns a list of Active Alerts.
     */
    virtual std::vector<ReportedAlertConstSPtr> GetActiveAlerts() const = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Returns a list of Active Alerts.
     * @param  source - results are filtered to those Alerts raised by this source.
     */
    virtual std::vector<ReportedAlertConstSPtr> GetActiveAlerts( const std::string& source ) const = 0;

    /*!---------------------------------------------------------------------------
     * @brief  Returns a list of registered alert filters.
     * @returns A vector containing a copy of the list of registered alert filters.
     *          This vector will be empty if there are no registered alert filters.
     */
    virtual std::vector<IAlertFilterSPtr> GetAlertFilters() const = 0;

    /*!---------------------------------------------------------------------------
     * @brief     Insert an Alert Filter into the list of registered alert filters.
     * @param[in] pFilter     - The filter to insert.
     * @param[in] pNextFilter - An existing registered alert filter to insert the filter before or
     *                          nullptr (default) if the filter is to be added to the end of the list.
     */
    virtual void InsertAlertFilter( IAlertFilterSPtr pFilter, IAlertFilterSPtr pNextFilter = nullptr ) = 0;

    /*!---------------------------------------------------------------------------
     * @brief     Remove an Alert Filter from the list of registered alert filters.
     * @param[in] pFilter     - The filter to remove.
     * @exception Throws a std::runtime_error if the filter is not currently registered.
     */
    virtual void RemoveAlertFilter( IAlertFilterSPtr pFilter ) = 0;

    /*! @brief  Block forever and wait for dispatcher flush to complete.
     */
    static const std::chrono::milliseconds INFINITE_TIMEOUT;

    /*!---------------------------------------------------------------------------
     * @brief     Calls dispatcher flush to wait for pending alerts on the current queue to get called.
     * @param[in] timeout_ms     - The duration to wait for.
     * @return    True if flush has completed or false if timed out.
     * @exception Throws a std::runtime_error if the dispatcher is not running
     *            or Flush is called from a different thread to the dispatcher thread.
     */
    virtual bool Flush( std::chrono::milliseconds timeout_ms = INFINITE_TIMEOUT ) = 0;
};


typedef std::shared_ptr<IAlertDistribution> IAlertDistributionSPtr;

} //  namespace imp
