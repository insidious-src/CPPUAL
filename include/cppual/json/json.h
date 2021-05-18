// Tencent is pleased to support the open source community by making Json available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company, and Milo Yip.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef CPPUAL_JSON_H_
#define CPPUAL_JSON_H_

/*!\file json.h
    \brief common definitions and configuration

    \see CPPUAL_JSON_CONFIG
 */

/*! \defgroup CPPUAL_JSON_CONFIG Json configuration
    \brief Configuration macros for library features

    Some Json features are configurable to adapt the library to a wide
    variety of platforms, environments and usage scenarios.  Most of the
    features can be configured in terms of overridden or predefined
    preprocessor macros at compile-time.

    Some additional customization is available in the \ref CPPUAL_JSON_ERRORS APIs.

    \note These macros should be given on the compiler command-line
          (where applicable)  to avoid inconsistent values when compiling
          different translation units of a single application.
 */

#include <cstdlib>  // malloc(), realloc(), free(), size_t
#include <cstring>  // memset(), memcpy(), memmove(), memcmp()

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_VERSION_STRING
//
// ALWAYS synchronize the following 3 macros with corresponding variables in /CMakeLists.txt.
//

//!@cond CPPUAL_JSON_HIDDEN_FROM_DOXYGEN
// token stringification
#define CPPUAL_JSON_STRINGIFY(x) CPPUAL_JSON_DO_STRINGIFY(x)
#define CPPUAL_JSON_DO_STRINGIFY(x) #x

// token concatenation
#define CPPUAL_JSON_JOIN(X, Y) CPPUAL_JSON_DO_JOIN(X, Y)
#define CPPUAL_JSON_DO_JOIN(X, Y) CPPUAL_JSON_DO_JOIN2(X, Y)
#define CPPUAL_JSON_DO_JOIN2(X, Y) X##Y
//!@endcond

/*! \def CPPUAL_JSON_MAJOR_VERSION
    \ingroup CPPUAL_JSON_CONFIG
    \brief Major version of Json in integer.
*/
/*! \def CPPUAL_JSON_MINOR_VERSION
    \ingroup CPPUAL_JSON_CONFIG
    \brief Minor version of Json in integer.
*/
/*! \def CPPUAL_JSON_PATCH_VERSION
    \ingroup CPPUAL_JSON_CONFIG
    \brief Patch version of Json in integer.
*/
/*! \def CPPUAL_JSON_VERSION_STRING
    \ingroup CPPUAL_JSON_CONFIG
    \brief Version of Json in "<major>.<minor>.<patch>" string format.
*/
#define CPPUAL_JSON_MAJOR_VERSION 1
#define CPPUAL_JSON_MINOR_VERSION 1
#define CPPUAL_JSON_PATCH_VERSION 0
#define CPPUAL_JSON_VERSION_STRING \
    CPPUAL_JSON_STRINGIFY(CPPUAL_JSON_MAJOR_VERSION.CPPUAL_JSON_MINOR_VERSION.CPPUAL_JSON_PATCH_VERSION)

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_NAMESPACE_(BEGIN|END)
/*! \def CPPUAL_JSON_NAMESPACE
    \ingroup CPPUAL_JSON_CONFIG
    \brief   provide custom cppual::Json namespace

    In order to avoid symbol clashes and/or "One Definition Rule" errors
    between multiple inclusions of (different versions of) Json in
    a single binary, users can customize the name of the main Json
    namespace.

    In case of a single nesting level, defining \c CPPUAL_JSON_NAMESPACE
    to a custom name (e.g. \c MyJson) is sufficient.  If multiple
    levels are needed, both \ref CPPUAL_JSON_NAMESPACE_BEGIN and \ref
    CPPUAL_JSON_NAMESPACE_END need to be defined as well:

    \code
    // in some .cpp file
    #define CPPUAL_JSON_NAMESPACE my::rapidjson
    #define CPPUAL_JSON_NAMESPACE_BEGIN namespace my { namespace cppual::Json {
    #define CPPUAL_JSON_NAMESPACE_END   } }
    #include "cppual::Json/..."
    \endcode

    \see cppual::Json
 */
