/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "EnumClass.h"
#include "EnumClassMapper.h"
#include <cstdint>


namespace imp
{

/*!
 * @brief  Flags to identify whether an element is positioned at the start or end of a collection
 *         Note that sole elements should be multiflagged: Start | End.
 */
enum class SiblingPositionFlag : uint8_t
{
    Middle,
    Start,
    End,
    LAST_ENUM
};
ENUMCLASS_FUNCTIONS_DECL( SiblingPositionFlag );


SiblingPositionFlag operator|( SiblingPositionFlag flag1, SiblingPositionFlag flag2 );

SiblingPositionFlag operator&( SiblingPositionFlag flag1, SiblingPositionFlag flag2 );

} //  namespace imp
