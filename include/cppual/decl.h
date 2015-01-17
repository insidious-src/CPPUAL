/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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
#undef OS_STD_MAC
#undef OS_GNU_LINUX
#undef OS_BSD
#undef OS_ANDROID
#undef OS_AIX
#undef OS_SOLARIS
#undef OS_IOS
#undef OS_LEOPARD_X
#undef OS_WIN32
#undef OS_WIN64
#undef OS_WINDOWS
#undef TXT
#undef TXT_STD_ASCII
#undef TXT_STD_UTF8
#undef TXT_STD_UTF16
#undef TXT_STD_UTF32
#undef TXT_STD_OLD_UNICODE
#undef API

// operating system support verification & definitions
#if defined (_WIN32) or defined (__WIN32) or defined (__WIN32__)
#
#	define OS_WIN32
#	define OS_WINDOWS OS_WIN32
#	define OS_CURRENT OS_WINDOWS
#
#elif defined (_WIN64) or defined (__WIN64) or defined (__WIN64__)
#
#	define OS_WIN64
#	define OS_WINDOWS OS_WIN64
#	define OS_CURRENT OS_WINDOWS
#
#elif defined (__unix__) or defined (__unix)
#
#	define OS_STD_UNIX
#	include <unistd.h>
#	if (defined (_POSIX_VERSION))
#		define OS_STD_POSIX _POSIX_VERSION
#	endif // POSIX
#	if defined (__linux__) or defined (__linux)
#		define OS_GNU_LINUX
#		define OS_CURRENT OS_GNU_LINUX
#	elif defined (BSD)
#		define OS_BSD
#		define OS_CURRENT OS_BSD
#	elif defined (_AIX)
#		define OS_AIX
#		define OS_CURRENT OS_AIX
#		error aix support not implemented yet!
#	elif (defined (__sun) or defined (__sun__)) and (defined (__SVR4) or defined (__SVR4__))
#		define OS_SOLARIS
#		define OS_CURRENT OS_SOLARIS
#		error solaris support not implemented yet!
#	endif
#
#elif defined (__APPLE__) and defined (__MACH__)
#	define OS_STD_UNIX
#	define OS_STD_MAC
#	include <unistd.h>
#	if (defined (_POSIX_VERSION))
#		define OS_STD_POSIX _POSIX_VERSION
#	endif // POSIX
#else
#	error unsupported operating system!
#endif // OS

#undef PACKED
#undef NOVTABLE
#undef DEPRECATED
#undef FORCEINLINE
#undef DECLSPEC_ALIGN

#if defined (_MSC_VER) and (_MSC_VER > 1300) // vc++ 2005
#
#   pragma once
#	define PACKED __declspec(align(packed))
#	define NOVTABLE __declspec(novtable)
#	define DEPRECATED(msg)
#   define FORCEINLINE __forceinline
#   define DECLSPEC_ALIGN(x) __declspec(align(x))
#   if !defined (STDCALL) and !defined (FASTCALL) and !defined (CDECL)
#       define STDCALL __stdcall
#       define FASTCALL __fastcall
#       define CDECL __cdecl
#   endif // STDCALL and CDECL and FASTCALL
#
#elif defined (__GNUC__)
#
#	define PACKED __attribute__ ((__packed__))
#	define NOVTABLE
#	define DEPRECATED(msg) __attribute__ ((deprecated(msg)))
//#   define FORCEINLINE __inline__
#   define FORCEINLINE inline __attribute__((always_inline))
#   define DECLSPEC_ALIGN(x) __attribute__((aligned(x)))
#   if !defined (STDCALL) and !defined (FASTCALL) and !defined (CDECL)
#       define STDCALL  __attribute__((stdcall))
#       define FASTCALL __attribute__((fastcall))
#       define CDECL
#   endif // STDCALL and CDECL and FASTCALL
#
#else
#
#	define PACKED
#	define NOVTABLE
#	define DEPRECATED(msg)
#   define FORCEINLINE
#   define DECLSPEC_ALIGN(x)
#   if !defined (STDCALL) and !defined (FASTCALL) and !defined (CDECL)
#       define STDCALL
#       define FASTCALL
#       define CDECL
#   endif // STDCALL and CDECL and FASTCALL
#
#endif // _MSC_VER and __GNUC__

