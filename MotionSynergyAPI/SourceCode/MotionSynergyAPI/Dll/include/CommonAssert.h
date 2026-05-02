/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include "CommonAssertImpl.h"

/*!
 *  @brief   Assertion which throws an exception.
 *  @details Evaluates the given expression, and if false, outputs to stderr
 *           and throws a std::logic_error exception
 *           with a message giving the expression, and appending the file,
 *           line number and function name.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *
 *  @param[in]   Expression     The expression to assert true.
 */
#define ASSERT( Expression ) _ASSERT_IMPL( Expression )

/*!
 *  @brief   Assertion which throws an exception.
 *  @details Evaluates the given expression, and if false, outputs to stderr
 *           and throws a std::logic_error exception
 *           with a message giving the expression, and appending the file,
 *           line number and function name.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *
 *  @param[in]   Expression     The expression to assert true.
 *  @param[in]   Message        A message describing the assertion.
 */
#define ASSERT_MSG( Expression, Message ) _ASSERT_MSG_IMPL( Expression, Message )

/*!
 *  @brief   A special assert for use in destructors which does not throw.
 *  @details Evaluates the given expression, and if false, outputs to stderr
 *           with a message giving the expression, and appending the file,
 *           line number and function name.
 *           Will assert() in debug builds and abort() in release builds.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *
 *  @param[in]   Expression     The expression to assert true.
 */
#define DESTRUCTOR_ASSERT( Expression ) _DESTRUCTOR_ASSERT_IMPL( Expression )

/*!
 *  @brief   A special assert for use in destructors which does not throw.
 *  @details Evaluates the given expression, and if false, outputs to stderr
 *           with a message giving the expression, and appending the file,
 *           line number and function name.
 *           Will assert() in debug builds and abort() in release builds.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *
 *  @param[in]   Expression     The expression to assert true.
 *  @param[in]   Message        A message describing the assertion.
 */
#define DESTRUCTOR_ASSERT_MSG( Expression, Message ) _DESTRUCTOR_ASSERT_MSG_IMPL( Expression, Message )

/*!
 *  @brief   Assertion which throws an exception if a function argument is invalid.
 *  @details Evaluates the given expression, and if false, outputs to stderr
 *           and throws a std::invalid_argument exception
 *           with a message giving the expression, and appending the file,
 *           line number and function name.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *
 *  @param[in]   Expression     The expression to assert true.
 */
#define ASSERT_ARG_VALID( Expression ) _ASSERT_ARG_VALID_IMPL( Expression )

/*!
 *  @brief   Assertion which throws an exception if a function argument is invalid.
 *  @details Evaluates the given expression, and if false, outputs to stderr
 *           and throws a std::invalid_argument exception
 *           with a message giving the expression, and appending the file,
 *           line number and function name.
 *           Unlike a normal assert, it cannot be disabled in debug builds.
 *           Use for invariance checks and other conditions that should always be true.
 *
 *  @param[in]   Expression     The expression to assert true.
 *  @param[in]   Message        A message describing the assertion.
 */
#define ASSERT_ARG_VALID_MSG( Expression, Message ) _ASSERT_ARG_VALID_MSG_IMPL( Expression, Message )
