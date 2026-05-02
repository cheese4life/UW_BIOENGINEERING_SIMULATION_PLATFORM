/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "DigitalIOBasicTypes.h"

#include "CommonAssert.h"
#include "EnumClassMapper.h"


#define ENUM_NAMESPACE imp::DigitalIOBitState
DECLARE_ENUMCLASS_MAPPER( DigitalIOBitState ){ ENUM_STRING( LOW ), ENUM_STRING( HIGH ) };
namespace imp
{
ENUMCLASS_FUNCTIONS_IMPL( DigitalIOBitState )
}
#undef ENUM_NAMESPACE


#define ENUM_NAMESPACE imp::DigitalIOBit
DECLARE_ENUMCLASS_MAPPER( DigitalIOBit ){
    ENUM_STRING( ZERO ),
    ENUM_STRING( ONE ),
    ENUM_STRING( TWO ),
    ENUM_STRING( THREE ),
    ENUM_STRING( FOUR ),
    ENUM_STRING( FIVE ),
    ENUM_STRING( SIX ),
    ENUM_STRING( SEVEN ) };
namespace imp
{
ENUMCLASS_FUNCTIONS_IMPL( DigitalIOBit )
}
#undef ENUM_NAMESPACE


namespace imp
{

uint8_t
BitMask( DigitalIOBit bit )
{
    static const uint8_t MASK[] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80 };
    const auto index = static_cast<uint32_t>( bit );
    ASSERT_ARG_VALID_MSG( index < DigitalIOBitEnum::Size(), "Out-of-range bit index passed to BitMask!" );
    return MASK[ index ];
}

} //  namespace imp