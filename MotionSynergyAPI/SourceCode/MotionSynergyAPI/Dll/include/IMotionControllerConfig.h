/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "BiquadFilterSettings.h"
#include "CommunicationSettings.h"
#include "CurrentLoopSettings.h"
#include "ElectricalProperties.h"
#include "FirmwareVersionInfo.h"
#include "MotionErrorSettings.h"
#include "MotionTrackingSettings.h"
#include "PhaseAngleInfo.h"
#include "PositionLoopSettings.h"

#include <string>
#include <vector>

namespace imp
{
enum class MotionControllerConfigurationType : int32_t;

/*!
 *  @brief   An interface for reading and writing various motion controller settings from/to memory/NVRAM.
 */
class IMotionControllerConfiguration
{
public:
    /*!
     *   @brief      Virtual destructor (empty).
     */
    virtual ~IMotionControllerConfiguration() = default;

    /*!
     *   @brief      Read the current motion tracking settings from the motion controller's memory.
     *   @return     The current motion tracking settings.
     */
    virtual MotionTrackingSettings GetMotionTracking() = 0;

    /*!
     *  @brief      Set the motion tracking settings in the motion controller's memory.
     *  @param[in]  motionTrackingSettings The new motion tracking settings to apply.
     */
    virtual void SetMotionTracking( const MotionTrackingSettings& motionTrackingSettings ) = 0;

    /*!
     *   @brief      Read the current motion error settings from the motion controller's memory.
     *   @return     The current motion error settings.
     */
    virtual MotionErrorSettings GetMotionError() = 0;

    /*!
     *  @brief      Set the motion error settings in the motion controller's memory.
     *  @param[in]  motionErrorSettings The new motion error settings to apply.
     */
    virtual void SetMotionError( const MotionErrorSettings& motionErrorSettings ) = 0;

    /*!
     *   @brief      Read the position loop settings from the motion controller's memory.
     *   @return     The position loop settings.
     */
    virtual PositionLoopSettings GetPositionLoop() = 0;

    /*!
     *  @brief      Set the position loop settings in the motion controller's memory.
     *  @param[in]  positionLoopSettings The new position loop settings to apply.
     */
    virtual void SetPositionLoop( const PositionLoopSettings& positionLoopSettings ) = 0;

    /*!
     *   @brief      Read the current loop settings from the motion controller's memory.
     *   @return     The current loop settings.
     */
    virtual CurrentLoopSettings GetCurrentLoop() = 0;

    /*!
     *  @brief      Set the current loop settings in the motion controller's memory.
     *  @param[in]  currentLoopSettings The new current loop settings to apply.
     */
    virtual void SetCurrentLoop( const CurrentLoopSettings& currentLoopSettings ) = 0;

    /*!
     *   @brief      Read the biquad filter settings from the motion controller's memory.
     *   @return     The biquad filter settings.
     */
    virtual BiquadFilterSettings GetBiquadFilters() = 0;

    /*!
     *  @brief      Set the biquad filter settings in the motion controller's memory.
     *  @param[in]  biquadFilterSettings The new biquad filter settings to apply.
     */
    virtual void SetBiquadFilters( const BiquadFilterSettings& biquadFilterSettings ) = 0;

    /*!
     *   @brief      Read the current communication settings.
     *   @details    These will correspond to the settings in the motion controller's memory and NVRAM, unless there
     *               has been a call to SetCommunication(). In this case, it will return the updated settings supplied to SetCommunication(),
     *               which are buffered, pending a call to WriteToNVRAM().
     *   @return     The current communication settings.
     */
    virtual CommunicationSettings GetCommunication() = 0;

    /*!
     *  @brief      Set the communication settings to be written to NVRAM (note the motion controller's memory is NOT updated).
     *  @param[in]  communicationSettings The new communication settings to apply when WriteToNVRAM() is called.
     */
    virtual void SetCommunication( const CommunicationSettings& communicationSettings ) = 0;

    /*!
     *  @brief      Transfer the specified types of configuration settings from the motion controller's memory to NVRAM so they will persist across power cycles.
     *  @details    Note that the axis needs to be de-energized (disabled) before calling this function.
     *              If the active communication settings are changed, the communications link needs to be re-established (i.e. disconnect followed by a connect or initialize).
     *  @param[in]  configurationTypes The types of configuration settings to persist.
     */
    virtual void WriteToNVRAM( std::vector<MotionControllerConfigurationType> configurationTypes ) = 0;

    /*!
     *  @brief      Restore from a PMD configuration script to the Magellan IC's working memory.
     *              Note that the axis needs to be de-energized (disabled) before calling this function.
     *  @param[in]  pathname The path to the PMD configuration script file to read.
     */
    virtual void Restore( std::string pathname ) = 0;

    /*!
     *  @brief      Save the Magellan IC's NVRAM configuration as a PMD configuration script.
     *              Note that the axis needs to be de-energized (disabled) before calling this function.
     *  @param[in]  pathname The path to the PMD configuration script to write.
     */
    virtual void Save( std::string pathname ) = 0;

    /*!
     *  @brief      Restore from a PMD configuration script to the Magellan IC's NVRAM configuration.
     *              Note that the axis needs to be de-energized (disabled) before calling this function.
     *  @param[in]  pathname The path to the PMD configuration script file to read.
     */
    virtual void RestoreToNVRAM( std::string pathname ) = 0;

    /*!
     *  @brief      Get firmware version from the PMD controller.
     *  @return     The firmware version.
     */
    virtual FirmwareVersionInfo GetFirmwareVersion() = 0;

    /*!
     * @brief       Get the current phase angle from the PMD controller.
     * @return      The phase angle information.
     */
    virtual PhaseAngleInfo GetPhaseAngle() = 0;

    /*!
     * @brief     Set the phase angle to be written to the PMD controller.
     * @param[in] rawPhaseAngle The raw phase angle to set.
     */
    virtual void SetPhaseAngle( uint16_t rawPhaseAngle ) = 0;

    /*!
     * @brief   Get the current value associated with commutation parameter 1 from the PMD controller.
     * @return  The commutation parameter 1 value.
     * @details Note that this value is equal to the PhaseAngle * Prescalar factor.
     */
    virtual int32_t GetCommutationParameter1() = 0;

    /*!
     * @brief     Set the commutation parameter 1 value to be written to the PMD controller.
     * @param[in] commutationParameter1 The value to set.
     * @details   Note that this value should be equal to the PhaseAngle * Prescalar factor.
     */
    virtual void SetCommutationParameter1( int32_t commutationParameter1 ) = 0;

    /*!
     * @brief       Get various electrical properties from the PMD controller.
     * @return      The electrical properties information.
     */
    virtual ElectricalProperties GetElectricalProperties() = 0;
};

} //  namespace imp
