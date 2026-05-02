/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IDigitalIOReader.h"

namespace imp
{
// Forward declarations
enum class DigitalIOBit;
enum class DigitalIOBitState;

/*!
 *  @brief   Defines the interface a DigitalIO device must adhere to
 *           for allowing clients to write its digital information.
 */
class IDigitalIOWriter : public virtual IDigitalIOReader
{
public:
    /*!
     *   @brief  Virtual destructor (empty).
     */
    virtual ~IDigitalIOWriter() = default;

    /*!
     *   @brief      Sets the state of bit addressed by the bit index.
     *   @param[in]  bit    Enum representing the DigitalIOBit
     *   @param[in]  state  State of the DigitalIOBit
     */
    virtual void SetBitState( DigitalIOBit bit, DigitalIOBitState state ) = 0;

    /*!
     *   @brief      Sets the state of all the bits as one byte.
     *   @param[in]  data    Byte reflecting the states of the bits.
     */
    virtual void SetRegister( uint8_t data ) = 0;
};

} //  namespace imp
