/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <memory>
#include <string>


namespace imp
{
class IConfiguration;
using IConfigurationSPtr = std::shared_ptr<IConfiguration>;

/*!
 *  @brief   Common interface for all simulators
 *  @details An simulator is a self-contained unit that will affect changes in
 *           emulated device state based on the state of one (or more) devices
 *           in the instrument controller.
 */
class ISimulator
{
public:
    ISimulator() = default;
    virtual ~ISimulator() = default;

    /*!
     *   @brief      Configures the simulator
     *   @param[in]  pConfiguration  The instance containing the simulator configuration
     *   @return     void
     */
    virtual void Configure( IConfigurationSPtr pConfiguration ) = 0;

    /*!
     *   @brief      Initialize the simulator and makes it ready for use.
     *   @return     void
     */
    virtual void Initialize() = 0;

    /*!
     *   @brief      Shutdown the simulator
     *   @return     void
     */
    virtual void Shutdown() = 0;

    /*!
     *   @brief      Name of the simulator.
     *   @return     The name of this simulator instance
     */
    virtual std::string Name() const = 0;
};

} //  namespace imp
