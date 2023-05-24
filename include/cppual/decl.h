/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_DECL_H_
#define CPPUAL_DECL_H_

#undef OS_CURRENT
#undef OS_STD_UNIX
#undef OS_STD_POSIX
#undef OS_STD_WINNT
#undef OS_STD_APPLE
#undef OS_STD_CYGWIN
#undef OS_PLATFORM_MINGW
#undef OS_PLATFORM_WIN_DESKTOP
#undef OS_PLATFORM_WIN_PHONE
#undef OS_PLATFORM_WIN_RT
#undef OS_PLATFORM_WIN_STORE
#undef OS_GNU_LINUX
#undef OS_BSD
#undef OS_ANDROID
#undef OS_AIX
#undef OS_SOLARIS
#undef OS_IOS
#undef OS_MACX
#undef OS_WIN32
#undef OS_WIN64
#undef OS_WINDOWS
#undef OS_PS3       // undefined
#undef OS_PS4
#undef OS_XBOX360   // undefined
#undef OS_XBOXONE   // undefined
#undef OS_WII_U     // undefined
#undef OS_FREERT    // undefined
#undef ARCH_64BITS
#undef ARCH_32BITS
#undef TXT
#undef TXT_STD_ASCII
#undef TXT_STD_UTF8
#undef TXT_STD_UTF16
#undef TXT_STD_UTF32
#undef TXT_STD_OLD_UNICODE
#undef DEBUG_MODE
#undef UNUSED
#undef COMPILER_NAME
#undef COMPILER_VERSION
#undef STRINGIFY_HELPER
#undef STRINGIFY
#undef DECL_EXPORT
#undef DECL_IMPORT
#undef DECL_HIDDEN

#define STRINGIFY(S) STRINGIFY_HELPER(S)
#define STRINGIFY_HELPER(S) #S

#ifdef __CYGWIN__
#   define OS_STD_CYGWIN
#endif // CYGWIN

/// operating system support verification & definitions
#if defined (__WINDOWS__) or defined (__TOS_WIN__) or defined (__WIN32__) or defined (__WIN64__)
#
#   ifdef WINAPI_FAMILY
#       if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
#           define OS_PLATFORM_WIN_DESKTOP
#       elif WINAPI_FAMILY == WINAPI_FAMILY_APP
#           define OS_PLATFORM_WIN_RT
#           define OS_PLATFORM_WIN_STORE
#       elif WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#           define OS_PLATFORM_WIN_PHONE
#           define OS_PLATFORM_WIN_RT
#       endif
#   else
#       define OS_PLATFORM_WIN_DESKTOP
#   endif

#   if defined (_WIN64) or defined (__WIN64) or defined (__WIN64__)
#
#       define OS_WIN64 1
#       define OS_STD_WINNT
#       define OS_WINDOWS OS_WIN64
#       define OS_CURRENT OS_WINDOWS
#
#   elif defined (_WIN32) or defined (__WIN32) or defined (__WIN32__)
#
#       define OS_WIN32 2
#       define OS_STD_WINNT
#       define OS_WINDOWS OS_WIN32
#       define OS_CURRENT OS_WINDOWS
#
#   endif
#
#elif defined (__unix__) or defined (__unix)
#
#   define OS_STD_UNIX
#
#   if (defined (__linux__) or defined (__gnu_linux__)) && !defined (__ANDROID__)
#       define OS_GNU_LINUX 3
#       define OS_CURRENT OS_GNU_LINUX
#       define OS_STD_POSIX
#   elif defined (BSD) or defined (__FreeBSD__) or defined (__NetBSD__) or defined (__OpenBSD__) or \
    defined (__bsdi__) or defined (__DragonFly__)
#       define OS_BSD 4
#       define OS_CURRENT OS_BSD
#       define OS_STD_POSIX
#   elif defined (__ANDROID__)
#       define OS_ANDROID
#       define OS_CURRENT OS_ANDROID
#       define OS_STD_POSIX
#   elif defined (_AIX)
#       define OS_AIX 5
#       define OS_CURRENT OS_AIX
#   elif (defined (__sun) or defined (__sun__)) and (defined (__SVR4) or defined (__SVR4__))
#       define OS_SOLARIS 6
#       define OS_CURRENT OS_SOLARIS
#   elif defined(__ORBIS__)
#       define OS_PS4 7
#       define OS_CURRENT OS_PS4
#       define OS_STD_POSIX
#   endif
#
#elif (defined (__APPLE__) and defined (__MACH__)) or defined(__APPLE_CC__) or defined(__APPLE_CPP__)
#   define OS_STD_UNIX
#   define OS_STD_APPLE
#   define OS_STD_POSIX
#   include "TargetConditionals.h"
#   if defined (TARGET_OS_IPHONE) or defined (TARGET_IPHONE_SIMULATOR)
#       define OS_IOS 8
#       define OS_CURRENT OS_IOS
#   elif defined(TARGET_OS_MAC)
#       define OS_MACX 9
#       define OS_CURRENT OS_MACX
#   else
#       error unknown apple operating system!
#   endif
#else
#   error unsupported operating system!
#endif // OS

