/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <algorithm>
#include <map>

namespace imp
{

/*!
 * @brief Determines whether an item is in a generic STL container.
 * @tparam Container The STL container type.
 * @param container The container to look in.
 * @param item The item to locate in the container.
 * @return true if item is found in the container, otherwise false.
 */
template<typename Container>
bool
Contains( Container const& container, typename Container::value_type const& item )
{
    return std::find( container.begin(), container.end(), item ) != container.end();
}

/*!
 * @brief Determines whether a key is in a std::map.
 * @tparam K The map key type.
 * @tparam V The map value type.
 * @tparam KConvertible The key parameter type that is convertible to K.
 * @param map The map to look in.
 * @param key The key to locate in the map.
 * @return true if key is found in the map, otherwise false.
 */
template<typename K, typename V, typename KConvertible>
bool
Contains( std::map<K, V> const& map, KConvertible const& key )
{
    static_assert( std::is_convertible<KConvertible, K>::value, "Key must be convertible to type K" );
    return map.find( key ) != map.end();
}

} // namespace imp