/*! \def CPPUAL_JSON_NAMESPACE_BEGIN
    \ingroup CPPUAL_JSON_CONFIG
    \brief   provide custom cppual::Json namespace (opening expression)
    \see CPPUAL_JSON_NAMESPACE
*/
/*! \def CPPUAL_JSON_NAMESPACE_END
    \ingroup CPPUAL_JSON_CONFIG
    \brief   provide custom cppual::Json namespace (closing expression)
    \see CPPUAL_JSON_NAMESPACE
*/
#ifndef CPPUAL_JSON_NAMESPACE
#define CPPUAL_JSON_NAMESPACE Json
#endif
#ifndef CPPUAL_JSON_NAMESPACE_BEGIN
#define CPPUAL_JSON_NAMESPACE_BEGIN namespace cppual { namespace CPPUAL_JSON_NAMESPACE {
#endif
#ifndef CPPUAL_JSON_NAMESPACE_END
#define CPPUAL_JSON_NAMESPACE_END }}
#endif

///////////////////////////////////////////////////////////////////////////////
// __cplusplus macro

//!@cond CPPUAL_JSON_HIDDEN_FROM_DOXYGEN

#if defined(_MSC_VER)
#define CPPUAL_JSON_CPLUSPLUS _MSVC_LANG
#else
#define CPPUAL_JSON_CPLUSPLUS __cplusplus
#endif

//!@endcond

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_HAS_STDSTRING

#ifndef CPPUAL_JSON_HAS_STDSTRING
#ifdef CPPUAL_JSON_DOXYGEN_RUNNING
#define CPPUAL_JSON_HAS_STDSTRING 1 // force generation of documentation
#else
#define CPPUAL_JSON_HAS_STDSTRING 0 // no string support by default
#endif
/*! \def CPPUAL_JSON_HAS_STDSTRING
    \ingroup CPPUAL_JSON_CONFIG
    \brief Enable Json support for \c string

    By defining this preprocessor symbol to \c 1, several convenience functions for using
    \ref cppual::Json::GenericValue with \c string are enabled, especially
    for construction and comparison.

    \hideinitializer
*/
#endif // !defined(CPPUAL_JSON_HAS_STDSTRING)

#if CPPUAL_JSON_HAS_STDSTRING
#include <string>
#endif // CPPUAL_JSON_HAS_STDSTRING

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_USE_MEMBERSMAP

/*! \def CPPUAL_JSON_USE_MEMBERSMAP
    \ingroup CPPUAL_JSON_CONFIG
    \brief Enable Json support for object members handling in a \c std::multimap

    By defining this preprocessor symbol to \c 1, \ref cppual::Json::GenericValue object
    members are stored in a \c std::multimap for faster lookup and deletion times, a
    trade off with a slightly slower insertion time and a small object allocat(or)ed
    memory overhead.

    \hideinitializer
*/
#ifndef CPPUAL_JSON_USE_MEMBERSMAP
#define CPPUAL_JSON_USE_MEMBERSMAP 0 // not by default
#endif

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_NO_INT64DEFINE

/*! \def CPPUAL_JSON_NO_INT64DEFINE
    \ingroup CPPUAL_JSON_CONFIG
    \brief Use external 64-bit integer types.

    Json requires the 64-bit integer types \c int64_t and  \c uint64_t types
    to be available at global scope.

    If users have their own definition, define CPPUAL_JSON_NO_INT64DEFINE to
    prevent Json from defining its own types.
*/
#ifndef CPPUAL_JSON_NO_INT64DEFINE
//!@cond CPPUAL_JSON_HIDDEN_FROM_DOXYGEN
#if defined(_MSC_VER) && (_MSC_VER < 1800)	// Visual Studio 2013
#include "msinttypes/stdint.h"
#include "msinttypes/inttypes.h"
#else
// Other compilers should have this.
#include <stdint.h>
#include <inttypes.h>
#endif
//!@endcond
#ifdef CPPUAL_JSON_DOXYGEN_RUNNING
#define CPPUAL_JSON_NO_INT64DEFINE
#endif
#endif // CPPUAL_JSON_NO_INT64TYPEDEF

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_FORCEINLINE