#if defined (__x86_64__) or defined (_M_AMD64) or defined (_M_X64) or defined (_AMD64_) or \
    defined (__arm64__) or defined (__aarch64__)
#  define ARCH_64BITS
#  define ARCH_STR "64 bit"
#else
#  define ARCH_32BITS
#  define ARCH_STR "32 bit"
#endif // ARCH

#undef PACKED
#undef NOVTABLE
#undef DEPRECATED
#undef FORCEINLINE
#undef DECLSPEC_ALIGN

#if defined (_MSC_VER) and (_MSC_VER > 1300) // vc++ 2005
#
#   pragma once
#   define PACKED __declspec(align(packed))
#   define NOVTABLE __declspec(novtable)
#   define DEPRECATED(msg)
#   define FORCEINLINE __forceinline
#   define DECLSPEC_ALIGN(x) __declspec(align(x))
#   if !defined (STDCALL) and !defined (FASTCALL) and !defined (CDECL)
#       define STDCALL __stdcall
#       define FASTCALL __fastcall
#       define CDECL __cdecl
#   endif // STDCALL and CDECL and FASTCALL
#
#elif defined (__GNUC__) or defined (__clang__)
#
#   define PACKED __attribute__ ((__packed__))
#   define NOVTABLE
#   define DEPRECATED(msg) __attribute__ ((deprecated(msg)))
#   define FORCEINLINE inline __attribute__((always_inline))
#   define DECLSPEC_ALIGN(x) __attribute__((aligned(x)))
#   if !defined (STDCALL) and !defined (FASTCALL) and !defined (CDECL)
#       if defined (__WIN64__) or defined(__WIN32__)
#           define STDCALL  __stdcall
#           define FASTCALL __fastcall
#           define CDECL
#       else
#           define STDCALL  __attribute__((stdcall))
#           define FASTCALL __attribute__((fastcall))
#           define CDECL
#       endif
#   endif // STDCALL and CDECL and FASTCALL
#
#else
#
#   define PACKED
#   define NOVTABLE
#   define DEPRECATED(msg)
#   define FORCEINLINE inline
#   define DECLSPEC_ALIGN(x) alignas(x)
#   if !defined (STDCALL) and !defined (FASTCALL) and !defined (CDECL)
#       define STDCALL
#       define FASTCALL
#       define CDECL
#   endif // STDCALL and CDECL and FASTCALL
#
#endif // _MSC_VER and __GNUC__

#if !defined (DECL_EXPORT) and !defined (DECL_IMPORT) and !defined (DECL_HIDDEN)
#
#   if defined (__WIN32__) or defined (__WIN64__)
#       define DECL_EXPORT __declspec(dllexport)
#       define DECL_IMPORT __declspec(dllimport)
#       define DECL_HIDDEN
#   elif (defined (__GNUC__))
#       define DECL_EXPORT __attribute__((visibility("default")))
#       define DECL_IMPORT __attribute__((visibility("default")))
#       define DECL_HIDDEN __attribute__((visibility("hidden")))
#   else
#       define DECL_IMPORT
#       define DECL_EXPORT
#       define DECL_HIDDEN
#   endif
#
#endif // DECL_EXPORT, DECL_IMPORT and DECL_HIDDEN

#ifdef OS_WINDOWS
#   define DLCALL STDCALL
#else
#   define DLCALL
#endif

#ifndef TRANSPARENT_UNION
#
#   ifdef __GNUC__
#        define TRANSPARENT_UNION __attribute__((__transparent_union__))
#   else
#        define TRANSPARENT_UNION
#   endif
#
#endif // TRANSPARENT_UNION

#ifdef API_EXPORT
#   define SHARED_API DECL_EXPORT
#else
#   define SHARED_API DECL_IMPORT
#endif

#if !defined (NDEBUG) or defined (__DEBUG) or defined (_DEBUG) or defined (__DEBUG__)
#   define DEBUG_MODE
#endif // DEBUG

