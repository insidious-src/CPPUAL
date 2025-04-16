/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#include <cppual/cast.h>
#include <cppual/types.h>
#include <cppual/bitset.h>
#include <cppual/noncopyable.h>
#include <cppual/concept/concepts.h>

#include <type_traits>

namespace cppual {

// =========================================================

class   resource_handle                    ;
typedef resource_handle resource_connection;

// =========================================================

enum class resource_type : u32
{
    null            =       0,
    instance        = 1 <<  0,
    device          = 1 <<  1,
    buffer          = 1 <<  2,
    image           = 1 <<  3,
    font            = 1 <<  4,
    macro           = 1 <<  5,
    texture         = 1 <<  6,
    source_code     = 1 <<  7,
    program         = 1 <<  8,
    query           = 1 <<  9,
    window          = 1 << 10,
    surface         = 1 << 11,
    pixmap          = 1 << 12,
    colormap        = 1 << 13,
    cursor          = 1 << 14,
    glyph_cursor    = 1 << 15,
    context         = 1 << 16,
    pipeline        = 1 << 17,
    queue           = 1 << 18,
    event           = 1 << 19,
    state           = 1 << 20,
    sampler         = 1 << 21,
    render_pass     = 1 << 22,
    descriptor_pool = 1 << 23,
    socket          = 1 << 24,
    custom          = 1 << 30
};

// =========================================================

typedef bitset<resource_type> resource_types;

// =========================================================

class resource_handle
{
public:
    typedef resource_handle self_type ;
    typedef uptr            value_type;
    typedef void*           pointer   ;

    union handle_union
    {
        value_type value { };
        pointer    ptr      ;

        consteval handle_union () noexcept = default;
        constexpr handle_union (value_type _handle) noexcept : value (_handle) { }
        constexpr handle_union (pointer    _handle) noexcept : ptr   (_handle) { }
        constexpr handle_union (std::nullptr_t    ) noexcept : ptr   ()        { }
    };

    typedef handle_union self_type::* safe_bool;

    consteval resource_handle () noexcept = default;
    constexpr resource_handle (value_type _handle) noexcept : _M_handle (_handle) { }
    constexpr resource_handle (pointer    _handle) noexcept : _M_handle (_handle) { }
    constexpr resource_handle (std::nullptr_t    ) noexcept : _M_handle ()        { }

    constexpr resource_handle (self_type &&)           noexcept = default;
    constexpr resource_handle (self_type const&)       noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    template <integer_t T>
    constexpr explicit resource_handle (T const _handle) noexcept
    : _M_handle (static_cast<value_type> (_handle))
    {
        static_assert (sizeof (T) <= sizeof (value_type), "T is bigger than the size of uptr!");
    }

    constexpr operator pointer    () const noexcept { return _M_handle.ptr  ; }
    constexpr operator value_type () const noexcept { return _M_handle.value; }

    constexpr bool valid () const noexcept
    {
        return _M_handle.value != value_type ();
    }

    template <integer_t T = value_type>
    constexpr T get () const noexcept
    {
        static_assert (sizeof (T) <= sizeof (value_type), "T is bigger than the size of uptr!");
        return static_cast<T> (_M_handle.value);
    }

    template <typename T,
              typename = std::enable_if_t<(std::is_pointer_v<T> || std::is_class_v<T>) &&
                                          !std::is_same_v<std::remove_cvref_t<T>, self_type>>
              >
    constexpr typename std::remove_pointer_t<T>* get () const noexcept
    {
        return direct_cast<std::remove_pointer_t<T>*> (_M_handle.ptr);
    }

    template <class_t T,
              typename = std::enable_if_t<std::is_same_v<std::remove_cvref_t<T>, self_type>>
              >
    constexpr self_type get () const noexcept
    {
        return *this;
    }

    consteval explicit operator safe_bool () const noexcept
    {
        return valid () ? &self_type::_M_handle : nullptr;
    }

    template <integer_t T>
    constexpr self_type& operator = (T const val) noexcept
    {
        static_assert (sizeof (T) <= sizeof (value_type), "T is bigger than the size of uptr!");

        _M_handle.value = static_cast<value_type> (val);
        return *this;
    }

    constexpr self_type& operator = (value_type const val) noexcept
    {
        _M_handle.value = val;
        return *this;
    }

    constexpr self_type& operator = (pointer const ptr) noexcept
    {
        _M_handle.ptr = ptr;
        return *this;
    }

    friend
    constexpr bool operator == (self_type const&, self_type const&) noexcept;

    friend
    constexpr bool operator == (self_type const&, std::nullptr_t) noexcept;

    friend
    constexpr bool operator == (std::nullptr_t, self_type const&) noexcept;

    friend
    constexpr bool operator == (self_type const&, self_type::value_type const) noexcept;

    friend
    constexpr bool operator == (self_type::value_type const, self_type const&) noexcept;

    friend
    constexpr bool operator == (self_type const&, self_type::pointer const) noexcept;