#ifndef CPPUAL_JSON_FORCEINLINE
//!@cond CPPUAL_JSON_HIDDEN_FROM_DOXYGEN
#if defined(_MSC_VER) && defined(NDEBUG)
#define CPPUAL_JSON_FORCEINLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ >= 4 && defined(NDEBUG)
#define CPPUAL_JSON_FORCEINLINE __attribute__((always_inline))
#else
#define CPPUAL_JSON_FORCEINLINE
#endif
//!@endcond
#endif // CPPUAL_JSON_FORCEINLINE

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_ENDIAN
#define CPPUAL_JSON_LITTLEENDIAN  0   //!< Little endian machine
#define CPPUAL_JSON_BIGENDIAN     1   //!< Big endian machine

//! Endianness of the machine.
/*!
    \def CPPUAL_JSON_ENDIAN
    \ingroup CPPUAL_JSON_CONFIG

    GCC 4.6 provided macro for detecting endianness of the target machine. But other
    compilers may not have this. User can define CPPUAL_JSON_ENDIAN to either
    \ref CPPUAL_JSON_LITTLEENDIAN or \ref CPPUAL_JSON_BIGENDIAN.

    Default detection implemented with reference to
    \li https://gcc.gnu.org/onlinedocs/gcc-4.6.0/cpp/Common-Predefined-Macros.html
    \li http://www.boost.org/doc/libs/1_42_0/boost/detail/endian.hpp
*/
#ifndef CPPUAL_JSON_ENDIAN
// Detect with GCC 4.6's macro
#  ifdef __BYTE_ORDER__
#    if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#      define CPPUAL_JSON_ENDIAN CPPUAL_JSON_LITTLEENDIAN
#    elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#      define CPPUAL_JSON_ENDIAN CPPUAL_JSON_BIGENDIAN
#    else
#      error Unknown machine endianness detected. User needs to define CPPUAL_JSON_ENDIAN.
#    endif // __BYTE_ORDER__
// Detect with GLIBC's endian.h
#  elif defined(__GLIBC__)
#    include <endian.h>
#    if (__BYTE_ORDER == __LITTLE_ENDIAN)
#      define CPPUAL_JSON_ENDIAN CPPUAL_JSON_LITTLEENDIAN
#    elif (__BYTE_ORDER == __BIG_ENDIAN)
#      define CPPUAL_JSON_ENDIAN CPPUAL_JSON_BIGENDIAN
#    else
#      error Unknown machine endianness detected. User needs to define CPPUAL_JSON_ENDIAN.
#   endif // __GLIBC__
// Detect with _LITTLE_ENDIAN and _BIG_ENDIAN macro
#  elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN)
#    define CPPUAL_JSON_ENDIAN CPPUAL_JSON_LITTLEENDIAN
#  elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN)
#    define CPPUAL_JSON_ENDIAN CPPUAL_JSON_BIGENDIAN
// Detect with architecture macros
#  elif defined(__sparc) || defined(__sparc__) || defined(_POWER) || defined(__powerpc__) || defined(__ppc__) || defined(__hpux) || defined(__hppa) || defined(_MIPSEB) || defined(_POWER) || defined(__s390__)
#    define CPPUAL_JSON_ENDIAN CPPUAL_JSON_BIGENDIAN
#  elif defined(__i386__) || defined(__alpha__) || defined(__ia64) || defined(__ia64__) || defined(_M_IX86) || defined(_M_IA64) || defined(_M_ALPHA) || defined(__amd64) || defined(__amd64__) || defined(_M_AMD64) || defined(__x86_64) || defined(__x86_64__) || defined(_M_X64) || defined(__bfin__)
#    define CPPUAL_JSON_ENDIAN CPPUAL_JSON_LITTLEENDIAN
#  elif defined(_MSC_VER) && (defined(_M_ARM) || defined(_M_ARM64))
#    define CPPUAL_JSON_ENDIAN CPPUAL_JSON_LITTLEENDIAN
#  elif defined(CPPUAL_JSON_DOXYGEN_RUNNING)
#    define CPPUAL_JSON_ENDIAN
#  else
#    error Unknown machine endianness detected. User needs to define CPPUAL_JSON_ENDIAN.
#  endif
#endif // CPPUAL_JSON_ENDIAN

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_64BIT

