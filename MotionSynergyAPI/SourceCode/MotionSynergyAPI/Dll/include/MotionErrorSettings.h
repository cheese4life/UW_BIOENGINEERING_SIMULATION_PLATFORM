/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include "MotionErrorAction.h"

#include <sstream>

namespace imp
{

/*!
 *  @brief  Defines the motion error settings (i.e. the position error limit and the action to take if a motion error occurs).
 */
class MotionErrorSettings
{
public:
    /*!
     * @brief The position error limit in selected units (mm, um, or nm).
     *        If the actual position deviates from the commanded position by more than
     *        this value, a motion error will be generated.
     */
    double PositionErrorLimit = 0.0;

    /*!
     * @brief The action to perform when a motion error occurs.
     */
    MotionErrorAction ActionOnError = MotionErrorAction::None;

    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "PositionErrorLimit=" << PositionErrorLimit << ", ";
        ss << "ActionOnError=" << MotionErrorActionEnum::ToString( ActionOnError );

        return ss.str();
    }
};

inline std::ostream&
operator<<( std::ostream& stream, const MotionErrorSettings& value )
{
    stream << value.ToString();
    return stream;
}

} //  namespace imp