    friend
    constexpr bool operator == (self_type::pointer const, self_type const&) noexcept;

private:
    handle_union _M_handle { };
};

// =========================================================

constexpr bool operator == (resource_handle const& lh, resource_handle const& rh) noexcept
{ return lh._M_handle.value == rh._M_handle.value; }

constexpr bool operator == (resource_handle const& lh, std::nullptr_t) noexcept
{ return lh._M_handle.ptr == nullptr; }

constexpr bool operator == (std::nullptr_t, resource_handle const& rh) noexcept
{ return rh._M_handle.ptr == nullptr; }

constexpr bool operator == (resource_handle const& lh, resource_handle::value_type const val) noexcept
{ return lh._M_handle.value == val; }

constexpr bool operator == (resource_handle::value_type const val, resource_handle const& rh) noexcept
{ return rh._M_handle.value == val; }

constexpr bool operator == (resource_handle const& lh, resource_handle::pointer const ptr) noexcept
{ return lh._M_handle.ptr == ptr; }

constexpr bool operator == (resource_handle::pointer const ptr, resource_handle const& rh) noexcept
{ return rh._M_handle.ptr == ptr; }

constexpr bool operator != (resource_handle const& lh, resource_handle const& rh) noexcept
{ return !(lh == rh); }

constexpr bool operator != (resource_handle const& lh, std::nullptr_t) noexcept
{ return !(lh == nullptr); }

constexpr bool operator != (std::nullptr_t, resource_handle const& rh) noexcept
{ return !(rh == nullptr); }

constexpr bool operator != (resource_handle const& lh, resource_handle::value_type const val) noexcept
{ return !(lh == val); }

constexpr bool operator != (resource_handle::value_type const val, resource_handle const& rh) noexcept
{ return !(rh == val); }

constexpr bool operator != (resource_handle const& lh, resource_handle::pointer const ptr) noexcept
{ return !(lh == ptr); }

constexpr bool operator != (resource_handle::pointer const ptr, resource_handle const& rh) noexcept
{ return !(rh == ptr); }

// =========================================================

struct resource_version
{
    typedef resource_version self_type  ;
    typedef int              value_type ;
    typedef string           string_type;

    enum class version_parts : u8
    {
        only_major = 1,
        to_minor      ,
        to_patch      ,
        all
    };

    constexpr resource_version () noexcept
    : self_type (0, 0)
    { }

    constexpr resource_version (value_type const _major,
                                value_type const _minor,
                                value_type const _patch = value_type (),
                                value_type const _rev   = value_type ()) noexcept
    : major    (_major),
      minor    (_minor),
      patch    (_patch),
      revision (_rev  )
    { }

    inline string_type to_string (version_parts const parts = version_parts::all) const noexcept
    {
        ostringstream stream;

        stream << major;
        if (parts >= version_parts::to_minor) stream << '.' << minor;
        if (parts >= version_parts::to_patch) stream << '.' << patch;
        if (parts == version_parts::all     ) stream << '-' << revision;

        return stream.str ();
    }

