/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <ostream>
#include <string>


namespace imp
{

/*!
 * @brief POD class for holding detailer information parsed from configuration.
 */
template<typename TCode>
struct ItemDetails
{
    /*!
     * @brief Constructor.
     * @param[in] code        The code that this ItemDetails describes.
     * @param[in] description A string describing the above code.
     * @param[in] translation The translation key associated with the above code.
     */
    ItemDetails( const TCode& code, const std::string& description, const std::string& translation )
    {
        Code = code;
        Description = description;
        Translation = translation;
    }

    /*! @brief The code that this ItemDetails describes. */
    TCode Code;

    /*! @brief A string describing the above code. */
    std::string Description;

    /*! @brief The translation key associated with the above code. */
    std::string Translation;
};

/*!
 * @brief    Streaming operator to automatically turn an ItemDetails into a string when output to a stream.
 */
template<typename TCode>
std::ostream&
operator<<( std::ostream& stream, const ItemDetails<TCode>& itemDetails )
{
    stream << " code=" << itemDetails.Code << ", description=" << itemDetails.Description
           << ", translation=" << itemDetails.Translation;

    return stream;
}

} //  namespace imp
