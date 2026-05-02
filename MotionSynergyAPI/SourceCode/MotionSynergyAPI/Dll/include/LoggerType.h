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

/*! @brief   The type of logger to instantiate. */
enum class LoggerType
{
    Unknown = 0, /*!< Reserved to cause poor implementation to fail early. */
    NullLogger,  /*!< A mock/null logger for testing purposes. */
    BoostLog,    /*!< Default implementation based on boost::log library. */
    LAST_ENUM
};

ENUMCLASS_FUNCTIONS_DECL( LoggerType )

} //  namespace imp
