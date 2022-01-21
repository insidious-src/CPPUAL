/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

#ifndef CPPUAL_RESOURCE_H_
#define CPPUAL_RESOURCE_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/flags.h>
#include <cppual/meta.h>

#include <type_traits>

namespace cppual {

class   Handle           ;
typedef Handle Connection;

// =========================================================

enum class ResourceType : u32
{
    Null           =       0,
    Instance       = 1 <<  0,
    Device         = 1 <<  1,
    Buffer         = 1 <<  2,
    Image          = 1 <<  3,
    Font           = 1 <<  4,
    Macro          = 1 <<  5,
    Texture        = 1 <<  6,
    Shader         = 1 <<  7,
    Program        = 1 <<  8,
    Query          = 1 <<  9,
    Window         = 1 << 10,
    Surface        = 1 << 11,
    Pixmap         = 1 << 12,
    Colormap       = 1 << 13,
    Cursor         = 1 << 14,
    GlyphCursor    = 1 << 15,
    Context        = 1 << 16,
    Queue          = 1 << 17,
    Event          = 1 << 18,
    State          = 1 << 19,
    Sampler        = 1 << 20,
    RenderPass     = 1 << 21,
    DescriptorPool = 1 << 22,
    Custom         = 1 << 30
};

// =========================================================

typedef BitSet<ResourceType> resource_types;

// =========================================================

class Handle final
{
public:
    typedef void*             pointer  ;
    typedef pointer Handle::* safe_bool;

    constexpr Handle () noexcept = default;
    constexpr Handle (pointer handle) noexcept : _M_handle (handle) { }
    constexpr Handle (std::nullptr_t) noexcept : _M_handle ()       { }
    inline    Handle (Handle&&) noexcept         = default;
    constexpr Handle (Handle const&) noexcept    = default;
    inline    Handle& operator = (Handle&&)      = default;
    inline    Handle& operator = (Handle const&) = default;

    template <typename T,
              typename = typename std::enable_if<is_integer<T>::value>::type
              >
    constexpr Handle (T handle) noexcept : _M_handle (unsafe_direct_cast<pointer> (handle))
    {
        static_assert (sizeof (T) <= sizeof (pointer), "T is bigger than the size of a pointer!");
    }

    constexpr operator pointer () const noexcept
    { return _M_handle; }

    constexpr operator uptr () const noexcept
    { return direct_cast<uptr> (_M_handle); }

    template <typename T,
              typename =
              typename std::enable_if<is_integer<T>::value>::type
              >
    constexpr T get () const noexcept
    {
        static_assert (sizeof (T) <= sizeof (pointer), "T is bigger than the size of a pointer!");
        return unsafe_direct_cast<T> (_M_handle);
    }

    template <typename T,
              typename =
              typename std::enable_if<(std::is_pointer<T>::value  ||
                                       std::is_class  <T>::value) &&
                                      !std::is_same<typename std::remove_cv<T>::type, Handle>::value
                                      >::type
              >
    constexpr typename std::remove_pointer<T>::type* get () const noexcept
    {
        return static_cast<typename std::remove_pointer<T>::type*> (_M_handle);
    }

    template <typename T,
              typename =
              typename std::enable_if<
                  std::is_same<typename std::remove_cv<T>::type, Handle>::value
                  >::type
              >
    constexpr Handle get () const noexcept
    {
        return *this;
    }

    constexpr explicit operator safe_bool () const noexcept
    {
        return _M_handle ? &Handle::_M_handle : nullptr;
    }

    friend
    constexpr bool operator == (Handle const&, Handle const&) noexcept;

    friend
    constexpr bool operator == (Handle const&, std::nullptr_t) noexcept;

private:
    pointer _M_handle { };
};

// =========================================================

constexpr bool operator == (Handle const& conn1, Handle const& conn2) noexcept
{ return conn1._M_handle == conn2._M_handle; }

constexpr bool operator == (Handle const& conn1, std::nullptr_t) noexcept
{ return conn1._M_handle == nullptr; }

constexpr bool operator != (Handle const& conn1, Handle const& conn2) noexcept
{ return !(conn1 == conn2); }

constexpr bool operator != (Handle const& conn1, std::nullptr_t) noexcept
{ return !(conn1 == nullptr); }

// =========================================================

struct Version
{
    constexpr Version () noexcept
    : Version (0, 0)
    { }

    constexpr Version (int _major, int _minor) noexcept
    : major(_major), minor(_minor)
    { }

