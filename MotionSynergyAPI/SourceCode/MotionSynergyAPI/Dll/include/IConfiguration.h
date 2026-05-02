/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonExceptions.h"
#include "StringConversion.h"

#include <boost/optional/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/regex.hpp>

#include <memory>
#include <string>
#include <vector>

namespace imp
{

const std::string ConfigurationComponentName( "Configuration" );

class IConfiguration;

/*! @brief Short-hand definition for shared pointer to IConfiguration */
using IConfigurationSPtr = std::shared_ptr<IConfiguration>;

/*! @brief Defines the supported configuration types
 *
 *   @details    Currently types tagged as integrity checked mean that the file has an
 *               integrity check as well as each setting
 */
//  TODO - make this stringable
typedef enum CFG_TYPE
{
    CFG_UNKNOWN,    /*!< Reserved to cause poor implementation to fail early. */
    CFG_INI_FILE,   /*!< Standard INI configuration read from a file - no integrity checking. */
    CFG_INI_STREAM, /*!< Standard INI configuration read from an istream source - no integrity checking. */
    CFG_INTEGRITY_CHECKED_INI_FILE,  /*!< Integrity-checked INI configuration read from a file. */
    CFG_INTEGRITY_CHECKED_INI_STREAM /*!< Integrity-checked INI configuration read from an istream source. */
} CFG_TYPE;

/*! @brief IConfigurationException exception type definition. */
struct IConfigurationException : std::runtime_error
{
    explicit IConfigurationException( const char* msg )
        : runtime_error( msg )
    {
    }
    explicit IConfigurationException( const std::string& msg )
        : runtime_error( msg )
    {
    }
    virtual ~IConfigurationException() = default;
};

/*! @brief  The delimiter used to separate hierarchical configuration keys. */
constexpr char CFG_DELIMITER_CHAR{ '.' };
const std::string CFG_DELIMITER( &CFG_DELIMITER_CHAR, 1 );

/*! @brief The CRC format regular expression */
const boost::regex CRC_FORMAT( "(.{1,})\\|([[:alnum:]]*)[[:space:]]*$" );

/*!
 *  @brief     Provides an interface to a configuration system, exposes static factory
 *             methods read config by type
 *  @details   Provide a static interface to get a IConfiguration instance based on a
 *             resource descriptor and a type
 */
class IConfiguration
{
private:
    /*!
     *  @brief  Inner class of IConfiguration for managing the storage of setting values.
     */
    class IniConfigurationTree
    {
    public:
        /*! @brief  Constructor. */
        IniConfigurationTree() = default;

        /*!
         *  @brief      Read configuration file into configuration trees.
         *  @param[in]  defaultFilePath  Path to default configuration file.
         *  @param[in]  localFilePath  Path to local configuration file.
         *  @throws     IConfigurationException  If invalid configuration file format.
         */
        void ParseConfig( const std::string& defaultFilePath, const std::string& localFilePath );

        /*!
         *  @brief      Read configuration stream into configuration trees.
         *  @param[in]  inputStream  Stream containing default configuration string.
         *  @param[in]  localFilePath  Path to local configuration file.
         *  @throws     IConfigurationException  If invalid configuration format.
         */
        void ParseConfig( std::istream& inputStream, const std::string& localFilePath );

        /*!
         *  @brief      Write local configuration to local configuration file.
         *  @throws     IConfigurationException  If filepath was not set or
         *                                       error writing configuration.
         */
        void WriteLocalConfig() const;

        /*!
         *  @brief      Write default configuration to default configuration file.
         *  @throws     IConfigurationException  If filepath was not set or
         *                                       error writing configuration.
         */
        void WriteDefaultConfig() const;

        /*!
         *  @brief      Retrieve setting string value based on highest setting precedence.
         *  @param[in]  path  String path to setting key.
         *  @return     Optional raw string representation of setting value.
         */
        boost::optional<std::string> GetSettingRawString( const std::string& path ) const;

        /*!
         *  @brief      Retrieve setting string value from a section based on highest setting precedence.
         *  @param[in]  section  Top level section name.
         *  @param[in]  key  Key name.
         *  @return     Optional raw string representation of setting value.
         */
        boost::optional<std::string> GetSettingRawString( const std::string& section, const std::string& key ) const;

