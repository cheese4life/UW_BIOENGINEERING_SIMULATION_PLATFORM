/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "Logger.h"

#include "IMotionSynergy.h"
#include "MotionSynergyAPISingleton.h"

#include <stdexcept>

#ifdef MOTIONSYNERGYAPI_DLL

namespace imp
{

static ILogger&
GetLogger()
{
    return MotionSynergyAPI::MotionSynergyAPISingleton::Get()->GetLogger();
}

const std::string ILogger::DefaultChannelName = "diagnostics";
const std::string ILogger::DefaultComponentName = "Application";


void
Logger::Create(
    LoggerType /*type*/,
    const std::string& /*configurationFile*/,
    const std::string& /*logOutputFolder*/,
    ILogger::OpenLogCallbackFn /*openLogCallback*/,
    ILogger::CloseLogCallbackFn /*closeLogCallback*/ )
{
    throw std::logic_error(
        "Logger::Create should not be called from a client application when using the MotionSynergy DLL." );
}

void
Logger::Destroy()
{
    throw std::logic_error(
        "Logger::Destroy should not be called from a client application when using the MotionSynergy DLL." );
}

void
Logger::Rollover()
{
    GetLogger().Rollover();
}

void
Logger::Rollover( const std::string& channel )
{
    GetLogger().Rollover( channel );
}

void
Logger::SetTriggerBasedRotation( std::function<bool()> pred )
{
    GetLogger().SetTriggerBasedRotation( pred );
}

void
Logger::SetTriggerBasedRotation( std::function<bool()> pred, const std::string& channel )
{
    GetLogger().SetTriggerBasedRotation( pred, channel );
}

void
Logger::SetOpenLogCallback( ILogger::OpenLogCallbackFn callback )
{
    GetLogger().SetOpenLogCallback( callback );
}

void
Logger::SetCloseLogCallback( ILogger::CloseLogCallbackFn callback )
{
    GetLogger().SetCloseLogCallback( callback );
}

void
Logger::AddChannel( const std::string& channel )
{
    GetLogger().AddChannel( channel );
}

LogLevel
Logger::GetLogLevel()
{
    return GetLogger().GetLogLevel();
}

void
Logger::SetLogLevel( LogLevel level, bool persist )
{
    GetLogger().SetLogLevel( level, persist );
}

void
Logger::Trace(
    const std::string& component,
    const std::string& channel,
    const std::string& sourceLocation,
    const std::string& rhs )
{
    GetLogger().Trace( component, channel, sourceLocation, rhs );
}

void
Logger::Debug(
    const std::string& component,
    const std::string& channel,
    const std::string& sourceLocation,
    const std::string& rhs )
{
    GetLogger().Debug( component, channel, sourceLocation, rhs );
}

void
Logger::Info(
    const std::string& component,
    const std::string& channel,
    const std::string& sourceLocation,
    const std::string& rhs )
{
    GetLogger().Info( component, channel, sourceLocation, rhs );
}

void
Logger::Warning(
    const std::string& component,
    const std::string& channel,
    const std::string& sourceLocation,
    const std::string& rhs )
{
    GetLogger().Warning( component, channel, sourceLocation, rhs );
}

void
Logger::Error(
    const std::string& component,
    const std::string& channel,
    const std::string& sourceLocation,
    const std::string& rhs )
{
    GetLogger().Error( component, channel, sourceLocation, rhs );
}

void
Logger::Fatal(
    const std::string& component,
    const std::string& channel,
    const std::string& sourceLocation,
    const std::string& rhs )
{
    GetLogger().Fatal( component, channel, sourceLocation, rhs );
}

} //  namespace imp

#endif // MOTIONSYNERGYAPI_DLL