    int major, minor;
};

// ====================================================

constexpr bool operator  < (Version const& gObj1, Version const& gObj2) noexcept
{ return (gObj1.major < gObj2.major || gObj1.minor < gObj2.minor); }

constexpr bool operator  < (Version const& gObj1, int uMajor) noexcept
{ return gObj1.major  < uMajor; }

constexpr bool operator  <= (Version const& gObj1, Version const& gObj2) noexcept
{ return (gObj1.major <= gObj2.major && gObj1.minor <= gObj2.minor); }

constexpr bool operator  <= (Version const& gObj1, int uMajor) noexcept
{ return gObj1.major  <= uMajor; }

constexpr bool operator  > (Version const& gObj1, Version const& gObj2) noexcept
{ return (gObj1.major > gObj2.major || gObj1.minor > gObj2.minor); }

constexpr bool operator  > (Version const& gObj1, int uMajor) noexcept
{ return gObj1.major  > uMajor; }

constexpr bool operator  >= (Version const& gObj1, Version const& gObj2) noexcept
{ return (gObj1.major >= gObj2.major && gObj1.minor >= gObj2.minor); }

constexpr bool operator  >= (Version const& gObj1, int uMajor) noexcept
{ return gObj1.major  >= uMajor; }

constexpr bool operator  == (Version const& gObj1, Version const& gObj2) noexcept
{ return (gObj1.major == gObj2.major && gObj1.minor == gObj2.minor); }

constexpr bool operator  != (Version const& gObj1, Version const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

// =========================================================

template <class Controller, class ID>
class Resource
{
public:
    typedef ID                    value_type ;
    typedef Controller            controller ;
    typedef Handle                handle_type;
    typedef Resource* Resource::* safe_bool  ;

    constexpr Resource () noexcept = default;
    Resource (Resource&&) = default;
    Resource& operator = (Resource&&) = default;
    Resource (Resource const&) = delete;
    Resource& operator = (Resource const&) = delete;

    virtual ~Resource () { }

    constexpr controller connection () const noexcept { return _M_connection; }
    constexpr bool       valid      () const noexcept { return _M_handle    ; }

    template <typename T = value_type>
    inline T handle () const noexcept { return _M_handle.get<T>(); }

    constexpr Resource (controller conn, value_type id) noexcept
    : _M_connection (conn),
      _M_handle     (id  )
    { }

    constexpr explicit operator safe_bool () const noexcept
    {
        return _M_connection && _M_handle ? this : nullptr;
    }

    template <class Controller_, class ID_>
    friend constexpr bool operator == (Resource<Controller_, ID_> const&,
                                       Resource<Controller_, ID_> const&);

private:
    controller  _M_connection { };
    handle_type _M_handle     { };
};

// =========================================================

template <class ID>
class Resource <void, ID>
{
public:
    typedef ID                       value_type ;
    typedef void                     controller ;
    typedef Handle                   handle_type;
    typedef handle_type* Resource::* safe_bool  ;

    constexpr Resource () noexcept = default;
    Resource (Resource&&) = default;
    Resource& operator = (Resource&&) = default;
    Resource (Resource const&) = delete;
    Resource& operator = (Resource const&) = delete;

    virtual ~Resource () { }

    constexpr bool valid () const noexcept { return _M_handle; }

    template <typename T = value_type>
    inline T handle () const noexcept { return _M_handle.get<T>(); }

    constexpr Resource (value_type id) noexcept
    : _M_handle (id)
    { }

    constexpr explicit operator safe_bool () const noexcept
    {
        return _M_handle ? &Resource::_M_handle : nullptr;
    }

    template <class ID_>
    friend constexpr bool operator == (Resource<void, ID_> const&,
                                       Resource<void, ID_> const&);

private:
    handle_type _M_handle;
};

// =========================================================

template <class ID>
constexpr bool operator == (Resource<void, ID> const& gObj1,
                            Resource<void, ID> const& gObj2)
{ return gObj1._M_handle == gObj2._M_handle; }

template <class ID>
constexpr bool operator != (Resource<void, ID> const& gObj1,
                            Resource<void, ID> const& gObj2)
{ return !(gObj1 == gObj2); }

template <class Controller, class ID>
constexpr bool operator == (Resource<Controller, ID> const& gObj1,
                            Resource<Controller, ID> const& gObj2)
{
    return gObj1.m_pCon == gObj2.m_pCon and gObj1._M_handle == gObj2._M_handle;
}

template <class Controller, class ID>
constexpr bool operator != (Resource<Controller, ID> const& gObj1,
                            Resource<Controller, ID> const& gObj2)
{ return !(gObj1 == gObj2); }

// =========================================================

} // cppual

namespace std {

template <typename CharT, typename Traits>
basic_ostream<CharT, Traits>&
operator << (std::basic_ostream<CharT, Traits>& stream, cppual::Version const& u)
{ return stream << u.major << "." << u.minor; }

template <>
struct hash<cppual::Version>
{
    /// Compute individual hash values for major,
    /// and minor and combine them using XOR
    /// and bit shifting
    size_t operator () (cppual::Version const& version) const
    {
        return ((hash<int>()(version.major) ^ (hash<int>()(version.minor) << 1)) >> 1);
    }
};

}

#endif // __cplusplus
#endif // CPPUAL_GFX_RESOURCE_H_