        /*!
         *  @brief      Returns true if setting is found in any configuration tree.
         *  @param[in]  path  String path to setting key.
         *  @return     See above.
         */
        bool SettingExists( const std::string& path ) const;

        /*!
         *  @brief      Returns true if setting is found in local configuration tree.
         *  @param[in]  path  String path to setting key.
         *  @return     See above.
         */
        bool LocalSettingExists( const std::string& path ) const;

        /*!
         *  @brief      Returns the path to the base configuration file containing the this setting
         *              or an empty string if the settings doesn't exist.
         *  @param[in]  path  String path to setting key.
         *  @return     See above.
         */
        std::string GetSettingFilename( const std::string& path ) const;

        /*!
         *  @brief   Returns a collection of section names from the default configuration tree.
         *  @return  See above.
         */
        std::vector<std::string> GetSectionNames() const;

        /*!
         *  @brief      Removes a setting key or section in the local configuration tree.
         *  @param[in]  path  String path to setting key or section.
         *  @return     Number of direct children removed from the given path.
         */
        size_t RemoveLocalSetting( std::string const& path );

        /*!
         *  @brief      Execute a function against every setting in every configuration tree.
         *  @param[in]  verifyPathCb  Callback function taking in a string path parameter
         *                            containing the setting path.
         *  @throws     ICongifurationException  If configuration tree is invalid.
         */
        void VerifyAllSettings( std::function<void( const std::string& )> verifyPathCb ) const;

        /*!
         *  @brief      Writes a string dump of every setting to a stream.
         *  @details    The stream will contain a line for each setting in the following format:
         *              <path.to.setting> = <value> | <default/local>: <source_config_file>
         *  @param[in]  stream  Output stream to store the string.
         */
        void Dump( std::ostream& stream ) const;

        /*!
         *  @brief      Returns a setting key-value map of a given section,
         *              based on highest setting precedence.
         *  @tparam     T  Setting value type to return.
         *  @param[in]  section  Section name.
         *  @return     See above.
         *  @throws     IConfigurationException  If type conversion fails.
         */
        template<typename T>
        std::map<std::string, T>
        GetSettingMap( const std::string& section ) const
        {
            try
            {
                std::map<std::string, T> map;

                // Inner function - add setting from a config pair to map
                auto AddToMap = [ &section, &map ]( const ConfigurationPair& configPair )
                {
                    // Read config tree in order - local overrides default
                    for ( const auto& tree : { configPair.m_defaultTree, configPair.m_localTree } )
                    {
                        const auto& sectionNode = tree.get_child_optional( section );

                        if ( !sectionNode )
                        {
                            continue;
                        }

                        for ( const auto& settingNode : *sectionNode )
                        {
                            const auto& key = settingNode.first;
                            map[ key ] = settingNode.second.template get_value<T>();
                        }
                    }
                };

                // ------------------------------------------------------------
                // Go through all config tree pairs in order, looking for matching section

                AddToMap( m_mainConfig );

                for ( const auto& includedConfig : m_includedConfigs )
                {
                    // Read includes in order - later includes override previous
                    for ( const auto& configPair : includedConfig.second )
                    {
                        AddToMap( configPair );
                    }
                }

                return map;
            }
            catch ( const boost::property_tree::ptree_error& e )
            {
                THROW_EXCEPTION(
                    "Exception thrown while reading map of settings. Section: " << section
                                                                                << ", exception info: " << e.what(),
                    IConfigurationException );
            }
        }

        /*!
         *  @brief      Store a setting or overwrite an existing setting in the
         *              local configuration tree.
         *  @param[in]  path  String path to setting.
         *  @param[in]  value  Setting value to store.
         *  @throws     IConfigurationException  If type conversion fails.
         */
        template<typename T>
        void
        SetLocalSetting( const std::string& path, const T& value )
        {
            try
            {
                auto ptreePath = boost::property_tree::ptree::path_type( path, CFG_DELIMITER_CHAR );
                auto& configPair = GetConfigurationPair( ptreePath );
                configPair.m_localTree.put( ptreePath, value );
                configPair.m_localSettingsModified = true;
            }
            catch ( const boost::property_tree::ptree_error& e )
            {
                THROW_EXCEPTION(
                    "IConfiguration: Exception thrown while setting local setting. Path: "
                        << path << ", exception info: " << e.what(),
                    IConfigurationException );
            }
        }

