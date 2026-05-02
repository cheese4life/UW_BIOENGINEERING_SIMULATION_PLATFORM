/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

// Standard library
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace MotionSynergyAPI
{

class IMotionSynergyConfig
{
public:
    virtual ~IMotionSynergyConfig() = default;

    /*!
     *  @brief  Get an integer setting from the instrument configuration. Must be called after Configure().
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to retrieve.
     *  @returns The configuration setting on success.
     *  @throws An IConfigurationException on failure.
     */
    virtual int32_t GetIntegerSetting( const std::string& path ) = 0;

    /*!
     *  @brief  Update an integer setting to the local instrument configuration. Must be called after Configure().
     *  @details This method will persist the change to the local configuration file immediately.
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to update.
     *  @param[in]  value  The new value of the setting to update.
     *  @param[in]  doWriteToLocalConfig  Whether to persist to local config immediately.
     *  @throws An IConfigurationException on failure.
     */
    virtual void SetIntegerSetting( const std::string& path, int32_t value, bool doWriteToLocalConfig = true ) = 0;

    /*!
     *  @brief  Get a unsigned long setting from the instrument configuration. Must be called after Configure().
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to retrieve.
     *  @returns The configuration setting on success.
     *  @throws An IConfigurationException on failure.
     */
    virtual uint64_t GetUnsignedLongSetting( const std::string& path ) = 0;

    /*!
     *  @brief  Get a double setting from the instrument configuration. Must be called after Configure().
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to retrieve.
     *  @returns The configuration setting on success.
     *  @throws An IConfigurationException on failure.
     */
    virtual double GetDoubleSetting( const std::string& path ) = 0;

    /*!
     *  @brief  Update a double setting to the local instrument configuration. Must be called after Configure().
     *  @details This method will persist the change to the local configuration file immediately.
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to update.
     *  @param[in]  value  The new value of the setting to update.
     *  @param[in]  doWriteToLocalConfig  Whether to persist to local config immediately.
     *  @throws An IConfigurationException on failure.
     */
    virtual void SetDoubleSetting( const std::string& path, double value, bool doWriteToLocalConfig = true ) = 0;

    /*!
     *  @brief  Get an string setting from the instrument configuration. Must be called after Configure().
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to retrieve.
     *  @returns The configuration setting on success.
     *  @throws An IConfigurationException on failure.
     */
    virtual std::string GetStringSetting( const std::string& path ) = 0;

    /*!
     *  @brief  Get an string setting from the instrument configuration. Must be called after Configure().
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to retrieve.
     *  @returns The configuration setting on success.
     *  @throws An IConfigurationException on failure.
     */
    virtual std::map<std::string, std::string> GetSectionSettings( const std::string& path ) = 0;

    /*!
     *  @brief  Update a string setting to the local instrument configuration. Must be called after Configure().
     *  @details This method will persist the change to the local configuration file immediately.
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to update.
     *  @param[in]  value  The new value of the setting to update.
     *  @param[in]  doWriteToLocalConfig  Whether to persist to local config immediately.
     *  @throws An IConfigurationException on failure.
     */
    virtual void
    SetStringSetting( const std::string& path, const std::string& value, bool doWriteToLocalConfig = true ) = 0;

    /*!
     *  @brief  Get an bool setting from the instrument configuration. Must be called after Configure().
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to retrieve.
     *  @returns The configuration setting on success.
     *  @throws An IConfigurationException on failure.
     */
    virtual bool GetBoolSetting( const std::string& path ) = 0;

    /*!
     *  @brief  Update a bool setting to the local instrument configuration. Must be called after Configure().
     *  @details This method will persist the change to the local configuration file immediately.
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to update.
     *  @param[in]  value  The new value of the setting to update.
     *  @param[in]  doWriteToLocalConfig  Whether to persist to local config immediately.
     *  @throws An IConfigurationException on failure.
     */
    virtual void SetBoolSetting( const std::string& path, bool value, bool doWriteToLocalConfig = true ) = 0;

    /*!
     *  @brief  Remove a setting from the local instrument configuration. Must be called after Configure().
     *  @details This method will persist the change to the local configuration file immediately.
     *  @param[in]  path  The path (e.g. Section.Key) to the configuration setting to remove.
     *  @param[in]  doWriteToLocalConfig  Whether to persist to local config immediately.
     *  @throws An IConfigurationException on failure.
     */
    virtual void RemoveLocalSetting( const std::string& path, bool doWriteToLocalConfig = true ) = 0;

    /*!
     *  @brief  Returns whether the specified section exists in the instrument configuration. Must be called after Configure().
     *  @param[in]  path  The name of the configuration section.
     *  @returns True if section is present. False otherwise.
     *  @throws An IConfigurationException on failure.
     */
    virtual bool IsSectionPresent( const std::string& section ) = 0;

    /*!
     *  @brief  Returns whether the specified key exists in the instrument configuration section. Must be called after Configure().
     *  @param[in]  section  The name of the configuration section.
     *  @param[in]  key  The name of the configuration key.
     *  @returns True if key is present. False otherwise.
     *  @throws An IConfigurationException on failure.
     */
    virtual bool IsKeyPresent( const std::string& section, const std::string& key ) const = 0;

    /*!
     *  @brief  Returns whether the specified setting exists in the instrument configuration. Must be called after Configure().
     *  @param[in]  path  The name of the configuration setting.
     *  @returns True if setting is present. False otherwise.
     *  @throws An IConfigurationException on failure.
     */
    virtual bool IsSettingPresent( const std::string& path ) const = 0;

    /*!
     *  @brief      Returns the path to the base configuration file containing the this setting
     *              or an empty string if the settings doesn't exist.
     *  @param[in]  path  String path to setting key.
     *  @return     See above.
     */
    virtual std::string GetSettingFilename( const std::string& path ) const = 0;

    virtual std::vector<std::string> GetSectionNames() = 0;
};

} // namespace MotionSynergyAPI