//! Whether using 64-bit architecture
#ifndef CPPUAL_JSON_64BIT
#if defined(__LP64__) || (defined(__x86_64__) && defined(__ILP32__)) || defined(_WIN64) || defined(__EMSCRIPTEN__)
#define CPPUAL_JSON_64BIT 1
#else
#define CPPUAL_JSON_64BIT 0
#endif
#endif // CPPUAL_JSON_64BIT

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_ALIGN

//! Data alignment of the machine.
/*! \ingroup CPPUAL_JSON_CONFIG
    \param x pointer to align

    Some machines require strict data alignment. The default is 8 bytes.
    User can customize by defining the CPPUAL_JSON_ALIGN function macro.
*/
#ifndef CPPUAL_JSON_ALIGN
#define CPPUAL_JSON_ALIGN(x) (((x) + static_cast<size_t>(7u)) & ~static_cast<size_t>(7u))
#endif

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_UINT64_C2

//! Construct a 64-bit literal by a pair of 32-bit integer.
/*!
    64-bit literal with or without ULL suffix is prone to compiler warnings.
    UINT64_C() is C macro which cause compilation problems.
    Use this macro to define 64-bit constants by a pair of 32-bit integer.
*/
#ifndef CPPUAL_JSON_UINT64_C2
#define CPPUAL_JSON_UINT64_C2(high32, low32) ((static_cast<uint64_t>(high32) << 32) | static_cast<uint64_t>(low32))
#endif

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_48BITPOINTER_OPTIMIZATION

//! Use only lower 48-bit address for some pointers.
/*!
    \ingroup CPPUAL_JSON_CONFIG

    This optimization uses the fact that current X86-64 architecture only implement lower 48-bit virtual address.
    The higher 16-bit can be used for storing other data.
    \c GenericValue uses this optimization to reduce its size form 24 bytes to 16 bytes in 64-bit architecture.
*/
#ifndef CPPUAL_JSON_48BITPOINTER_OPTIMIZATION
#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
#define CPPUAL_JSON_48BITPOINTER_OPTIMIZATION 1
#else
#define CPPUAL_JSON_48BITPOINTER_OPTIMIZATION 0
#endif
#endif // CPPUAL_JSON_48BITPOINTER_OPTIMIZATION

#if CPPUAL_JSON_48BITPOINTER_OPTIMIZATION == 1
#if CPPUAL_JSON_64BIT != 1
#error CPPUAL_JSON_48BITPOINTER_OPTIMIZATION can only be set to 1 when CPPUAL_JSON_64BIT=1
#endif
#define CPPUAL_JSON_SETPOINTER(type, p, x) (p = reinterpret_cast<type *>((reinterpret_cast<uintptr_t>(p) & static_cast<uintptr_t>(CPPUAL_JSON_UINT64_C2(0xFFFF0000, 0x00000000))) | reinterpret_cast<uintptr_t>(reinterpret_cast<const void*>(x))))
#define CPPUAL_JSON_GETPOINTER(type, p) (reinterpret_cast<type *>(reinterpret_cast<uintptr_t>(p) & static_cast<uintptr_t>(CPPUAL_JSON_UINT64_C2(0x0000FFFF, 0xFFFFFFFF))))
#else
#define CPPUAL_JSON_SETPOINTER(type, p, x) (p = (x))
#define CPPUAL_JSON_GETPOINTER(type, p) (p)
#endif

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_SSE2/CPPUAL_JSON_SSE42/CPPUAL_JSON_NEON/CPPUAL_JSON_SIMD

/*! \def CPPUAL_JSON_SIMD
    \ingroup CPPUAL_JSON_CONFIG
    \brief Enable SSE2/SSE4.2/Neon optimization.

    Json supports optimized implementations for some parsing operations
    based on the SSE2, SSE4.2 or NEon SIMD extensions on modern Intel
    or ARM compatible processors.

    To enable these optimizations, three different symbols can be defined;
    \code
    // Enable SSE2 optimization.
    #define CPPUAL_JSON_SSE2

    // Enable SSE4.2 optimization.
    #define CPPUAL_JSON_SSE42
    \endcode

    // Enable ARM Neon optimization.
    #define CPPUAL_JSON_NEON
    \endcode

    \c CPPUAL_JSON_SSE42 takes precedence over SSE2, if both are defined.

    If any of these symbols is defined, Json defines the macro
    \c CPPUAL_JSON_SIMD to indicate the availability of the optimized code.
*/
#if defined(CPPUAL_JSON_SSE2) || defined(CPPUAL_JSON_SSE42) \
    || defined(CPPUAL_JSON_NEON) || defined(CPPUAL_JSON_DOXYGEN_RUNNING)