        /*!
         *  @brief      Store a setting or overwrite an existing setting in the
         *              default (factory) configuration tree.
         *  @param[in]  path  String path to setting.
         *  @param[in]  value  Setting value to store.
         *  @throws     IConfigurationException  If type conversion fails.
         */
        template<typename T>
        void
        SetDefaultSetting( const std::string& path, const T& value )
        {
            try
            {
                auto ptreePath = boost::property_tree::ptree::path_type( path, CFG_DELIMITER_CHAR );
                auto& configPair = GetConfigurationPair( ptreePath );
                configPair.m_defaultTree.put( ptreePath, value );
            }
            catch ( const boost::property_tree::ptree_error& e )
            {
                THROW_EXCEPTION(
                    "IConfiguration: Exception thrown while setting default setting. Path: "
                        << path << ", exception info: " << e.what(),
                    IConfigurationException );
            }
        }

    private:
        struct ConfigurationPair;
        void ParseIncludes();
        const ConfigurationPair& GetConfigurationPair( const boost::property_tree::ptree::path_type& path ) const;
        ConfigurationPair& GetConfigurationPair( const boost::property_tree::ptree::path_type& path );
        boost::optional<std::string>
        GetSettingRawStringInternal( const boost::property_tree::ptree::path_type& path ) const;
        ConfigurationPair CreateIncludedConfigurationPair(
            const std::string& defaultFilePath,
            const std::string& localFilePath,
            const std::string& sectionName );

    private:
        /*! @brief  Holds data for a default-local configuration pair. */
        struct ConfigurationPair
        {
            /*! @brief  The parsed contents of the loaded configuration. */
            boost::property_tree::ptree m_defaultTree;
            /*! @brief  The parsed contents of the loaded configuration from local overrides. */
            boost::property_tree::ptree m_localTree;
            /*! @brief  File path to the default config file. */
            std::string m_defaultFilePath;
            /*! @brief  File path to local config file to write local tree settings to. */
            std::string m_localFilePath;
            /*! @brief  Flag to indicate when the local tree was modified since it was first created. */
            bool m_localSettingsModified{ false };
        };

        /*! @brief  Configuration pair of the main config. */
        ConfigurationPair m_mainConfig;

        /*! @brief  Collection of configuration pairs of included config files. */
        using SectionName = std::string;
        std::map<SectionName, std::vector<ConfigurationPair>> m_includedConfigs;
        static const std::string RootSectionName;
        static const std::string IncludeFileKeyPrefix;
    };

    // --------------------------------------------------------------------------------------------
    // IConfiguration declaration

public:
    IConfiguration() = default;
    virtual ~IConfiguration() = default;

    /*! @brief Template method to get a setting, with a default value if it is not found.
     *
     *         Use this method if there is a MEANINGFUL default to use if the setting
     *         is not found, the default will be silently used.
     *         If the setting not existing in the configuration is important (there is no
     *         meaningful default) then use the other GetSetting method
     *
     *  @param path : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     *  @param defaultValue : the value to be returned if the setting is not found in the configuration
     *  @return T : the value corresponding to the 'key' param
     */
    template<typename T>
    T
    GetSetting( const std::string& path, const T& defaultValue ) const
    {
        auto retVal = GetOptionalSetting<T>( path );
        return retVal ? *retVal : defaultValue;
    }

    /*! @brief Template method to get a setting.
     *
     *         Use this method if there is not a MEANINGFUL default to use if the setting
     *         is not found, the default will be silently used.
     *         If the setting not existing in the configuration is important (there is no
     *         meaningful default) then use the other GetSetting method
     *
     *  @throws IConfigurationException :
     *                        if the path does not exist
     *                        if the value could not be translated to T
     *                        if CRC is not ok
     *
     *  @param path : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     *  @return T : the value corresponding to the 'key' param
     */
    template<typename T>
    T
    GetSetting( const std::string& path ) const
    {
        auto retVal = GetOptionalSetting<T>( path );

        if ( !retVal )
        {
            THROW_EXCEPTION( "Setting not found: " << path, IConfigurationException );
        }

        return *retVal;
    }

