/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

namespace imp
{
/*!
 *  @brief   An interface for performing User Defined Profile (UDP) moves and updating the associated user defined profile data.
 */
class IUserDefinedProfile
{
public:
    /*!
     *   @brief      Virtual destructor (empty).
     */
    virtual ~IUserDefinedProfile() = default;

    /*!
     *   @brief      Move axis using the currently selected time-based UDP profile.
     *               A time-based UDP profile must have first been selected by calling SelectMoveProfile().
     *               If a time-based UDP profile is not active, this method will return an error.
     *   @param[in]  repeat     Number of times to repeat move.
     */
    virtual void MoveRelativeUDP( uint32_t repeat ) = 0;

    /*!
     *   @brief      Update a single position at the specified index in the currently selected user defined profile.
     *   @param[in]  index     Zero-based index into the profile data array.
     *   @param[in]  position  The new position in selected units (mm, um, or nm) to write to this index in the profile data array.
     */
    virtual void UpdateProfileData( uint32_t index, double position ) = 0;

    /*!
     * @brief   Get the current data stream index being used by the motion controller to calculate the target position of the axis during a UDP move.
     * @return  The current data stream index.
     */
    virtual uint32_t GetCurrentDataStreamIndex() = 0;

    /*!
     *   @brief      Select the sine wave UDP profile and setup to be ready for a relative UDP move.
     *   @details    Note that setting a parameter too high can cause a device error.
     *   @param[in]  frequencyHz         The frequency of the sine wave in Hz.
     *   @param[in]  amplitude           The amplitude of the sine wave in selected units (mm, um, or nm).
     *   @param[in]  angularAcceleration The angular acceleration in Hz per sec.
     */
    virtual void SelectSineWaveMoveProfile( double frequencyHz, double amplitude, double angularAcceleration ) = 0;
};

} //  namespace imp
