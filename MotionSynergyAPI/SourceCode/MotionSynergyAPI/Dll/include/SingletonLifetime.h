/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonExceptions.h"

#include <memory>
#include <string>

namespace imp
{

/*!
 * @brief        A SingletonLifetime provides a common implementation for managing the lifetime of a Singleton instance.
 * @details      This class should not be exposed directly to clients of the Singleton.
 *               Instructions on implementing the Singleton pattern can be found in the Louganis Idioms document '400_1247_xx_Louganis Coding Idioms.doc'.
 * @tparam TSingletonInterface           Type of the interface supported by the Singleton.
 * @tparam TSingletonSecondaryInterface  Type of an optional '2nd' interface supported by the Singleton.
 * @tparam TSingleton                    Type of the Singleton which supports both interfaces.
 *
 */
template<
    typename TSingletonInterface,
    typename TSingletonSecondaryInterface = TSingletonInterface,
    typename TSingleton = TSingletonInterface>
class SingletonLifetime
{
public:
    /*!
     * @brief SingletonLifetime constructor
     *
     * @param[in] name                   The name of the singleton used to identify the singleton when reporting an error.
     * @param[in] allowReregistration    Usually a singleton is created once and lives for the life of the application. If
     *                                   this parameter is true, it's possible to replace an existing instance of a singleton
     *                                   with a new instance (i.e. by calling Register() more than once).
     */
    explicit SingletonLifetime( const std::string& name, bool allowReregistration = false )
        : m_name( name )
        , m_allowReregistration( allowReregistration )
    {
    }

    /*!
     * @brief        SingletonLifetime destructor
     * @details      Checks to see Destroy() has been called and outputs a message to stderr if it hasn't. The
     *               destructor does not need to be called explicitly, it will be called as part of
     *               static variable clean up during application exit.
     */
    ~SingletonLifetime()
    {
        if ( IsRegistered() )
        {
            // This may happen very late in the application shutdown sequence, so only output the warning to
            // stderr.
            std::cerr << "Destroy() for singleton '" << m_name << "' was not called before application exit."
                      << std::endl;
        }
    }

    /*!
     * @brief        Register an instance as the singleton.
     * @returns      The previously registered instance if allow re-registration is permitted and Register has previously been called or a nullptr.
     * @exception   std::logic_error if Register has previously been called and re-registration is not permitted.
     */
    std::shared_ptr<TSingleton>
    Register( std::shared_ptr<TSingleton> pSingleton )
    {
        if ( IsRegistered() && !m_allowReregistration )
        {
            THROW_LOGIC_ERROR( "Attempting to register a 2nd instance for singleton '" << m_name << "'." );
        }

        auto pPreviousInstance = m_pSingleton;
        m_pSingleton = pSingleton;
        return pPreviousInstance;
    }

    /*!
     * @brief    Has the singleton instance been registered.
     * @returns  true if an instance has been registered, false if not.
     */
    bool
    IsRegistered() const
    {
        return m_pSingleton != nullptr;
    }

    /*!
     * @brief        Get the singleton instance.
     * @returns      The singleton instance on success.
     * @exception   Throws a std::logic_error if there is no registered singleton instance.
     */
    std::shared_ptr<TSingletonInterface>
    Get()
    {
        if ( !IsRegistered() )
        {
            THROW_LOGIC_ERROR(
                "Attempting to access singleton '" << m_name << "' before an instance has been registered." );
        }

        return m_pSingleton;
    }

    /*!
     * @brief       Get the 'secondary' interface of the singleton instance.
     * @details     Some singletons provide different interfaces to different clients. This method
     *              allows a 2nd interface to be returned. For singletons which only provide a
     *              single interface, this method will return the same interface as returned by 'Get()'.
     * @returns     The secondary singleton interface on success.
     * @exception  Throws a std::logic_error if there is no registered singleton instance
     *              or the registered instance does not support the requested interface.
     */
    std::shared_ptr<TSingletonSecondaryInterface>
    GetSecondaryInterface()
    {
        auto pSecondaryInterface = std::dynamic_pointer_cast<TSingletonSecondaryInterface>( m_pSingleton );

        if ( !pSecondaryInterface )
        {
            THROW_LOGIC_ERROR(
                "Attempting to access singleton '" << m_name
                                                   << "' before an instance has been registered or the registered "
                                                      "instance does not support the requested interface." );
        }

        return pSecondaryInterface;
    }

    /*!
     * @brief       Destroy the currently registered singleton instance. Must be called on application shutdown.
     * @details     Safe to call multiple times and even if a singleton instance was never registered. Checks the
     *              current singleton usage count and outputs a warning to stderr if the singleton is still in use.
     */
    void
    Destroy()
    {
        if ( m_pSingleton.use_count() > 1 )
        {
            // This may happen late in the application shutdown sequence outside the main exception handler,
            // so output the warning to stderr rather than logging or throwing an exception.
            std::cerr << "Attempting to destroy singleton '" << m_name
                      << "' before all shared pointer references have been released (i.e the singleton may still be in "
                         "use, use_count() == "
                      << m_pSingleton.use_count() << ")." << std::endl;
        }

        m_pSingleton.reset();
    }

private:
    std::string m_name;
    bool m_allowReregistration;
    std::shared_ptr<TSingleton> m_pSingleton;
};


} //  namespace imp
