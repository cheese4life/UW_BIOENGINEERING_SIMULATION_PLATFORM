/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <cstdint>
#include <memory>
#include <mutex>

namespace imp
{
class ReportedAlert;
typedef std::shared_ptr<const ReportedAlert> ReportedAlertConstSPtr;
} // namespace imp

namespace MotionSynergyAPI
{

/*!
 *  @brief  Class to register for alerts and capture the first
 *          medium (i.e. Severity >= Medium) alert raised.
 *          Used in the wrappers around native method calls invoked
 *          from the Managed C++ via the MotionSynergyAPI public interface.
 */
class AlertCapture
{
public:
    /*!
     *  @brief  Constructor which registers for alerts by default.
     *   @param[in]  doRegister    Register for alerts during construction (true by default)
     */
    AlertCapture( bool doRegister = true );

    /*!
     *  @brief  Destructor calls Deregister() to unregister for alerts.
     */
    virtual ~AlertCapture();

    /*!
     *  @brief  Manually register for alerts. Only needed if the constructor parameter doRegister was set to false.
     */
    void Register();

    /*!
     *  @brief  Flush and manually deregister for alerts. Allows deregistration prior to the destructor which is useful when
     *          wrapping methods which tear down infrastructure such as AlertDistribution. Indicates to AlertCapture that
     *          AlertDistribution will be destroyed.
     *  @exception Throws a std::runtime_error if the AlertDistribution dispatcher is not running.
     *  @exception Throws a std::runtime_error if called from a different thread to the AlertDistribution thread.
     *  @exception Throws a std::runtime_error if AlertDistribution::Flush has timed out.
     */
    void Shutdown();

    /*!
     *  @brief  Determines if an error alert (i.e. Severity >= Medium) has been raised.
     *  @returns true if an error alert has been raised, false if not.
     *  @exception Throws a std::runtime_error if the AlertDistribution dispatcher is not running.
     *  @exception Throws a std::runtime_error if called from a different thread to the AlertDistribution thread.
     *  @exception Throws a std::runtime_error if AlertDistribution::Flush has timed out.
     */
    bool WasErrorRaised() const;

    /*!
     *  @brief  Returns the pointer to the alert which was raised.
     *  @returns The pointer of the first medium severity or higher alert, or a nullptr if no alert has been raised.
     */
    imp::ReportedAlertConstSPtr GetNativeAlert() const;

    /*!
     *  @brief  Flush all alerts which have not been dispatched by AlertDistribution.
     *  @exception Throws a std::runtime_error if the AlertDistribution flush times out after 60 seconds.
     */
    static void WaitForAlerts();

private:
    void Deregister();

    imp::ReportedAlertConstSPtr m_pAlert = nullptr;
    uint32_t m_alertRegistrationId = 0;
    std::mutex m_alertRegistrationMutex;
    bool m_isFlushRequired;
};


} // namespace MotionSynergyAPI
