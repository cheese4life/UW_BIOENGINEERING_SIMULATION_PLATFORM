/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <string>
#include <vector>

namespace imp
{

/// @brief
/// A simple utility class for reading and writing text files.
///
class TextFile
{
public:
    static std::vector<std::string> ReadContents( const std::string& filename );
    static void WriteContents( const std::string& filename, const std::vector<std::string>& lines );
};

} // namespace imp
