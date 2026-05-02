/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <sstream>
#include <string>


namespace imp
{

/*!
 *  @brief   Initial (limited) implementation of turning containers into a string
 */
template<typename Container>
std::string ContainerToString( Container const& container );


template<typename It>
std::string ContainerToString( It begin, It end );


/*!
 *  @brief   Initial (limited) implementation of turning containers into a string
 */
template<typename Container>
std::string
ContainerToString( Container const& container )
{
    return ContainerToString( container.begin(), container.end() );
}


/*!
 *  @brief   In case we see a string, don't treat it like a container of characters!
 */
inline std::string
ContainerToString( std::string const& container )
{
    return container;
}

template<typename T>
struct is_pair : std::false_type
{
};

template<typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type
{
};
template<typename T>
constexpr bool is_pair_v = is_pair<T>::value;

template<class T, std::enable_if_t<is_pair_v<typename std::iterator_traits<T>::value_type>, bool> = true>
void
PrintItr( std::ostream& os, T itr )
{
    os << itr->first << "=" << itr->second;
}
template<class T, std::enable_if_t<!is_pair_v<typename std::iterator_traits<T>::value_type>, bool> = true>
void
PrintItr( std::ostream& os, T itr )
{
    os << *itr;
}

template<typename It>
std::string
ContainerToString( It begin, It end )
{
    std::ostringstream ret;
    ret << "{";

    for ( auto it = begin; it != end; ++it )
    {
        PrintItr( ret, it );

        if ( std::next( it ) != end )
        {
            ret << ",";
        }
    }

    ret << "}";
    return ret.str();
}

} //  namespace imp
