/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "StackTrace.h"
#include <memory>
#include <sstream>
#include <vector>


namespace imp
{

//  We mostly use this type as per a LIFO stack, but for output we want to go through
//  the data structure from start to end, which is why we aren't using a std::stack over a vector.


class StackTraceEntry
{
public:
    StackTraceEntry( const std::string& functionName, const std::string& params )
        : m_functionName( functionName )
        , m_params( params )
    {
    }
    std::string
    GetFunctionName() const
    {
        return m_functionName + "()";
    }

    std::string
    GetFunctionWithParameters() const
    {
        return m_functionName + "(" + m_params + ")";
    }

private:
    std::string m_functionName;
    std::string m_params;
};
typedef std::vector<StackTraceEntry> StackTraceEntries;

static thread_local std::shared_ptr<StackTraceEntries> s_pStackTraceEntries;

StackTrace::StackTrace( const std::string& functionName, const std::string& params )
{
    std::string cleanFuncName = functionName;

#ifdef __GNUC__
    // Extract just ClassName::FunctionName from the entire function signature provided by gcc __PRETTY_FUNCTION__
    size_t funcOpenBracketPos = cleanFuncName.find( '(' );
    cleanFuncName = cleanFuncName.substr( 0, funcOpenBracketPos );
    size_t colonColonPos = cleanFuncName.find_last_of( ':' );

    if ( colonColonPos != std::string::npos )
    {
        std::string className = cleanFuncName.substr( 0, colonColonPos - 1 );
        size_t classStartPos = className.find_last_of( ':' );

        if ( classStartPos == std::string::npos )
        {
            classStartPos = className.find_last_of( ' ' );
        }

        if ( classStartPos != std::string::npos )
        {
            cleanFuncName = cleanFuncName.substr( classStartPos + 1 );
        }
    }
    else
    {
        // Handle static functions with no class or namespace
        size_t funcNameStartPos = cleanFuncName.find_last_of( ' ' );

        if ( funcNameStartPos != std::string::npos )
        {
            cleanFuncName = cleanFuncName.substr( funcNameStartPos + 1 );
        }
    }

#endif

    if ( !s_pStackTraceEntries.get() )
    {
        s_pStackTraceEntries.reset( new StackTraceEntries );
    }

    s_pStackTraceEntries->push_back( StackTraceEntry( cleanFuncName, params ) );
}

StackTrace::~StackTrace()
{
    //  ensure we don't attempt to remove an element that doesn't exist.
    if ( !s_pStackTraceEntries->empty() )
    {
        s_pStackTraceEntries->pop_back();
    }
}

std::string
StackTrace::GetCurrentFunctionName()
{
    return s_pStackTraceEntries && !s_pStackTraceEntries->empty() ? s_pStackTraceEntries->back().GetFunctionName() : "";
}

std::string
StackTrace::GetCurrentFunctionWithParameters()
{
    return s_pStackTraceEntries && !s_pStackTraceEntries->empty()
               ? s_pStackTraceEntries->back().GetFunctionWithParameters()
               : "";
}

std::string
StackTrace::GetIndentationString( uint32_t indentationSize /*= 3*/ )
{
    return s_pStackTraceEntries && s_pStackTraceEntries->size() > 1u
               ? std::string( indentationSize * ( s_pStackTraceEntries->size() - 1 ), ' ' )
               : "";
}

std::string
StackTrace::GetStackTrace()
{
    std::ostringstream str;
    str << std::endl;

    if ( s_pStackTraceEntries )
    {
        StackTraceEntries const tempStack = *s_pStackTraceEntries;

        for ( auto&& funcEntry : tempStack )
        {
            str << funcEntry.GetFunctionWithParameters() << std::endl;
        }
    }

    return str.str();
}

} //  namespace imp