    /*! @brief Template method to get a setting.
     *
     *         Use this method if it is not important/significant is the setting does not exist.
     *         If the setting does not existing an uninitialized optional will be returned
     *
     *  @throws IConfigurationException :
     *                        if the path does not exist
     *                        if the value could not be translated to T
     *                        if CRC is not ok
     *
     *  @param path : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     *  @return boost::optional<T> : an optional wrapping the value corresponding to the 'key' param
     */
    template<typename T>
    boost::optional<T>
    GetOptionalSetting( const std::string& path ) const
    {
        if ( path.empty() )
        {
            // an empty path will return the root of the property tree
            // the root should not be 'got' as it should not have a value
            THROW_EXCEPTION( "Empty path used, cannot get setting on root node", IConfigurationException );
        }

        auto retVal = m_configTree.GetSettingRawString( path );

        if ( !retVal )
        {
            return boost::none;
        }

        // there is a setting at the supplied path so check the data integrity
        if ( !CheckSettingIntegrity( path ) )
        {
            // an empty path will return the root of the property tree
            // the root should not be 'got' as it should not have a value
            THROW_EXCEPTION(
                "The integrity check failed on the setting, if the source is valid then corruption has occurred in "
                "RAM ",
                IConfigurationException );
        }

        return boost::optional<T>( RemoveCrc<T>( path, retVal.get() ) );
    }

    /*! @brief Template method to get a setting containing a delimiter-separated list of values.
     *
     *  @param[in] path : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     *  @param[in] delimiters : a string of delimiters used to separate values (default delimiter is a comma).
     *  @param[in] trimWhitespace : remove whitespace from the beginning and end of each value (default is to remove whitespace).
     *  @return  A vector of values of type T.
     *  @throws IConfigurationException :
     *                        if the path is empty
     *                        if the value could not be translated to T
     *                        if CRC is not ok
     */
    template<typename T>
    std::vector<T>
    GetSettingList( const std::string& path, const std::string& delimiters = ",", bool trimWhitespace = true ) const
    {
        auto settingListString = GetSetting<std::string>( path, std::string() );
        boost::algorithm::trim( settingListString );

        std::vector<std::string> settingList;

        if ( !settingListString.empty() )
        {
            boost::algorithm::split( settingList, settingListString, boost::is_any_of( delimiters ) );
        }

        if ( trimWhitespace )
        {
            for ( auto&& settingString : settingList )
            {
                boost::algorithm::trim( settingString );
            }
        }

        std::vector<T> nativeTypeSettingList;

        for ( auto&& settingString : settingList )
        {
            nativeTypeSettingList.push_back( FromString<T>( settingString ) );
        }

        return nativeTypeSettingList;
    }

    /*! @brief Template method to get a setting from a section.
     *
     *         Use this method if there is not a MEANINGFUL default to use if the setting
     *         is not found, the default will be silently used.
     *         If the setting not existing in the configuration is important (there is no
     *         meaningful default) then use the other GetSettingFromSection method
     *
     *  @details This method is useful for dealing with section names containing the default
     *           configuration delimiter as GetSetting is unable to resolve the path correctly.
     *
     *  @throws IConfigurationException :
     *                        if the path does not exist
     *                        if the value could not be translated to T
     *                        if CRC is not ok
     *
     *  @param section : the top level section name
     *  @param key : the key name
     *  @return T : the value corresponding to the 'key' param
     */
    template<typename T>
    T
    GetSettingFromSection( const std::string& section, const std::string& key )
    {
        auto retVal = GetOptionalSettingFromSection<T>( section, key );

        if ( !retVal )
        {
            THROW_EXCEPTION( "Setting not found - Section: " << section << " Key: " << key, IConfigurationException );
        }

        return *retVal;
    }

