/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDigitalIOPublisher.h"

namespace imp
{
// Forward declarations
enum class DigitalIOBit;
enum class DigitalIOBitState;

/*!
 *  @brief   Defines the interface a DigitalIO device must adhere to
 *           for allowing clients to read its digital information.
 */
class IDigitalIOReader : public virtual IDigitalIOPublisher
{
public:
    /*!
     *   @brief  Virtual destructor (empty).
     */
    virtual ~IDigitalIOReader() = default;

    /*!
     *   @brief      Gets the state of bit addressed by the bit index.
     *   @param[in]  bit    Enum representing the DigitalIOBit
     *   @param[in]  forceRefresh    Forces retrieval of value from the remote
     *                               device as opposed to from a local cache.
     *   @return     State of the DigitalIOBit
     */
    virtual DigitalIOBitState GetBitState( DigitalIOBit bit, bool forceRefresh = false ) = 0;

    /*!
     *   @brief      Gets the state of all the bits as one byte.
     *   @param[in]  forceRefresh    Forces retrieval of value from the remote
     *                               device as opposed to from a local cache.
     *   @return     Byte reflecting the states of the bits.
     */
    virtual uint8_t GetRegister( bool forceRefresh = false ) = 0;
};

} //  namespace imp
