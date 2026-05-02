/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#include "MotionSynergyAPISingleton.h"
#include "DllLoader.h"

#include <algorithm>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#ifdef _WIN32
    #include <Windows.h>
#endif

#ifdef __GNUC__
    #include <libgen.h>
    #include <linux/limits.h>
    #include <unistd.h>
#endif
namespace MotionSynergyAPI
{

DllLoaderSPtr MotionSynergyAPISingleton::s_pDllLoader;


IMotionSynergySPtr MotionSynergyAPISingleton::s_pMotionSynergyAPI;

#ifdef __GNUC__

std::string
getExePath()
{
    char result[ PATH_MAX ];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );

    if ( count != -1 )
        return std::string( dirname( result ) );

    return std::string();
}
#endif


IMotionSynergySPtr
MotionSynergyAPISingleton::Get()
{
    if ( !s_pDllLoader )
    {


#ifdef _WIN32
        std::string dllPath = "MotionSynergyAPINative.dll";

        // If the dll cannot be located using a simple relative path from the exe.
        if ( !is_regular_file( boost::filesystem::path( dllPath ) ) )
        {
            // Convert the path to the above DLL to an absolute path, assuming it is located alongside this module (dll / exe).
            wchar_t path[ MAX_PATH ];
            HMODULE hm = nullptr;
            if ( GetModuleHandleEx(
                     GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                     reinterpret_cast<LPCWSTR>( &MotionSynergyAPISingleton::Get ),
                     &hm ) != 0 &&
                 GetModuleFileName( hm, path, sizeof( path ) ) != 0 )
            {
                std::wstring widePath( path );
                std::string asciiPath;
                asciiPath.resize( widePath.size() );
                std::transform(
                    widePath.begin(),
                    widePath.end(),
                    asciiPath.begin(),
                    []( wchar_t widechar ) { return static_cast<char>( widechar ); } );

                dllPath = ( boost::filesystem::path( asciiPath ).parent_path() / boost::filesystem::path( dllPath ) )
                              .string();
            }
        }
#elif __GNUC__
        std::string dllPath = "./libMotionSynergyAPINative.so";
        // If the dll cannot be located using a simple relative path.
        if ( !is_regular_file( boost::filesystem::path( dllPath ) ) )
        {
            // Assume it's located in the same folder as the exe.
            dllPath = getExePath() + "/libMotionSynergyAPINative.so";
        }
#endif
        const auto pDllLoader = std::make_shared<DllLoader>( dllPath );
        pDllLoader->Load();
        // If the load was successful (i.e. no exception thrown), store the pointer to the dll loader.
        s_pDllLoader = pDllLoader;
    }

    if ( !s_pMotionSynergyAPI )
    {
        s_pMotionSynergyAPI = s_pDllLoader->GetInstance<IMotionSynergy>( "IMotionSynergy" );
    }

    return s_pMotionSynergyAPI;
}

void
MotionSynergyAPISingleton::Destroy()
{
    s_pMotionSynergyAPI.reset();
    s_pDllLoader.reset();
}

} // namespace MotionSynergyAPI