    value_type major   ;
    value_type minor   ;
    value_type patch   ;
    value_type revision;
};

// ====================================================

constexpr bool operator < (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return gObj1.major < gObj2.major || gObj1.minor < gObj2.minor; }

constexpr bool operator < (resource_version const& gObj1, resource_version::value_type const uMajor) noexcept
{ return gObj1.major < uMajor; }

constexpr bool operator <= (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return (gObj1.major <= gObj2.major && gObj1.minor <= gObj2.minor); }

constexpr bool operator <= (resource_version const& gObj1, resource_version::value_type const uMajor) noexcept
{ return gObj1.major <= uMajor; }

constexpr bool operator > (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return (gObj1.major > gObj2.major || gObj1.minor > gObj2.minor); }

constexpr bool operator > (resource_version const& gObj1, resource_version::value_type const uMajor) noexcept
{ return gObj1.major > uMajor; }

constexpr bool operator >= (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return (gObj1.major >= gObj2.major && gObj1.minor >= gObj2.minor); }

constexpr bool operator >= (resource_version const& gObj1, resource_version::value_type const uMajor) noexcept
{ return gObj1.major >= uMajor; }

constexpr bool operator == (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return (gObj1.major == gObj2.major && gObj1.minor == gObj2.minor); }

constexpr bool operator != (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

// =========================================================

template <typename C, non_void_t H,
          resource_handle::value_type NULL_VALUE = resource_handle::value_type ()>
class resource : public non_copyable_virtual
{
public:
    typedef resource<C, H, NULL_VALUE> self_type      ;
    typedef std::remove_cvref_t<C>     connection_type;
    typedef std::remove_cvref_t<H>     value_type     ;
    typedef resource_handle            handle_type    ;
    typedef self_type* self_type::*    safe_bool      ;

    inline constexpr static handle_type null_value = NULL_VALUE;

    constexpr resource () noexcept = default;

    inline resource (self_type&&   rc) noexcept
    : _M_connection (rc._M_connection),
      _M_handle     (rc._M_handle    )
    {
        rc._M_connection = connection_type ();
        rc._M_handle     = null_value;
    }

    inline self_type& operator = (self_type&& rc) noexcept
    {
        if (this != &rc)
        {
            _M_connection = rc._M_connection;
            _M_handle     = rc._M_handle    ;
            rc._M_handle  = null_value      ;
        }

        return *this;
    }

    constexpr virtual ~resource () = default;

    constexpr connection_type connection () const noexcept
    { return _M_connection; }

    constexpr bool valid () const noexcept
    { return  _M_handle != null_value && _M_connection; }

    template <typename T = value_type>
    constexpr T handle () const noexcept
    { return _M_handle.get<T> (); }

    inline void set_connection (connection_type const conn) noexcept
    { _M_connection = conn; }

    template <integer_t T>
    inline void set_handle (T const handle) noexcept
    { _M_handle = handle; }

    inline void set_handle (handle_type const handle) noexcept
    { _M_handle = handle; }

    constexpr explicit resource (connection_type const conn, handle_type const handle) noexcept
    : _M_connection (conn  ),
      _M_handle     (handle)
    { }

    template <integer_t T>
    constexpr explicit resource (connection_type const conn, T const handle) noexcept
    : _M_connection (conn  ),
      _M_handle     (handle)
    { }

    constexpr explicit operator safe_bool () const noexcept
    { return valid () ? this : nullptr; }

    template <class C_, class ID_>
    friend constexpr bool operator == (resource<C_, ID_> const&, resource<C_, ID_> const&);

private:
    connection_type _M_connection { };
    handle_type     _M_handle     { };
};

// =========================================================

template <non_void_t H, resource_handle::value_type NULL_VALUE>
class resource <void, H, NULL_VALUE> : public non_copyable_virtual
{
public:
    typedef resource<void, H, NULL_VALUE> self_type  ;
    typedef std::remove_cvref_t<H>        value_type ;
    typedef resource_handle               handle_type;
    typedef self_type* self_type::*       safe_bool  ;

    inline constexpr static handle_type const null_value = NULL_VALUE;

    constexpr resource () noexcept = default;

    constexpr resource (self_type&& rc) noexcept
    : _M_handle (rc._M_handle)
    {
        rc._M_handle = null_value;
    }

    inline self_type& operator = (self_type&& rc) noexcept
    {
        if (this != &rc)
        {
            _M_handle    = rc._M_handle;
            rc._M_handle = null_value  ;
        }

        return *this;
    }

    constexpr virtual ~resource () = default;

    constexpr bool valid () const noexcept
    { return _M_handle != null_value; }

    template <typename T = value_type>
    constexpr T handle () const noexcept
    { return _M_handle.get<T> (); }

    inline void set_handle (handle_type const handle) noexcept
    { _M_handle = handle; }

    template <integer_t T>
    inline void set_handle (T const handle) noexcept
    { _M_handle = handle; }

    constexpr explicit resource (handle_type const handle) noexcept
    : _M_handle (handle)
    { }

    template <integer_t T>
    constexpr explicit resource (T const handle) noexcept
    : _M_handle (handle)
    { }

    constexpr explicit operator safe_bool () const noexcept
    { return valid () ? this : nullptr; }

    template <class ID_>
    friend constexpr bool operator == (resource<void, ID_> const&, resource<void, ID_> const&);

private:
    handle_type _M_handle { };
};

// =========================================================

template <class ID>
constexpr bool operator == (resource<void, ID> const& gObj1, resource<void, ID> const& gObj2)
{ return gObj1._M_handle == gObj2._M_handle; }

template <class ID>
constexpr bool operator != (resource<void, ID> const& gObj1, resource<void, ID> const& gObj2)
{ return !(gObj1 == gObj2); }

template <class C, class ID>
constexpr bool operator == (resource<C, ID> const& gObj1, resource<C, ID> const& gObj2)
{
    return gObj1._M_connection == gObj2._M_connection && gObj1._M_handle == gObj2._M_handle;
}

template <class C, class ID>
constexpr bool operator != (resource<C, ID> const& gObj1, resource<C, ID> const& gObj2)
{ return !(gObj1 == gObj2); }

// =========================================================

} // namespace cppual

// =========================================================

namespace std {

// =========================================================

using cppual::resource_version;

// =========================================================

template <typename CharT, typename Traits>
basic_ostream<CharT, Traits>&
operator << (basic_ostream<CharT, Traits>& stream, resource_version const& u)
{
    return stream << u.major << '.' << u.minor << '.' << u.patch << '-' << u.revision;
}

// =========================================================

template <>
struct hash <resource_version>
{
    typedef resource_version::value_type value_type;

    /// Compute individual hash values for major,
    /// and minor and combine them using XOR
    /// and bit shifting
    size_t operator () (resource_version const& version) const
    {
        return ((hash<value_type>()(version.major)          ^
                (hash<value_type>()(version.minor)    << 1) ^
                (hash<value_type>()(version.patch)    << 1) ^
                (hash<value_type>()(version.revision) << 1)) >> 1);
    }
};

// =========================================================

} // namespace std

#endif // __cplusplus
#endif // CPPUAL_RESOURCE_H_
