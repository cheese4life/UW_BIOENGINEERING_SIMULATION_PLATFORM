/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "IConfiguration.h"


namespace imp
{

/*! @brief ConfigurationFailedIntegrityException exception type
 */
struct ConfigurationFailedIntegrityException : std::runtime_error
{
    explicit ConfigurationFailedIntegrityException( const char* msg )
        : runtime_error( msg )
    {
    }
    explicit ConfigurationFailedIntegrityException( const std::string& msg )
        : runtime_error( msg )
    {
    }
    virtual ~ConfigurationFailedIntegrityException() = default;
};


/*!
 *  @brief     Defines an interface for retrieving instrument calibration
 */
class IntegrityCheckedIniConfiguration
    : public IConfiguration
    , boost::noncopyable
{
public:
    /*! @brief Read the configuration from the specified path
     *
     *  @note ExGuarantee : basic
     *
     *  @throws ptree_bad_path : if the path does not exist
     *  @throws ConfigurationFailedIntegrityException : If the format of the file is incorrect or
     *          CRC doesn't match
     *
     *  @param  defaultPathname : The path to the global ini file
     *  @param  localPathname   : The path to the local override INI file or containing folder to load.
     */
    IntegrityCheckedIniConfiguration( const std::string& defaultPathname, const std::string& localPathname );

    /*! @brief Read the configuration from a stream
     *
     *  @note ExGuarantee : basic
     *
     *  @throws ptree_bad_path : if the path does not exist
     *  @throws ConfigurationFailedIntegrityException : If the format of the file is incorrect or
     *          CRC doesn't match
     *
     *  @param[in] inputStream   The input stream containing the ini file
        @param[in] localPathname The path to the local override INI file to write changes to.
     */
    explicit IntegrityCheckedIniConfiguration( std::istream& inputStream, const std::string& localPathname );

    virtual ~IntegrityCheckedIniConfiguration();

    /*! @brief converts an existing ini file to the integrity checked ini file
     *         or updates an existing integrity checked ini file
     *
     *  @note ExGuarantee : basic
     *
     *  @throws ptree_bad_path : if the path does not exist
     *  @throws ConfigurationFailedIntegrityException : If the format of the file is incorrect or
     *          CRC doesn't match
     *
     *  @param filePath : The path to the normal ini file
     */
    static void CreateIntegrityCheckedIniConfiguration( const std::string& filePath );

protected:
    /*! @brief Checks the data integrity of the setting defined by the supplied path
     *
     *  @note ExGuarantee : basic
     *
     *  @throws ptree_bad_path : if the path does not exist
     *  @throws ConfigurationFailedIntegrityException : If the format of the file is incorrect or
     *          CRC doesn't match
     *
     *  @param path : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     *  @return bool: true if the setting passed the integrity check
     */
    bool CheckSettingIntegrity( const std::string& path ) const override;

    /*! @brief Adds a CRC to the given local setting.
     *
     *  @note ExGuarantee : basic
     *
     *  @throws ptree_bad_path : if the path does not exist
     *
     *  @param path : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     */
    void IntegrityProtectLocalSetting( const std::string& path ) override;


private:
    void CheckAllSettingIntegrity();
    static std::string CalculateCRC( const std::string& path, const std::string& fullSettingValueStr );
    static void CleanupValue( std::string& value );
};

} //  namespace imp