    /*! @brief Template method to get a setting from a section, with a default value if it is not found.
     *
     *         Use this method if there is a MEANINGFUL default to use if the setting
     *         is not found, the default will be silently used.
     *         If the setting not existing in the configuration is important (there is no
     *         meaningful default) then use the other GetSettingFromSection method
     *
     *  @details This method is useful for dealing with section names containing the default
     *           configuration delimiter as GetSetting is unable to resolve the path correctly.
     *
     *  @param section : the top level section name
     *  @param key : the key name
     *  @param defaultValue : the value to be returned if the setting is not found in the configuration
     *  @return T : the value corresponding to the 'key' param
     */
    template<typename T>
    T
    GetSettingFromSection( const std::string& section, const std::string& key, const T& defaultValue )
    {
        auto retVal = GetOptionalSettingFromSection<T>( section, key );

        if ( !retVal )
        {
            return defaultValue;
        }

        return *retVal;
    }

    /*! @brief  Write the entire contents of the parsed configuration tree to a target stream.
        @param[in,out]  stream  The stream to write the configuration to.
    */
    void Dump( std::ostream& stream ) const;

    /*! @brief Writes contents of local config to a file.
     *
     *  @throws IConfigurationException if the file cannot be written.
     */
    void WriteLocalConfig() const;

    /*! @brief Template method to add a setting to the configuration.
     *
     *  This setting is only in memory. If WriteLocalConfig is called, it will be written to disk.
     *
     *  This function will overwrite existing local settings.
     *
     *  @param path  : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     *  @param value : the value to store.
     *  @throws IConfigurationException : if the path is empty or the value could not be translated to T
     */
    template<typename T>
    void
    SetLocalSetting( const std::string& path, const T& value )
    {
        if ( path.empty() )
        {
            THROW_EXCEPTION( "Path given to SetLocalSetting is empty.", IConfigurationException );
        }

        m_configTree.SetLocalSetting( path, value );
        IntegrityProtectLocalSetting( path );
    }

    /*! @brief Writes contents of default config to a file.
     *
     *  @throws IConfigurationException if the file cannot be written.
     */
    void WriteDefaultConfig() const;

    /*! @brief Template method to add a setting to the configuration.
     *
     *  This setting is only in memory. If WriteDefaultConfig is called, it will be written to disk.
     *
     *  This function will overwrite existing default settings.
     *
     *  @param path  : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     *  @param value : the value to store.
     *  @throws IConfigurationException : if the path is empty or the value could not be translated to T
     */
    template<typename T>
    void
    SetDefaultSetting( const std::string& path, const T& value )
    {
        if ( path.empty() )
        {
            THROW_EXCEPTION( "Path given to SetDefaultSetting is empty.", IConfigurationException );
        }

        m_configTree.SetDefaultSetting( path, value );
    }

    /*! @brief Gets a map of maps of key/value pairs from the configuration file.
     *         Configuration format is as follows:
     *         valueMapListKey=Map1Name,Map2Name,etc
     *         [Map1Name]
     *         Key1=Value1
     *         Key2=Value2
     *         ...
     *
     *  @param[in] valueMapListKey : the fully qualified path to the key which holds the list of map names eg. [Section].etc.[Section].[Key]
     *  @returns A map of mapName : map (key/value pairs), or an empty map if not found.
     *  @throws IConfigurationException if a map value element could not be converted.
     *          Does NOT throw if the key could not be found.
     */
    template<typename T>
    std::map<std::string, std::map<std::string, T>>
    GetMapOfValueMaps( const std::string& valueMapListKey ) const
    {
        std::map<std::string, std::map<std::string, T>> results;
        auto sectionNames = GetSettingList<std::string>( valueMapListKey );

        for ( auto&& sectionName : sectionNames )
        {
            if ( results.find( sectionName ) != results.end() )
            {
                THROW_EXCEPTION(
                    "IConfiguration: Duplicate value map section name '" << sectionName << "' in '" << valueMapListKey
                                                                         << "'.",
                    IConfigurationException );
            }

            results[ sectionName ] = GetSettingMap<T>( sectionName );
        }

        return results;
    }

