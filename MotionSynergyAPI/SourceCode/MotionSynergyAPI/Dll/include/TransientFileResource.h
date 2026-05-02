/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <string>


namespace imp
{


/*!
 * @brief RAII class for a transient file.
 * @details WARNING: any pre-existing file contents will be lost!
 *          Behaviour is based upon the concept that the referenced file is a
 *          temporary file, and thus its lifetime is bound to the
 *          lifetime of the instance of this class,
 *          as its existence outside of this lifetime is unintended.
 */
class TransientFileResource
{
public:
    /*!
     * @brief   Establishes control of the lifetime of a file.
     * @details Any existing instance of the file is destroyed.
     * @param   filePath The path to the file.
     * @param   create If true, an empty instance of the file is created.
     * @param   deleteExisting If true, any existing file will be deleted first.
     * @throws  boost::filesystem::filesystem_error When an error occurs that prevents even knowing if the file exists.
     */
    explicit TransientFileResource( const std::string& filePath, bool create, bool deleteExisting = true );

    /*!
     * @brief  Copy constructor/assignment - not allowed.
     */
    TransientFileResource( const TransientFileResource& other ) = delete;

    /*!
     * @brief  Copy assignment - not allowed.
     */
    TransientFileResource& operator=( const TransientFileResource& other ) = delete;

    /*!
     * @brief  Move constructor for return value optimisation.
     */
    TransientFileResource( TransientFileResource&& other ) noexcept;

    /*!
     * @brief Move assignment - not allowed.
     */
    TransientFileResource& operator=( TransientFileResource&& other ) noexcept = delete;

    /*!
     * @brief   Relinquishes control of the lifetime of a file.
     * @details Any existing instance of the file is destroyed.
     */
    ~TransientFileResource();

    /*!
     * @brief   Creates the (empty) file, if it does not exist.
     * @throws  boost::filesystem::filesystem_error When an error occurs that prevents even knowing if the file exists.
     */
    void CreateFile() const;

    /*!
     * @brief   Deletes the file, if it exists.
     * @throws  boost::filesystem::filesystem_error When an error occurs that prevents even knowing if the file exists.
     * @returns false if the file did not exist in the first place, otherwise true.
     */
    bool DeleteFile() const;

    /*! @brief The path to the file resource being controlled. */
    const std::string Path;
};


} //  namespace imp
