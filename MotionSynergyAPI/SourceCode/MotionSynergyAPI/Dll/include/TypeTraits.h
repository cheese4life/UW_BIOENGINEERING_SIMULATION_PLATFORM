/*
   The copyright to the computer program(s) herein is the property of
   Dover Motion, Inc. This file is subject to the terms and
   conditions found in the 'DoverLicense.txt' file included with this
   source code package.
*/

#pragma once

#include <type_traits>

namespace imp
{

    // Arrays as a template parameter do not decay:
    // string literals show up as char const (&)[N]. We determine N with std::extent after temporarily stripping the reference.
    // If IsStringLiteral is instantiated with the type of a string literal, the inherited member 'value' is true, else false.
    // Note though that arrays of size 0 are not allowed, so we replace that with an array of size 1. This works because
    // if extents returns 0 it means it's not an array generally speaking, which means it's not an array of size 1 specifically.
    template<typename T>
    struct IsStringLiteral
        : std::is_same<
        T,
        std::add_lvalue_reference_t<char const [std::extent<std::remove_reference_t<T>>::value == 0 ? 1 : std::extent<std::remove_reference_t<T>>::value]>
        >
    {

    };

}
