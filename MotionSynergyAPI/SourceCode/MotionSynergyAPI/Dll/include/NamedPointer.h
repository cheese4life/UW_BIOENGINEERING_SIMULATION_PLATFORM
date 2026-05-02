/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>

namespace imp
{

/*!
 * @brief  Generic helper function to turn a pointer into a hex string.
 */
template<typename T>
std::string
NamedPointer( T const* const ptr )
{
    // print the hex representation of the pointer address - pad the address with zeros and
    // pad according to the width of the pointer type
    std::ostringstream address;
    address << "0x" << std::hex << std::setfill( '0' ) << std::setw( sizeof( void* ) * 2 ) << std::uppercase
            << reinterpret_cast<std::intptr_t const>( ptr );
    return address.str();
}

/*!
 * @brief  Specialisation to handle nullptr (since otherwise it cannot deduce the T).
 */
inline std::string
NamedPointer( decltype( nullptr ) )
{
    void* pVal = nullptr;
    return NamedPointer( pVal );
}

} //  namespace imp
