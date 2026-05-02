/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include "MotionCompleteMode.h"

#include <sstream>

namespace imp
{

/*!
 *  @brief  Defines when an axis move is determined to be complete by the motion controller.
 */
class MotionTrackingSettings
{
public:
    /*!
     * @brief Determines how the motion controller determines a move is complete.
     */
    MotionCompleteMode Mode;

    /*!
     * @brief The length of time an axis must remain within the settle window before the move is
     *        deemed to be complete. This setting is only applicable when the motion complete mode
     *        is set to 'MotionSettles'.
     */
    double SettleTimeMs;

    /*!
     * @brief The size of the settle window in selected units (mm, um, or nm).
              This setting is only applicable when the motion complete mode
              is set to 'MotionSettles'.
     */
    double SettleWindow;


    std::string
    ToString() const
    {
        std::stringstream ss;
        ss << "Mode=" << MotionCompleteModeEnum::ToString( Mode ) << ", ";
        ss << "SettleTimeMs=" << SettleTimeMs << ", ";
        ss << "SettleWindow=" << SettleWindow;

        return ss.str();
    }
};

inline std::ostream&
operator<<( std::ostream& stream, const MotionTrackingSettings& value )
{
    stream << value.ToString();
    return stream;
}

} //  namespace imp
