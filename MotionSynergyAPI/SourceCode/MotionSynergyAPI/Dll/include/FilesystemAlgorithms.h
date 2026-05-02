/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <string>
#include <vector>

namespace imp
{
class FileSystemAlgorithms
{
public:
    /*!
     * @brief Checks to see if a directory exists at the specified path.
     * @param[in] directoryPath The path to the directory.
     * @returns True if the directory exists, false if not.
     */
    static bool IsDirectory( boost::filesystem::path const& directoryPath );

    /*!
     * @brief Gets the total size of the matching files in the directory
     * @param[in] directoryPath The path to the directory.
     * @param[in] re            A regular expression which can be used to match specific filenames in the directory.
     * @returns The total size of the matching files in the directory.
     */
    static uint64_t GetDirectorySize( boost::filesystem::path const& directoryPath, boost::regex const& re );

    /*!
     * @brief Gets a list of all entries (files/sub-directories) in the directory.
     * @param[in] directoryPath The path to the directory.
     * @returns A vector of all entries (files/sub-directories) in the directory.
     */
    static std::vector<std::string> GetDirectoryContents( boost::filesystem::path const& directoryPath );

    /*!
     * @brief Return the path to the folder containing the DLL/executable this function resides in.
     * @details Not currently implemented on Linux. Will return an empty string in this case.
     * @returns The absolute path to this DLL/executable on success, an empty string on failure.
     */
    static std::string GetDllFolder();
};
} // namespace imp