#define CPPUAL_JSON_SIMD
#endif

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_NO_SIZETYPEDEFINE

#ifndef CPPUAL_JSON_NO_SIZETYPEDEFINE
/*! \def CPPUAL_JSON_NO_SIZETYPEDEFINE
    \ingroup CPPUAL_JSON_CONFIG
    \brief User-provided \c SizeType definition.

    In order to avoid using 32-bit size types for indexing strings and arrays,
    define this preprocessor symbol and provide the type cppual::Json::SizeType
    before including Json:
    \code
    #define CPPUAL_JSON_NO_SIZETYPEDEFINE
    namespace cppual::Json { typedef ::std::size_t SizeType; }
    #include "cppual::Json/..."
    \endcode

    \see cppual::Json::SizeType
*/
#ifdef CPPUAL_JSON_DOXYGEN_RUNNING
#define CPPUAL_JSON_NO_SIZETYPEDEFINE
#endif
CPPUAL_JSON_NAMESPACE_BEGIN
//! Size type (for string lengths, array sizes, etc.)
/*! Json uses 32-bit array/string indices even on 64-bit platforms,
    instead of using \c size_t. Users may override the SizeType by defining
    \ref CPPUAL_JSON_NO_SIZETYPEDEFINE.
*/
typedef unsigned SizeType;
CPPUAL_JSON_NAMESPACE_END
#endif

// always import std::size_t to cppual::Json namespace
CPPUAL_JSON_NAMESPACE_BEGIN
using std::size_t;
CPPUAL_JSON_NAMESPACE_END

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_ASSERT

//! Assertion.
/*! \ingroup CPPUAL_JSON_CONFIG
    By default, cppual::Json uses C \c assert() for internal assertions.
    User can override it by defining CPPUAL_JSON_ASSERT(x) macro.

    \note Parsing errors are handled and can be customized by the
          \ref CPPUAL_JSON_ERRORS APIs.
*/
#ifndef CPPUAL_JSON_ASSERT
#include <cassert>
#define CPPUAL_JSON_ASSERT(x) assert(x)
#endif // CPPUAL_JSON_ASSERT

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_STATIC_ASSERT

// Prefer C++11 static_assert, if available
#ifndef CPPUAL_JSON_STATIC_ASSERT
#if CPPUAL_JSON_CPLUSPLUS >= 201103L || ( defined(_MSC_VER) && _MSC_VER >= 1800 )
#define CPPUAL_JSON_STATIC_ASSERT(x) \
   static_assert(x, CPPUAL_JSON_STRINGIFY(x))
#endif // C++11
#endif // CPPUAL_JSON_STATIC_ASSERT

// Adopt C++03 implementation from boost
#ifndef CPPUAL_JSON_STATIC_ASSERT
#ifndef __clang__
//!@cond CPPUAL_JSON_HIDDEN_FROM_DOXYGEN
#endif
CPPUAL_JSON_NAMESPACE_BEGIN
template <bool x> struct STATIC_ASSERTION_FAILURE;
template <> struct STATIC_ASSERTION_FAILURE<true> { enum { value = 1 }; };
template <size_t x> struct StaticAssertTest {};
CPPUAL_JSON_NAMESPACE_END

#if defined(__GNUC__) || defined(__clang__)
#define CPPUAL_JSON_STATIC_ASSERT_UNUSED_ATTRIBUTE __attribute__((unused))
#else
#define CPPUAL_JSON_STATIC_ASSERT_UNUSED_ATTRIBUTE
#endif
#ifndef __clang__
//!@endcond
#endif

/*! \def CPPUAL_JSON_STATIC_ASSERT
    \brief (Internal) macro to check for conditions at compile-time
    \param x compile-time condition
    \hideinitializer
 */
