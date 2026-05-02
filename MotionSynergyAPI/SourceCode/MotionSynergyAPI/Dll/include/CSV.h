/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#ifndef CSV_H
#define CSV_H
#pragma once

#include <cstring>
#include <exception>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

/// @brief
/// csv parser
class CSV
{
public:
    CSV()
        : m_rows( 0 )
    {
    }

    int
    Rows()
    {
        return m_rows;
    }
    int
    Cols()
    {
        return static_cast<int>( m_columnNames.size() );
    }

    /// @brief
    /// Converts std::string type column data into a vector.
    ///
    /// @tparam T type to convert column data into.
    /// @param columnName column to convert column data into.
    /// @return a vector of converted column data.
    template<typename T>
    std::enable_if_t<std::is_same<T, std::string>::value, std::vector<std::string>> Column( const char* columnName );

    /// @brief
    /// Converts double type column data into a vector.
    ///
    /// @tparam T type to convert column data into.
    /// @param columnName column to convert column data into.
    /// @return a vector of converted column data.
    template<typename T>
    std::enable_if_t<std::is_same<T, double>::value, std::vector<double>> Column( const char* columnName );

    /// @brief
    /// Converts integer type column data into a vector.
    ///
    /// @tparam T type to convert column data into.
    /// @param columnName column to convert column data into.
    /// @return a vector of converted column data.
    template<typename T>
    std::enable_if_t<std::numeric_limits<T>::is_integer, std::vector<T>> Column( const char* columnName );

    /// @brief
    /// Loads csv data from a file.
    ///
    /// @param file path to file to load csv file from.
    /// @param columnNames list of column names to associate column data with,
    ///                    nullptr assumes that first row contains column names.
    void ReadFile( const char* file, const char* columnNames = nullptr );

    /// @brief
    /// Dumps csv data to file without column names.
    ///
    /// @param file file to dump data to.
    void DumpFile( const char* file );

    class exception : public std::exception
    {
    public:
        exception( const char* w, const char* file, unsigned int line )
            : std::exception()
            , m_len( static_cast<int>( strlen( file ) + strlen( w ) + static_cast<size_t>( 80 ) ) )
            , m_what( w )
            , m_file( file )
            , m_line( line )
            , m_str( std::shared_ptr<char[]>( new char[ m_len + 1 ] ) )
        {
            (void)snprintf( m_str.get(), m_len, "%s@%u:%s\n", m_file, m_line, m_what );
            m_str[ m_len ] = '\0';
        }

        const char*
        what() const noexcept override
        {
            return m_str.get();
        }

        unsigned int
        line() const
        {
            return m_line;
        }

        const char*
        file() const
        {
            return m_file;
        }

        const char*
        message() const
        {
            return m_str.get();
        }


    private:
        int m_len;
        const char* m_what;
        const char* m_file;
        unsigned int m_line;
        std::shared_ptr<char[]> m_str;
    };

    struct sign_exception : public exception
    {
        sign_exception( const char* file, unsigned int line )
            : exception( "sign exception", file, line )
        {
        }
    };

    struct file_not_found : public exception
    {
        file_not_found( const char* file, unsigned int line )
            : exception( "file not found", file, line )
        {
        }
    };

    struct file_error : public exception
    {
        file_error( const char* file, unsigned int line )
            : exception( "file error", file, line )
        {
        }
    };

    void
    swap( CSV& csv ) noexcept
    {
        csv.m_columnNames.swap( m_columnNames );
        csv.m_columnGapCount.swap( m_columnGapCount );
        csv.m_columnNameMap.swap( m_columnNameMap );
        const int temp = csv.m_rows;
        csv.m_rows = m_rows;
        m_rows = temp;
    }

private:
    void ParseDocument( const char* document, const char* colNames = nullptr );
    std::vector<std::string> m_columnNames;
    std::vector<int32_t> m_columnGapCount;
    std::map<std::string, std::vector<std::string>> m_columnNameMap;
    int m_rows;
};
#endif // CSV_H