#ifdef __cplusplus

#if __cplusplus < 201103L or (defined (_MSC_VER) and _MSC_VER < 1700)
#   error this compiler does not support ISO C++11!
#endif

#if defined (__clang__)
#
#   define COMPILER_NAME "Clang"
#   define COMPILER_VERSION \
    STRINGIFY(__clang_major__) + '.' + STRINGIFY(__clang_minor__) + '.' + STRINGIFY(__clang_patchlevel__)
#
#elif defined (__GNUC__)
#
#   define COMPILER_NAME "GCC"
#   define COMPILER_VERSION \
    STRINGIFY(__GNUC__) + '.' + STRINGIFY(__GNUC_MINOR__) + '.' + STRINGIFY(__GNUC_PATCHLEVEL__)
#
#elif defined (_MSC_VER)
#
#   define COMPILER_NAME "MSVC"
#   define COMPILER_VERSION STRINGIFY(_MSC_FULL_VER)
#
#else
#
#   define COMPILER_NAME "Unknown Compiler"
#   define COMPILER_VERSION ""
#
#endif

#define UNUSED(x) (void)x

/// character storage literals
#ifdef __STD_UTF_8__
#   define TXT_STD_UTF8
#   define TXT(quote) u8 ## quote
#elif defined (__STD_UTF_16__)
#   define TXT_STD_UTF16
#   define TXT(quote) u ## quote
#elif defined (__STD_UTF_32__)
#   define TXT_STD_UTF32
#   define TXT(quote) U ## quote
#elif defined (__STD_UNICODE__)
#   define TXT_STD_OLD_UNICODE
#   define TXT(quote) L ## quote
#else
#   define TXT_STD_ASCII
#   define TXT(quote) quote
#endif

template <typename T, typename U>
inline T pointer_cast (U value)
{
#   ifdef DEBUG_MODE
    return dynamic_cast<T> (value);
#   else
    return static_cast<T>  (value);
#   endif
}

namespace cppual {

/// void typedefs
typedef const void cvoid;

/// bool typedefs
typedef const bool cbool;

/// 1 byte size typedefs
typedef unsigned char byte;
typedef unsigned char const cbyte;

/// ASCII & UTF-8 sized typedefs
typedef const char cchar;
typedef unsigned char uchar;
typedef const unsigned cuchar;

typedef char char8;
typedef cchar cchar8;
typedef uchar uchar8;
typedef cuchar cuchar8;

/// UTF-16 sized typedefs
typedef char16_t char16;
typedef const char16_t cchar16;

/// UTF-32 sized typedefs
typedef char32_t char32;
typedef const char32_t cchar32;

/// old UNICODE typedefs
typedef wchar_t wchar;
typedef const wchar_t cwchar;

#ifdef TXT_STD_UTF16
typedef char16 tchar;
typedef cchar16 ctchar;
typedef uchar16 utchar;
typedef cuchar16 cutchar;
#elif defined (TXT_STD_UTF32)
typedef char32 tchar;
typedef cchar32 ctchar;
typedef uchar32 utchar;
typedef cuchar32 cutchar;
#elif defined (TXT_STD_OLD_UNICODE)
typedef wchar tchar;
typedef cwchar ctchar;
typedef wchar utchar;
typedef cwchar cutchar;
#else
typedef char8 tchar;
typedef cchar8 ctchar;
typedef uchar8 utchar;
typedef cuchar8 cutchar;
#endif

/// short int typedefs
typedef const short cshort;
typedef unsigned short ushort;
typedef const unsigned short cushort;

/// int typedefs
typedef const int cint;
typedef unsigned uint;
typedef const unsigned cuint;
typedef const long clong;
typedef unsigned long ulong;
typedef const unsigned long culong;

/// 64-bit long int typedefs
typedef long long long64;
typedef const long long clong64;
typedef unsigned long long ulong64;
typedef const unsigned long long culong64;

/// long_t typedefs
#ifdef OS_WIN64
typedef long long long_t;
typedef const long long clong_t;
typedef unsigned long long ulong_t;
typedef const unsigned long long culong_t;
typedef long long call_ret_t;
#else
typedef long long_t;
typedef const long clong_t;
typedef unsigned long ulong_t;
typedef const unsigned long culong_t;
typedef int call_ret_t;
#endif

/// floating point typedefs
typedef const float cfloat;
typedef const double cdouble;
typedef long double ldouble;
typedef const long double cldouble;

} // cppual

#endif // __cplusplus
#endif // CPPUAL_DECL_H_