#define CPPUAL_JSON_STATIC_ASSERT(x) \
    typedef ::CPPUAL_JSON_NAMESPACE::StaticAssertTest< \
      sizeof(::CPPUAL_JSON_NAMESPACE::STATIC_ASSERTION_FAILURE<bool(x) >)> \
    CPPUAL_JSON_JOIN(StaticAssertTypedef, __LINE__) CPPUAL_JSON_STATIC_ASSERT_UNUSED_ATTRIBUTE
#endif // CPPUAL_JSON_STATIC_ASSERT

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_LIKELY, CPPUAL_JSON_UNLIKELY

//! Compiler branching hint for expression with high probability to be true.
/*!
    \ingroup CPPUAL_JSON_CONFIG
    \param x Boolean expression likely to be true.
*/
#ifndef CPPUAL_JSON_LIKELY
#if defined(__GNUC__) || defined(__clang__)
#define CPPUAL_JSON_LIKELY(x) __builtin_expect(!!(x), 1)
#else
#define CPPUAL_JSON_LIKELY(x) (x)
#endif
#endif

//! Compiler branching hint for expression with low probability to be true.
/*!
    \ingroup CPPUAL_JSON_CONFIG
    \param x Boolean expression unlikely to be true.
*/
#ifndef CPPUAL_JSON_UNLIKELY
#if defined(__GNUC__) || defined(__clang__)
#define CPPUAL_JSON_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define CPPUAL_JSON_UNLIKELY(x) (x)
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
// Helpers

//!@cond CPPUAL_JSON_HIDDEN_FROM_DOXYGEN

#define CPPUAL_JSON_MULTILINEMACRO_BEGIN do {
#define CPPUAL_JSON_MULTILINEMACRO_END \
} while((void)0, 0)

// adopted from Boost
#define CPPUAL_JSON_VERSION_CODE(x,y,z) \
  (((x)*100000) + ((y)*100) + (z))

#if defined(__has_builtin)
#define CPPUAL_JSON_HAS_BUILTIN(x) __has_builtin(x)
#else
#define CPPUAL_JSON_HAS_BUILTIN(x) 0
#endif

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_DIAG_PUSH/POP, CPPUAL_JSON_DIAG_OFF

#if defined(__GNUC__)
#define CPPUAL_JSON_GNUC \
    CPPUAL_JSON_VERSION_CODE(__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)
#endif

#if defined(__clang__) || (defined(CPPUAL_JSON_GNUC) && CPPUAL_JSON_GNUC >= CPPUAL_JSON_VERSION_CODE(4,2,0))

#define CPPUAL_JSON_PRAGMA(x) _Pragma(CPPUAL_JSON_STRINGIFY(x))
#define CPPUAL_JSON_DIAG_PRAGMA(x) CPPUAL_JSON_PRAGMA(GCC diagnostic x)
#define CPPUAL_JSON_DIAG_OFF(x) \
    CPPUAL_JSON_DIAG_PRAGMA(ignored CPPUAL_JSON_STRINGIFY(CPPUAL_JSON_JOIN(-W,x)))

// push/pop support in Clang and GCC>=4.6
#if defined(__clang__) || (defined(CPPUAL_JSON_GNUC) && CPPUAL_JSON_GNUC >= CPPUAL_JSON_VERSION_CODE(4,6,0))
#define CPPUAL_JSON_DIAG_PUSH CPPUAL_JSON_DIAG_PRAGMA(push)
#define CPPUAL_JSON_DIAG_POP  CPPUAL_JSON_DIAG_PRAGMA(pop)
#else // GCC >= 4.2, < 4.6
#define CPPUAL_JSON_DIAG_PUSH /* ignored */
#define CPPUAL_JSON_DIAG_POP /* ignored */
#endif

#elif defined(_MSC_VER)

// pragma (MSVC specific)
#define CPPUAL_JSON_PRAGMA(x) __pragma(x)
#define CPPUAL_JSON_DIAG_PRAGMA(x) CPPUAL_JSON_PRAGMA(warning(x))

#define CPPUAL_JSON_DIAG_OFF(x) CPPUAL_JSON_DIAG_PRAGMA(disable: x)
#define CPPUAL_JSON_DIAG_PUSH CPPUAL_JSON_DIAG_PRAGMA(push)
#define CPPUAL_JSON_DIAG_POP  CPPUAL_JSON_DIAG_PRAGMA(pop)

#else

