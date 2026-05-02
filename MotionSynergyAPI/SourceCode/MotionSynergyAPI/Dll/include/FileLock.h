/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once
#include <boost/filesystem/path.hpp>
#include <string>

namespace imp
{

/*! @brief Provides cross-platform support for exclusive locking of a file.
 *  @details On linux this involves setting the immutable attribute which requires superuser or sudo permission.
 *           On windows this involves opening a file with exclusive access.
 */
class FileLock
{
public:
    /*! @constructor Store the filename to lock. Does not attempt to lock the file at this point.
     *   @param[in] filename     Full pathname to the file to lock.
     */
    FileLock( const boost::filesystem::path& filename );

    /*! @destructor Calls Unlock() to ensure the file is unlocked.
     */
    ~FileLock();

    /*! @brief Attempt to gain an exclusive lock on the file.
     *   @returns true if successful, false if not.
     */
    bool Lock();

    /*! @brief Unlock the file. Safe to call even if Lock() has not been called.
     */
    void Unlock();

private:
    boost::filesystem::path m_filename;
#ifdef _WIN32
    void* m_handle;
#else
    int32_t m_fd;
#endif
};

} //  namespace imp
