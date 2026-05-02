/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "EnumClass.h"

namespace imp
{

/*!
 *  @brief   Defines the trigger to begin a trace data capture.
 */
enum class TraceTrigger : int32_t
{
    Immediate,     /*!< Trace data capture begins immediately and continues until StopTraceCapture() is called. */
    OnMotionStart, /*!< Trace data capture begins at the start of the next move and ends when the move completes. */
    OnMotionStartDelayed, /*!< Trace data capture begins at a delayed time after the start of the next move and ends when the move completes. This is only supported by the UDP sine wave move.*/ 
    OnMotionStartNoEnd, /*!< Trace data capture begins at the start of the next move and continues until StopTraceCapture() is called. */
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( TraceTrigger )

} //  namespace imp