#define CPPUAL_JSON_DIAG_OFF(x) /* ignored */
#define CPPUAL_JSON_DIAG_PUSH   /* ignored */
#define CPPUAL_JSON_DIAG_POP    /* ignored */

#endif // CPPUAL_JSON_DIAG_*

///////////////////////////////////////////////////////////////////////////////
// C++11 features

#ifndef CPPUAL_JSON_HAS_CXX11
#define CPPUAL_JSON_HAS_CXX11 (CPPUAL_JSON_CPLUSPLUS >= 201103L)
#endif

#ifndef CPPUAL_JSON_HAS_CXX11_RVALUE_REFS
#if CPPUAL_JSON_HAS_CXX11
#define CPPUAL_JSON_HAS_CXX11_RVALUE_REFS 1
#elif defined(__clang__)
#if __has_feature(cxx_rvalue_references) && \
    (defined(_MSC_VER) || defined(_LIBCPP_VERSION) || defined(__GLIBCXX__) && __GLIBCXX__ >= 20080306)
#define CPPUAL_JSON_HAS_CXX11_RVALUE_REFS 1
#else
#define CPPUAL_JSON_HAS_CXX11_RVALUE_REFS 0
#endif
#elif (defined(CPPUAL_JSON_GNUC) && (CPPUAL_JSON_GNUC >= CPPUAL_JSON_VERSION_CODE(4,3,0)) && defined(__GXX_EXPERIMENTAL_CXX0X__)) || \
      (defined(_MSC_VER) && _MSC_VER >= 1600) || \
      (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x5140 && defined(__GXX_EXPERIMENTAL_CXX0X__))

#define CPPUAL_JSON_HAS_CXX11_RVALUE_REFS 1
#else
#define CPPUAL_JSON_HAS_CXX11_RVALUE_REFS 0
#endif
#endif // CPPUAL_JSON_HAS_CXX11_RVALUE_REFS

#if CPPUAL_JSON_HAS_CXX11_RVALUE_REFS
#include <utility> // std::move
#endif

#ifndef CPPUAL_JSON_HAS_CXX11_NOEXCEPT
#if CPPUAL_JSON_HAS_CXX11
#define CPPUAL_JSON_HAS_CXX11_NOEXCEPT 1
#elif defined(__clang__)
#define CPPUAL_JSON_HAS_CXX11_NOEXCEPT __has_feature(cxx_noexcept)
#elif (defined(CPPUAL_JSON_GNUC) && (CPPUAL_JSON_GNUC >= CPPUAL_JSON_VERSION_CODE(4,6,0)) && defined(__GXX_EXPERIMENTAL_CXX0X__)) || \
    (defined(_MSC_VER) && _MSC_VER >= 1900) || \
    (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x5140 && defined(__GXX_EXPERIMENTAL_CXX0X__))
#define CPPUAL_JSON_HAS_CXX11_NOEXCEPT 1
#else
#define CPPUAL_JSON_HAS_CXX11_NOEXCEPT 0
#endif
#endif
#ifndef CPPUAL_JSON_NOEXCEPT
#if CPPUAL_JSON_HAS_CXX11_NOEXCEPT
#define CPPUAL_JSON_NOEXCEPT noexcept
#else
#define CPPUAL_JSON_NOEXCEPT throw()
#endif // CPPUAL_JSON_HAS_CXX11_NOEXCEPT
#endif

// no automatic detection, yet
#ifndef CPPUAL_JSON_HAS_CXX11_TYPETRAITS
#if (defined(_MSC_VER) && _MSC_VER >= 1700)
#define CPPUAL_JSON_HAS_CXX11_TYPETRAITS 1
#else
#define CPPUAL_JSON_HAS_CXX11_TYPETRAITS 0
#endif
#endif

#ifndef CPPUAL_JSON_HAS_CXX11_RANGE_FOR
#if defined(__clang__)
#define CPPUAL_JSON_HAS_CXX11_RANGE_FOR __has_feature(cxx_range_for)
#elif (defined(CPPUAL_JSON_GNUC) && (CPPUAL_JSON_GNUC >= CPPUAL_JSON_VERSION_CODE(4,6,0)) && defined(__GXX_EXPERIMENTAL_CXX0X__)) || \
      (defined(_MSC_VER) && _MSC_VER >= 1700) || \
      (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x5140 && defined(__GXX_EXPERIMENTAL_CXX0X__))