#if !defined (DECL_EXPORT) and !defined (DECL_IMPORT) and !defined (DECL_HIDDEN)
#
#	ifdef __GNUC__
#		define DECL_EXPORT __attribute__((visibility("default")))
#		define DECL_IMPORT __attribute__((visibility("default")))
#  		define DECL_HIDDEN __attribute__((visibility("hidden")))
#	elif defined (_MSC_VER)
#    	define DECL_EXPORT __declspec(dllexport)
#    	define DECL_IMPORT __declspec(dllimport)
#  		define DECL_HIDDEN
#	else
#    	define DECL_IMPORT
#    	define DECL_EXPORT
#  		define DECL_HIDDEN
#	endif
#
#endif // DECL_EXPORT and DECL_IMPORT and DECL_HIDDEN

#ifndef TRANSPARENT_UNION
#
#	ifdef __GNUC__
#		define TRANSPARENT_UNION __attribute__((__transparent_union__))
#	else
#    	define TRANSPARENT_UNION
#	endif
#
#endif // TRANSPARENT_UNION

#ifdef API_EXPORT
#	define SHARED_API DECL_EXPORT
#else
#	define SHARED_API DECL_IMPORT
#endif

#if !defined (NDEBUG) or defined (__DEBUG) or defined (_DEBUG) or defined (__DEBUG__)
#   define DEBUG_MODE
#endif // DEBUG

#ifdef __cplusplus

#if __cplusplus < 201103L or (defined (_MSC_VER) and _MSC_VER < 1700)
#	error this compiler does not support ISO C++11!
#endif

// character storage literals
#ifdef __STD_UTF_8__
#	define TXT_STD_UTF8
#	define TXT(quote) u8 ## quote
#elif defined (__STD_UTF_16__)
#	define TXT_STD_UTF16
#	define TXT(quote) u ## quote
#elif defined (__STD_UTF_32__)
#	define TXT_STD_UTF32
#	define TXT(quote) U ## quote
#elif defined (__STD_UNICODE__)
#	define TXT_STD_OLD_UNICODE
#	define TXT(quote) L ## quote
#else
#	define TXT_STD_ASCII
#	define TXT(quote) quote
#endif

template <typename T, typename U>
inline U pointer_cast (T value)
{
#	ifdef DEBUG_MODE
	return dynamic_cast<U> (value);
#	else
	return static_cast<U>  (value);
#	endif
}


namespace cppual
{

// void typedefs
typedef const void cvoid;

// bool typedefs
typedef const bool cbool;

// ASCII & UTF-8 sized typedefs
typedef const char cchar;
typedef unsigned char uchar;
typedef const unsigned cuchar;

typedef char char8;
typedef cchar cchar8;
typedef uchar uchar8;
typedef cuchar cuchar8;

// UTF-16 sized typedefs
typedef char16_t char16;
typedef const char16_t cchar16;

// UTF-32 sized typedefs
typedef char32_t char32;
typedef const char32_t cchar32;

// old UNICODE typedefs
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

// 1 byte size typedefs
typedef uchar8 byte;
typedef cuchar8 cbyte;

// short int typedefs
typedef const short cshort;
typedef unsigned short ushort;
typedef const unsigned short cushort;

// int typedefs
typedef const int cint;
typedef unsigned uint;
typedef const unsigned cuint;
typedef const long clong;
typedef unsigned long ulong;
typedef const unsigned long culong;

// 64-bit long int typedefs
typedef long long long64;
typedef const long long clong64;
typedef unsigned long long ulong64;
typedef const unsigned long long culong64;

// long_t typedefs
#ifdef OS_WIN64
typedef long long long_t;
typedef const long long clong_t;
typedef unsigned long long ulong_t;
typedef const unsigned long long culong_t;
#else
typedef long long_t;
typedef const long clong_t;
typedef unsigned long ulong_t;
typedef const unsigned long culong_t;
#endif

// floating point typedefs
typedef const float cfloat;
typedef const double cdouble;
typedef long double ldouble;
typedef const long double cldouble;

} // cppual

#endif // __cplusplus
#endif // CPPUAL_DECL_H_
