/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "DllLoader.h"

#include <iostream>


namespace MotionSynergyAPI
{

DllLoader::DllLoader( const std::string& dllPathname )
    : m_dllPathname( dllPathname )
    , m_dllHandle( nullptr )
{
}

DllLoader::~DllLoader()
{
    Unload();
}

bool
DllLoader::IsLibraryLoaded() const
{
    return m_dllHandle != nullptr;
}

#ifdef _WIN32

    #include <Windows.h>

void
DllLoader::Load()
{
    m_dllHandle = LoadLibraryA( m_dllPathname.c_str() );
    if ( m_dllHandle == nullptr )
    {
        std::ostringstream errorMsg;
        errorMsg << "Unable to load library " << m_dllPathname << ".";
        throw std::runtime_error( errorMsg.str() );
    }
}

void
DllLoader::Unload()
{
    if ( m_dllHandle != nullptr )
    {
        if ( FreeLibrary( static_cast<HMODULE>( m_dllHandle ) ) == 0 )
        {
            // Don't throw or assert here as this method is called from the destructor.
            std::cerr << "Unable to free library " << m_dllPathname << ".";
        }
        m_dllHandle = nullptr;
    }
}
void*
DllLoader::GetFunctionAddress( const std::string& functionName )
{
    return GetProcAddress( static_cast<HMODULE>( m_dllHandle ), functionName.c_str() );
}

#endif // _WIN32

#ifdef __GNUC__

    #include <dlfcn.h>

void
DllLoader::Load()
{
    m_dllHandle = dlopen( m_dllPathname.c_str(), RTLD_LOCAL | RTLD_LAZY );
    if ( m_dllHandle == nullptr )
    {
        std::ostringstream errorMsg;
        errorMsg << "Unable to load library " << m_dllPathname << ".";
        throw std::runtime_error( errorMsg.str() );
    }
}

void
DllLoader::Unload()
{
    if ( m_dllHandle != nullptr )
    {
        if ( dlclose( m_dllHandle ) != 0 )
        {
            // Don't throw or assert here as this method is called from the destructor.
            std::cerr << "Unable to free library " << m_dllPathname << ".";
        }
        m_dllHandle = nullptr;
    }
}
void*
DllLoader::GetFunctionAddress( const std::string& functionName )
{
    return dlsym( m_dllHandle, functionName.c_str() );
}


#endif // __GNUC__

} // namespace MotionSynergyAPI