#define CPPUAL_JSON_HAS_CXX11_RANGE_FOR 1
#else
#define CPPUAL_JSON_HAS_CXX11_RANGE_FOR 0
#endif
#endif // CPPUAL_JSON_HAS_CXX11_RANGE_FOR

///////////////////////////////////////////////////////////////////////////////
// C++17 features

#ifndef CPPUAL_JSON_HAS_CXX17
#define CPPUAL_JSON_HAS_CXX17 (CPPUAL_JSON_CPLUSPLUS >= 201703L)
#endif

#if CPPUAL_JSON_HAS_CXX17
# define CPPUAL_JSON_DELIBERATE_FALLTHROUGH [[fallthrough]]
#elif defined(__has_cpp_attribute)
# if __has_cpp_attribute(clang::fallthrough)
#  define CPPUAL_JSON_DELIBERATE_FALLTHROUGH [[clang::fallthrough]]
# elif __has_cpp_attribute(fallthrough)
#  define CPPUAL_JSON_DELIBERATE_FALLTHROUGH __attribute__((fallthrough))
# else
#  define CPPUAL_JSON_DELIBERATE_FALLTHROUGH
# endif
#else
# define CPPUAL_JSON_DELIBERATE_FALLTHROUGH
#endif

//!@endcond

//! Assertion (in non-throwing contexts).
 /*! \ingroup CPPUAL_JSON_CONFIG
    Some functions provide a \c noexcept guarantee, if the compiler supports it.
    In these cases, the \ref CPPUAL_JSON_ASSERT macro cannot be overridden to
    throw an exception.  This macro adds a separate customization point for
    such cases.

    Defaults to C \c assert() (as \ref CPPUAL_JSON_ASSERT), if \c noexcept is
    supported, and to \ref CPPUAL_JSON_ASSERT otherwise.
 */

///////////////////////////////////////////////////////////////////////////////
// CPPUAL_JSON_NOEXCEPT_ASSERT

#ifndef CPPUAL_JSON_NOEXCEPT_ASSERT
#ifdef CPPUAL_JSON_ASSERT_THROWS
#include <cassert>
#define CPPUAL_JSON_NOEXCEPT_ASSERT(x) assert(x)
#else
#define CPPUAL_JSON_NOEXCEPT_ASSERT(x) CPPUAL_JSON_ASSERT(x)
#endif // CPPUAL_JSON_ASSERT_THROWS
#endif // CPPUAL_JSON_NOEXCEPT_ASSERT

///////////////////////////////////////////////////////////////////////////////
// malloc/realloc/free

#ifndef CPPUAL_JSON_MALLOC
///! customization point for global \c malloc
#define CPPUAL_JSON_MALLOC(size) std::malloc(size)
#endif
#ifndef CPPUAL_JSON_REALLOC
///! customization point for global \c realloc
#define CPPUAL_JSON_REALLOC(ptr, new_size) std::realloc(ptr, new_size)
#endif
#ifndef CPPUAL_JSON_FREE
///! customization point for global \c free
#define CPPUAL_JSON_FREE(ptr) std::free(ptr)
#endif

///////////////////////////////////////////////////////////////////////////////
// new/delete

#ifndef CPPUAL_JSON_NEW
///! customization point for global \c new
#define CPPUAL_JSON_NEW(TypeName) new TypeName
#endif
#ifndef CPPUAL_JSON_DELETE
///! customization point for global \c delete
#define CPPUAL_JSON_DELETE(x) delete x
#endif

///////////////////////////////////////////////////////////////////////////////
// Type

/*! \namespace cppual::Json
    \brief main Json namespace
    \see CPPUAL_JSON_NAMESPACE
*/
CPPUAL_JSON_NAMESPACE_BEGIN

//! Type of JSON value
enum Type {
    kNullType = 0,      //!< null
    kFalseType = 1,     //!< false
    kTrueType = 2,      //!< true
    kObjectType = 3,    //!< object
    kArrayType = 4,     //!< array
    kStringType = 5,    //!< string
    kNumberType = 6     //!< number
};

CPPUAL_JSON_NAMESPACE_END

#endif // CPPUAL_JSON_H_
