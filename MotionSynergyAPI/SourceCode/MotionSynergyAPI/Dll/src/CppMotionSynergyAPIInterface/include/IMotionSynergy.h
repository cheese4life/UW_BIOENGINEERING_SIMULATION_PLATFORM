/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ProductInfo.h"

#include <memory>
#include <string>
#include <vector>

namespace imp
{
class IDigitalInputDevice;
class IAxisDevice;
class IConfiguration;
using IConfigurationSPtr = std::shared_ptr<IConfiguration>;
class IDispatcher;
using IDispatcherSPtr = std::shared_ptr<IDispatcher>;
class IDispatcherThread;
using IDispatcherThreadSPtr = std::shared_ptr<IDispatcherThread>;
class ILogger;
} // namespace imp

namespace MotionSynergyAPI
{

class IMotionSynergy;
using IMotionSynergySPtr = std::shared_ptr<IMotionSynergy>;
class IMotionSynergyConfig;

/*!
 *   @brief  A simplified version of MotionSynergyAPI interface for C++ Applications responsible for
 *   creating the infrastructure (e.g. configuration, logging, etc) necessary to control
 *   one or more Axis devices.
 */
class IMotionSynergy
{
public:
    /*!
     *   @brief  Destructor calls Shutdown() to ensure the infrastructure is destroyed.
     */
    virtual ~IMotionSynergy() = default;

    /*!
     *   @brief  Create the necessary infrastructure, read the instrument configuration and create the axis device instances.
     *
     *   @param[in]  instrumentConfigPathname      Path to the main 'instrument' configuration file.
     *   @param[in]  logConfigPathname          Path to the logging configuration file.
     *   @param[in]  logFolderPathname          Path to write log files to.     *
     *
     *   @exception Various exceptions will be thrown in the case an error is encountered (e.g. cannot load the configuration).
     */
    virtual void Configure(
        const std::string& instrumentConfigPathname,
        const std::string& logConfigPathname,
        const std::string& logFolderPathname ) = 0;

    /*!
     *   @brief  Shut down the infrastructure in a controlled manner.
     */
    virtual void Shutdown() = 0;

    /*!  @brief  Return a list of products defined in the configuration.
     *   @details Configure() must have been called first.
     *
     *   @returns A vector of ProductInfo structures, one per product defined in configuration.
     */
    virtual std::vector<ProductInfo> GetProductList() = 0;

    /*!
     *   @brief  Return a reference to an axis.
     *   @details Configure() must have been called first.
     *
     *   @param[in]  axisName      Name of the axis to return.
     *
     *   @exception std::runtime_error if the axis cannot be found (i.e. axisName doesn't match an axis defined in configuration).
     */
    virtual imp::IAxisDevice& GetAxis( const std::string& axisName ) const = 0;


    /*!
     *   @brief  Check if a digital input has been enabled in configuration and is available for use.
     *   @details Configure() must have been called first.
     *
     *   @param[in]  digitalInputName      Name of the digital input to check.
     *
     *   @returns true if the digital input is available for use, false if the digital input cannot be found or is still disabled in configuration.
     */
    virtual bool IsDigitalInputEnabled( const std::string& digitalInputName ) const = 0;

    /*!
     *   @brief  Return a reference to a digital input.
     *   @details Configure() must have been called first.
     *
     *   @param[in]  digitalInputName      Name of the digital input to return.
     *
     *   @exception std::runtime_error if the digital input cannot be found (i.e. digitalInputName doesn't match a digital input defined in configuration).
     */
    virtual imp::IDigitalInputDevice& GetDigitalInput( const std::string& digitalInputName ) const = 0;

    /*!
     *  @brief Get a pointer to the main dispatcher interface.
     *  Creates the dispatcher and associated thread on the first call to this method. Subsequent calls
     *  will return the same instance.
     *  @returns The main dispatcher.
     */
    virtual imp::IDispatcherSPtr GetDispatcher() = 0;

    /*!
     *  @brief  Determine the first product listed in the configuration file and its associate axis/axes.
     *  @param[out]  productAxesNames    Output parameter which will contain a list of axes associated with this product.
     *  @returns The product type and a list of associated axes on success, 'Undefined' and an empty list on failure.
     */
    virtual std::string GetFirstProduct( std::vector<std::string>& productAxesNames ) = 0;

    /*!
     *  @brief  Get a reference to the main logger interface.
     *  @returns The main logger.
     */
    virtual imp::ILogger& GetLogger() = 0;

    /*!
     *  @brief  Get a reference to the configuration interface.
     *  @returns An interface to the configuration.
     */
    virtual IMotionSynergyConfig& GetConfiguration() = 0;
};

} // namespace MotionSynergyAPI
