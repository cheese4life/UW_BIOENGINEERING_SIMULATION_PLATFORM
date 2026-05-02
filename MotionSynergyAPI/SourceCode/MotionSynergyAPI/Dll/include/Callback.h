/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

/*!
* @brief      Defines abstract Callback classes.
*             These classes declare the interface to be provided by all
*             Callbacks.
*/
#pragma once

namespace IPLKeystone
{

/*! @class Callback<Context>
*  @brief Declares the Invoke() method to be defined by Callbacks.
*/
template<typename Context>
class Callback
{
protected:

    Callback() {}

public:

    virtual ~Callback() {}

    /*!
    @brief Callback invokation method (passing a value) that must be
           supported by all Callbacks.
    @param [in] context A value passed with the callback
    @return True if the callback was successfully invoked
    */
    virtual void Invoke( const Context& context ) = 0;
};

/*!
* @class NullCallback<Context>
* @brief Concrete subclass of Callback<Context> which takes no action upon notification.
*/
template<typename Context>
class NullCallback
    : public Callback<Context>
{
public:

    NullCallback() {}
    ~NullCallback() {}

    void Invoke( const Context& ) {}
};

/*! @class Callback<void>
*  @brief Declares the Invoke() method to be defined by Callbacks.
*/
template<>
class Callback<void>
{
protected:

    Callback() {}

public:

    virtual ~Callback() {}

    /*!
    @brief Callback invokation method (passing a value) that must be
           supported by all Callbacks.
    @param [in] context A value passed with the callback
    @return True if the callback was successfully invoked
    */
    virtual void Invoke() = 0;
};

/*!
* @class NullCallback<void>
* @brief Concrete subclass of Callback<void> which takes no action upon notification.
*/
template<>
class NullCallback<void>
    : public Callback<void>
{
public:

    NullCallback() {}
    ~NullCallback() {}

    void Invoke() override {}
};

} // namespace IPLKeystone