    /*! @brief Template method to update a map of maps containing key/value pairs.
     *         Configuration format is as follows:
     *         valueMapListKey=Map1Name,Map2Name,etc
     *         [Map1Name]
     *         Key1=Value1
     *         Key2=Value2
     *         ...
     *
     *  These updates are only stored in memory. If WriteLocalConfig is called, they will be written to disk.
     *
     *  This function will overwrite existing local settings.
     *
     *  @param[in] valueMapListKey      : the fully qualified path to the key which holds the list of map names eg. [Section].etc.[Section].[Key]
     *  @param[in] valueMapList         : the map of mapName : map (key/value pairs) to update.
     *  @throws IConfigurationException : if the path is empty  or a value could not be translated to T or an existing local
     *                                    value map could not be removed.
     */
    template<typename T>
    void
    SetLocalMapOfValueMaps(
        const std::string& valueMapListKey,
        const std::map<std::string, std::map<std::string, T>>& valueMapList )
    {
        // Remove any existing local value maps
        const auto existingValueMapNames = GetSettingList<std::string>( valueMapListKey );

        for ( auto&& valueMapName : existingValueMapNames )
        {
            // Ignore the return value here as not all value maps are local.
            RemoveLocalSetting( valueMapName );
        }

        std::vector<std::string> valueMapNames;

        for ( auto&& valueMap : valueMapList )
        {
            const auto valueMapName = valueMap.first;
            valueMapNames.push_back( valueMapName );

            // Update the set of key/values
            for ( auto&& value : valueMap.second )
            {
                SetLocalSetting( valueMapName + CFG_DELIMITER + value.first, value.second );
            }
        }

        const auto valueMapNamesString = boost::algorithm::join( valueMapNames, "," );

        // Update the value map names entry in the local configuration (i.e. valueMapListKey=valueMapName1,valueMapeName2,...)
        SetLocalSetting( valueMapListKey, valueMapNamesString );
    }

    /*! @brief Template method to add a setting to the configuration.
     *
     *  This setting is only in memory. If WriteLocalConfig is called, it will be written to disk.
     *
     *  This function will not overwrite an existing setting - but returns false.
     *
     *  @throws ptree_bad_data : if the value could not be translated to T
     *
     *  @param path  : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     *  @param value : the value to store.
     *  @return bool : return true if successfully stored. returns false if path is empty or setting already exists.
     */
    template<typename T>
    bool
    AddTemporarySetting( const std::string& path, const T& value )
    {
        if ( path.empty() )
        {
            return false;
        }

        // Check if it exists - don't overwrite.
        if ( m_configTree.SettingExists( path ) )
        {
            return false;
        }

        m_configTree.SetLocalSetting( path, value );
        IntegrityProtectLocalSetting( path );

        return true;
    }

    /*! @brief Removes a setting in the local configuration.
     *
     *  This setting is only in memory. Expects keys to be unique.
     *  If WriteLocalConfig is called, it will be written to disk.
     *
     *  @param path  : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     *  @return        the number of items removed
     */
    size_t RemoveLocalSetting( std::string const& path );

    /*! @brief Gets a map of key/value pairs from the requested section in the
     *          configuration file.
     *
     *  @throws IConfigurationException if a map value element could not be converted.
     *          Does NOT throw if the key could not be found.
     *
     *  @param[in] section Section in the configuration where the data resides.
     *  @returns A map of key/value pairs, or an empty map if not found.
     *           User is allowed to not specify a key,
     *           in which case an empty map is returned.
     */
    template<typename T>
    std::map<std::string, T>
    GetSettingMap( const std::string& section ) const
    {
        return m_configTree.GetSettingMap<T>( section );
    }

    /*! @brief Returns a collection of top level section names from the default configuration file. */
    std::vector<std::string> GetSectionNames() const;

    /*!
     *  @brief      Returns the path to the base configuration file containing the this setting
     *              or an empty string if the settings doesn't exist.
     *  @param[in]  path  String path to setting key.
     *  @return     See above.
     */
    std::string GetSettingFilename( const std::string& path ) const;

    /*! @brief Creates an IConfiguration based on the type and resource descriptor
     *
     *         Switches based on the configuration type and creates a derived class that
     *         performs the population of the internal boost property tree
     *
     *  @param[in] type                  the type of configuration to be read in
     *  @param[in] resourceDescriptor    described the source/storage of the ie. file path or URI
     *  @param[in] localPathname         the pathname of the local configuration file or its containing folder used to store setting updates.
     *                                   Defaults to an empty string which means the local configuration file will have a .local.cfg suffix.
     *  @return IConfigurationSPtr       a shared pointer to the constructed IConfiguration
     */
    static IConfigurationSPtr ReadConfiguration(
        CFG_TYPE type,
        const std::string& resourceDescriptor,
        const std::string& localPathname = std::string() );

protected:
    /*! @brief  Data structure storing the configuration paths and values. */
    IniConfigurationTree m_configTree;

protected:
    /*! @brief Checks the data integrity of the setting defined by the supplied path
     *
     *  @param path : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     *  @return bool: true if the setting passed the integrity check
     */
    virtual bool CheckSettingIntegrity( const std::string& path ) const = 0;

