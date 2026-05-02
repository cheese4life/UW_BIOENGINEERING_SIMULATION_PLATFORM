/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "ToString.h"
#include <sstream>


namespace imp
{

/*!
 *   @brief  Acts as a simple wrapper around a stringstream, to better stringify each segment.
 */
class SmartStream
{
public:
    SmartStream() = default;

    template<typename T>
    SmartStream&
    operator<<( T val )
    {
        m_stream << imp::ToString( val );
        return *this;
    }

    SmartStream&
    operator<<( SmartStream& ( *pFn )(SmartStream&))
    {
        return pFn( *this );
    }

    std::string
    str() const
    {
        return m_stream.str();
    }

private:
    std::ostringstream m_stream;

    friend SmartStream& Newline( SmartStream& );
};

inline SmartStream&
Newline( SmartStream& stream )
{
    stream.m_stream << std::endl;
    return stream;
}

} // namespace imp
