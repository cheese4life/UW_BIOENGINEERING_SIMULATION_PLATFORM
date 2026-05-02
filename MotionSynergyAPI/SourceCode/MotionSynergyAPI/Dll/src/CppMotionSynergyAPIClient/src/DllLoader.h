/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

namespace MotionSynergyAPI
{

/*!
 *  @brief  Load/Unload a DLL and create an instance of a C++ class implemented in the DLL.
 */
class DllLoader
{
public:
    /*!
     *  @brief      Constructor. The constructor does not load the DLL, it just stores the path to the DLL.
     *  @param[in]  dllPathname          The path to the DLL to load.
     */
    DllLoader( const std::string& dllPathname );

    /*!
     *  @brief      Destructor. Calls Unload() to ensure the DLL is unloaded.
     */
    ~DllLoader();

    /*!
     *  @brief      Attempts to load the DLL.
     *  @exception Throws a std::runtime_error if the DLL cannot be loaded.
     */
    void Load();

    /*!
     *  @brief      Return an instance of a specified class/interface.
     *  @details    Will call Load() if the DLL hasn't been loaded already.
     *  @param[in]  typeName Name of the type to create (the dll must have declared '[typeName]_allocator' and '[typeName]_deleter' to perform new/delete on the type instance.
     *  @return     An instance of the class/interface on success.
     *  @exception  Throws a std::runtime_error if the DLL cannot be loaded or the class/interface cannot be created.
     */
    template<typename TInterface>
    std::shared_ptr<TInterface>
    GetInstance( const std::string& typeName )
    {
        using allocClass = TInterface* (*)();
        using deleteClass = void ( * )( TInterface* );

        // If Load() hasn't been called previously, perform a load as part of looking up the first interface.
        if ( !IsLibraryLoaded() )
        {
            Load();
        }

        const auto allocName = typeName + "_allocator";
        const auto deleteName = typeName + "_deleter";
        auto allocFunc = reinterpret_cast<allocClass>( GetFunctionAddress( allocName ) );
        auto deleteFunc = reinterpret_cast<deleteClass>( GetFunctionAddress( deleteName ) );

        if ( !allocFunc || !deleteFunc )
        {
            Unload();

            std::ostringstream errorMsg;
            errorMsg << "Can't find allocator '" << allocName << "' or deleter '" << deleteName << "' symbol in "
                     << m_dllPathname << ".";
            throw std::runtime_error( errorMsg.str() );
        }

        return std::shared_ptr<TInterface>( allocFunc(), [ deleteFunc ]( TInterface* p ) { deleteFunc( p ); } );
    }

    /*!
     *  @brief      Attempts to unload the DLL.
     */
    void Unload();

private:
    bool IsLibraryLoaded() const;
    void* GetFunctionAddress( const std::string& functionName );

    std::string m_dllPathname;
    void* m_dllHandle;
};

} // namespace MotionSynergyAPI