    /*! @brief Adds a CRC to a given local setting.
     *
     *  @param path : the fully qualified path to the setting eg. [Section].etc.[Section].[Key]
     */
    virtual void IntegrityProtectLocalSetting( const std::string& path ) = 0;

private:
    static std::string ConstructLocalConfigPath(
        const std::string& defaultConfigFilePathStr,
        const boost::optional<std::string>& localConfigPathStr = boost::none );

    template<typename T>
    T
    RemoveCrc( const std::string& path, const std::string& strConfigVal ) const
    {
        T tmpVal;

        try
        {
            boost::match_results<std::string::const_iterator> subExpression;

            if ( boost::regex_match( strConfigVal, subExpression, CRC_FORMAT ) == false )
            {
                tmpVal = FromString<T>( strConfigVal );
            }
            else
            {
                tmpVal = FromString<T>( subExpression[ 1 ] );
            }
        }
        catch ( std::exception const& ex )
        {
            std::string const str( ex.what() );
            THROW_EXCEPTION( str + " - Failed conversion - " + path + ": " + strConfigVal, IConfigurationException );
        }

        return tmpVal;
    }

    template<typename T>
    T
    FromString( const std::string& strValue ) const
    {
        return StringConversion::FromString<T>( strValue );
    }

    /*! @brief Template method to get a setting from a section.
     *
     *         Use this method if it is not important/significant is the setting does not exist.
     *         If the setting does not existing an uninitialized optional will be returned
     *
     *  @throws IConfigurationException :
     *                        if the section does not exist
     *                        if the value could not be translated to T
     *                        if CRC is not ok
     *
     *  @param section : the top level section name
     *  @param key : the key name
     *  @return boost::optional<T> : an optional wrapping the value corresponding to the 'key' param
     */
    template<typename T>
    boost::optional<T>
    GetOptionalSettingFromSection( const std::string& section, const std::string& key )
    {
        if ( section.empty() || key.empty() )
        {
            THROW_EXCEPTION( "Empty section and/or key provided", IConfigurationException );
        }

        auto retVal = m_configTree.GetSettingRawString( section, key );

        if ( !retVal )
        {
            return boost::none;
        }

        auto path = section + CFG_DELIMITER + key;

        // there is a setting at the supplied path so check the data integrity
        // TODO REUSE-951 this does not work for section names containing CFG_DELIMITER
        if ( !CheckSettingIntegrity( path ) )
        {
            // an empty path will return the root of the property tree
            // the root should not be 'got' as it should not have a value
            THROW_EXCEPTION(
                "The integrity check failed on the setting, if the source is valid then corruption has occurred in "
                "RAM ",
                IConfigurationException );
        }

        return boost::optional<T>( RemoveCrc<T>( path, retVal.get() ) );
    }
};

/*!
 * @brief    Template specialisation for single-precision floating point types
 * @details  This specialisation allows all floating point conversion results
 *           except in the case when "NaN" is returned - in which case an exception
 *           is thrown.
 */
template<>
inline float
IConfiguration::FromString( const std::string& strValue ) const
{
    float value = StringConversion::FromString<float>( strValue );

    if ( std::isnan( value ) )
    {
        THROW_OUT_OF_RANGE( "NaN is not allowed as a configurable float value" );
    }

    return value;
}

/*!
 * @brief    Template specialisation for double-precision floating point types
 * @details  This specialisation allows all floating point conversion results
 *           except in the case when "NaN" is returned - in which case an exception
 *           is thrown.
 */
template<>
inline double
IConfiguration::FromString( const std::string& strValue ) const
{
    double value = StringConversion::FromString<double>( strValue );

    if ( std::isnan( value ) )
    {
        THROW_OUT_OF_RANGE( "NaN is not allowed as a configurable double value" );
    }

    return value;
}

} //  namespace imp
