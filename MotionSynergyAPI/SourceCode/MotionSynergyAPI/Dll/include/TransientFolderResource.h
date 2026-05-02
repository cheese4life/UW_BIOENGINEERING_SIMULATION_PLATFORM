/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <string>

/*!
 * @brief RAII class for a transient folder.
 * @details WARNING: any pre-existing folder contents will be lost!
 *          Behaviour is based upon the concept that the referenced folder is a
 *          temporary folder, and thus its lifetime is bound to the
 *          lifetime of the instance of this class,
 *          as its existence outside of this lifetime is unintended.
 */
class TransientFolderResource
{
public:
    /*!
     * @brief   Establishes control of the lifetime of a folder.
     * @details Any existing instance of the folder is deleted.
     * @param   folderPath The path to the folder.
     * @param   create If true, an empty instance of the folder is created.
     * @throws  boost::filesystem::filesystem_error When an error occurs deleting the existing folder or creating the new folder.
     */
    explicit TransientFolderResource( const std::string& folderPath, bool create );

    /*!
     * @brief   Relinquishes control of the lifetime of a folder.
     * @details Any existing instance of the folder is destroyed.
     */
    ~TransientFolderResource();

    /*!
     * @brief   Creates the (empty) folder, if it does not exist.
     * @throws  boost::filesystem::filesystem_error When an error occurs when attempting to create the new folder.
     */
    void CreateFolder() const;

    /*!
     * @brief   Deletes the folder, if it exists.
     * @throws  boost::filesystem::filesystem_error An unexpected error occurred attempting to remove a file or folder.
     */
    void DeleteFolder() const;

    /*! @brief The path to the folder resource being controlled. */
    const std::string Path;

private:
    void DeleteFolder( const std::string& folderPath ) const;
};
