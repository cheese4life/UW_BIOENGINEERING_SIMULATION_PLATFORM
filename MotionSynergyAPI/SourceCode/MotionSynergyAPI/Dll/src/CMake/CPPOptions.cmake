# Specify C++ compiler and linker settings

# Require C++20 and enable extensions for all targets.
set(CMAKE_CXX_STANDARD              20)
set(CMAKE_CXX_STANDARD_REQUIRED     ON)
set(CMAKE_CXX_EXTENSIONS            ON)

# print what toolchain file we are using
message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

# Write out the compiler and version being used to build to the console.
message(STATUS "Using compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Set up compiler warnings and turn on warnings as errors
if(MSVC)
    add_compile_options(
        /MP     # enable multi-processor compilation
        /W4     # warning level 4
        /WX     # warnings as errors
        /experimental:external
        /external:anglebrackets
        /external:W0 # any #include <> won't emit warnings
        # Note, /w1 will enable the warning and treat it as level 1. Enabling fatal warnings will treat these as errors.
        /w14062 # enumerator not handled in switch
        /w14242 # conversion from 'type1' to 'type2', possible loss of data
        /wd4250 # 'class1' : inherits 'class2::member' via dominance - this will inhibit warnings when a method in the superbase is overridden in one of the middle derived classes in a diamond hierarchy
        /w14254 # 'operator': conversion from 'type1' to 'type2', possible loss of data
        /w14263 # 'function': member function does not override any base class virtual member function
        /w14264 # 'virtual_function': no override available for virtual member function from base 'class'; function is hidden
        /w14265 # When a class has virtual functions but a nonvirtual destructor
        /w14266 # 'function': no override available for virtual member function from base 'type'; function is hidden
        /w14287 # 'operator': unsigned/negative constant mismatch
        /w14289 # nonstandard extension used : 'var' : loop control variable declared in the for-loop is used outside the for-loop scope
        /w14296 # 'operator': expression is always false
        /w14311 # This warning detects 64-bit pointer truncation issues
        /w14388 # signed/unsigned mismatch
        /w14464 # relative include path contains '..'
        /w14545 # expression before comma evaluates to a function which is missing an argument list
        /w14546 # function call before comma missing argument list
        /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
        /w14549 # 'operator1': operator before comma has no effect; did you intend 'operator2'?
        /w14555 # expression has no effect; expected expression with side-effect
        /w14596 # 'identifier': illegal qualified name in member declaration
        /w14640 # 'instance': construction of local static object is not thread-safe
                # "In the current implementation you will still occasionally get a warning through from an external header when that warning was emitted by the compiler's back-end (as opposed to front-end). These warnings usually start with C47XX ...     You can still disable these warnings the traditional way with /wd47XX."
        /wd4701 # Potentially uninitialized local variable 'name' used [leaks from boost crc]
        /w14777 # 'function' : format string 'string' requires an argument of type 'type1', but variadic argument number has type 'type2'
        /w14826 # Conversion from 'type1' to 'type2' is sign-extended. This may cause unexpected runtime behavior.
        /w14837 # trigraph detected: '??character' replaced by 'character'
        /w14842 # the result of 'offsetof' applied to a type using multiple inheritance is not guaranteed to be consistent between compiler releases
        /w14868 # 'file(line_number)' compiler may not enforce left-to-right evaluation order in braced initialization list'
        /w14905 # wide string literal cast to 'LPSTR'
        /w14906 # string literal cast to 'LPWSTR'
        /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly applied
        /w14946 # reinterpret_cast used between related classes: 'class1' and 'class2'
        /w14986 # 'symbol': exception specification does not match previous declaration
        /w14987 # nonstandard extension used: 'throw (...)'
        /w15029 # nonstandard extension used: alignment attributes in C++ apply to variables, data members and tag types only
        /w15031 # #pragma warning(pop): likely mismatch, popping warning state pushed in different file
        /w15032 # detected #pragma warning(push) with no corresponding #pragma warning(pop)
        /w15038 # data member 'member1' will be initialized after data member 'member2'
        /w15042 # 'function': function declarations at block scope cannot be specified 'inline' in standard C++; remove 'inline' specifier
        # /w15045 # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        /w15220 # 'name': a non-static data member with a volatile qualified type no longer implies that compiler generated copy/move constructors and copy/move assignment operators are not trivial
        /w15221 # xfg::rename is deprecated.
        /w15211 # 'keyword-1' has been deprecated; prefer using 'keyword-2' instead
        /w15222 # 'attribute-name': all unscoped attribute names are reserved for future standardization
        /w15223 # all attribute names in the attribute namespace 'msvc' are reserved for the implementation
        /w15224 # all attribute names in the attribute namespace 'a-namespace' are reserved for future standardization
        /w15227 # nonstandard extension, resolved 'symbol' to 'instance' which is not visible with /permissive- on
        /w15228 # nonstandard extension, 'identifier' resolved to a member of a dependent base. This lookup is not allowed under /permissive-
        /w15229 # nonstandard extension, the hidden friend function 'function-name' was found by name lookup which isn't allowed under /permissive-.
        /w15230 # nonstandard extension, 'identifier' was resolved to 'symbol' under /permissive. Under /permissive- it would resolve to 'other-symbol'
        /w15233 # explicit lambda capture 'identifier' is not used
        /w15239 # 'Symbol': potentially-throwing function called from a function declared __declspec(nothrow). Undefined behavior may occur if an exception is thrown
        /w15240 # 'attribute-string': attribute is ignored in this syntactic position
        /w15243 # 'Type-name': using incomplete class 'symbol' can cause potential one definition rule violation due to ABI limitation

        /Zc:externC- # Disable extern "C" name mangling conformance to C++ standard to maintain compatibility with existing codebases
        
        # This is enabled by default with c++20.
        # /permissive-
    )
    if(IMP_CCP_ENABLE_ASAN)
        add_compile_options(
            /fsanitize=address
            /Zi # Disabling incremental linking requires disabling support for Edit and Continue.
            )
        add_link_options(
            /INCREMENTAL:NO # Incremental linking is incompatible with ASan.
            /ignore:4075 # Disabling incremental linking gives warning 4075 as pre-build gmock libraries were built with /ZI, which
                         # is ignored due to the /INCREMENTAL:NO setting. As this lib is prebuilt, there's no way to set the /Zi setting
                         # at build time, so we have to ignore the warning.
            )
    endif()

    add_compile_definitions(
        IMP_CCP                                 # louganis: building CCP
        UNICODE                                 # windows headers
        _UNICODE                                # msvc C runtime
        _CRT_USE_CONFORMING_ANNEX_K_TIME=1      # msvc std library: use conforming K functions for time handling (e.g., gmtime_s)
        WIN32_LEAN_AND_MEAN                     # msvc: removes a lot of uncommonly used headers: see https://docs.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers#faster-builds-with-smaller-header-files
        _WIN32_WINNT=0x0A00                     # force minimum Windows 10 for Windows system targets
        NOMINMAX                                # undefine the windows min and max macros, which break the std function templates
    )

    set(OSName "windows")                       # Used when defining the build directory name

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(
        -Wall
        -Werror
        -Wextra
        -Wnull-dereference
        -Wduplicated-cond
        -Wdouble-promotion
        -Wformat=2
        -Wshadow                                # Warn whenever a local variable or type declaration shadows another variable, parameter, type, class member (in C++), or instance variable (in Objective-C) or whenever a built-in function is shadowed.
        -Wvla
        -Wdeprecated
        -Wformat-truncation
        # -Wundef                               # triggered by testing undefined definitions: currently IMP testing of the values of _M_CEE and _MANAGED without testing if defined will trigger this.
        -fno-common
        -Wcast-qual
        -Wlogical-op
        -Wnull-dereference
        -Wmisleading-indentation
        -Wcast-align
        $<$<COMPILE_LANGUAGE:CXX>:-Wold-style-cast>         # C++ files should not use the old-style C casts
        $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>       # C++ files should not have virtual functions with an accessible non-virtual destructor
        $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>     # C++ member function in derived hides member function in base
        $<$<COMPILE_LANGUAGE:CXX>:-Wstrict-null-sentinel>   # C++ warning if using gcc-only uncasted NULL as sentinel, which is non-portable
    )
    if(IMP_CCP_ENABLE_ASAN)
        add_compile_options(-fsanitize=address)
        add_link_options(-fsanitize=address)
    endif()

    add_compile_definitions(
        IMP_CCP                                 # code is compiled for the Invetech Modular Platform CCP
    )

    add_compile_options(-fvisibility-inlines-hidden)

    # add_compile_options(-fvisibility=hidden)
    # add_compile_options(-flto)
    # add_link_options(-flto)

    add_link_options(-ldl)

    # Enable position independent code so shared libraries can be correctly linked for Linux.
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    set(OSName "linux")                         # Used when defining the build directory name

else()
    message(FATAL_ERROR "Unsupported compiler in use: ${CMAKE_CXX_COMPILER_ID}")
endif()


# define IMP_CCP_64BIT on 64-bit targets
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "64-bit platform")
    set(IMP_CCP_64BIT ON)
    add_compile_definitions(IMP_CCP_64BIT)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(STATUS "32-bit platform")
else()
    message(FATAL_ERROR "Unsupported platform width")
endif()
