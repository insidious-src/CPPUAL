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

#ifndef CPPUAL_RESOURCE_H_
#define CPPUAL_RESOURCE_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/flags.h>
#include <cppual/meta.h>

#include <type_traits>
#include <functional>

namespace cppual {

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
    shader          = 1 <<  7,
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
    typedef uptr  value_type;
    typedef void* pointer   ;

    constexpr resource_handle () noexcept = default;
    constexpr resource_handle (value_type _handle) noexcept : _M_handle (_handle) { }
    constexpr resource_handle (pointer    _handle) noexcept : _M_handle (direct_cast<value_type> (_handle)) { }
    constexpr resource_handle (std::nullptr_t    ) noexcept : _M_handle ()  { }

    inline    resource_handle (resource_handle&&)                  noexcept = default;
    constexpr resource_handle (resource_handle const&)             noexcept = default;
    inline    resource_handle& operator = (resource_handle&&)      noexcept = default;
    inline    resource_handle& operator = (resource_handle const&) noexcept = default;

    template <typename T,
              typename = typename std::enable_if_t<is_integer_v<T>>
              >
    constexpr resource_handle (T _handle) noexcept
    : _M_handle (static_cast<value_type> (_handle))
    {
        static_assert (sizeof (T) <= sizeof (pointer), "T is bigger than the size of a pointer!");
    }

    constexpr operator pointer () const noexcept
    { return direct_cast<pointer> (_M_handle); }

    constexpr operator value_type () const noexcept
    { return _M_handle; }

    template <typename T,
              typename =
              typename std::enable_if_t<is_integer_v<T>>
              >
    constexpr T get () const noexcept
    {
        static_assert (sizeof (T) <= sizeof (pointer), "T is bigger than the size of a pointer!");
        return static_cast<T> (_M_handle);
    }

    template <typename T,
              typename =
              typename std::enable_if_t<(std::is_pointer_v<T>  ||
                                         std::is_class_v  <T>) &&
                                        !std::is_same_v<typename std::remove_cv<T>::type, resource_handle>>
              >
    constexpr typename std::remove_pointer<T>::type* get () const noexcept
    {
        return direct_cast<typename std::remove_pointer<T>::type*> (_M_handle);
    }

    template <typename T,
              typename =
              typename std::enable_if_t<std::is_same<typename std::remove_cv<T>::type, resource_handle>::value>
              >
    constexpr resource_handle get () const noexcept
    {
        return *this;
    }

    constexpr explicit operator bool () const noexcept
    {
        return _M_handle;
    }

    template <typename T,
              typename =
              typename std::enable_if_t<is_integer_v<T>>
              >
    inline resource_handle& operator = (T val) noexcept
    {
        static_assert (sizeof (T) <= sizeof (pointer), "T is bigger than the size of a pointer!");

        _M_handle = static_cast<value_type> (val);
        return *this;
    }

    inline resource_handle& operator = (value_type val) noexcept
    {
        _M_handle = val;
        return *this;
    }

    inline resource_handle& operator = (pointer ptr) noexcept
    {
        _M_handle = direct_cast<value_type> (ptr);
        return *this;
    }

    friend
    constexpr bool operator == (resource_handle const&, resource_handle const&) noexcept;

    friend
    constexpr bool operator == (resource_handle const&, std::nullptr_t) noexcept;

    friend
    constexpr bool operator == (std::nullptr_t, resource_handle const&) noexcept;

    friend
    constexpr bool operator == (resource_handle const&, resource_handle::value_type) noexcept;

    friend
    constexpr bool operator == (resource_handle::value_type, resource_handle const&) noexcept;

private:
    value_type _M_handle { };
};

// =========================================================

constexpr bool operator == (resource_handle const& lh, resource_handle const& rh) noexcept
{ return lh._M_handle == rh._M_handle; }

constexpr bool operator == (resource_handle const& lh, std::nullptr_t) noexcept
{ return lh._M_handle == resource_handle::value_type (); }

constexpr bool operator == (std::nullptr_t, resource_handle const& rh) noexcept
{ return rh._M_handle == resource_handle::value_type (); }

constexpr bool operator == (resource_handle const& lh, resource_handle::value_type val) noexcept
{ return lh._M_handle == val; }

constexpr bool operator == (resource_handle::value_type val, resource_handle const& rh) noexcept
{ return rh._M_handle == val; }

constexpr bool operator != (resource_handle const& lh, resource_handle const& rh) noexcept
{ return !(lh == rh); }

constexpr bool operator != (resource_handle const& lh, std::nullptr_t) noexcept
{ return !(lh == nullptr); }

constexpr bool operator != (std::nullptr_t, resource_handle const& rh) noexcept
{ return !(rh == nullptr); }

constexpr bool operator != (resource_handle const& lh, resource_handle::value_type val) noexcept
{ return !(lh == val); }

constexpr bool operator != (resource_handle::value_type val, resource_handle const& rh) noexcept
{ return !(rh == val); }

// =========================================================

struct resource_version
{
    typedef int value_type;

    constexpr resource_version () noexcept
    : resource_version (0, 0)
    { }

    constexpr resource_version (value_type _major, value_type _minor) noexcept
    : major (_major), minor (_minor)
    { }

    value_type major;
    value_type minor;
};

// ====================================================

constexpr bool operator  < (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return (gObj1.major < gObj2.major || gObj1.minor < gObj2.minor); }

constexpr bool operator  < (resource_version const& gObj1, resource_version::value_type uMajor) noexcept
{ return gObj1.major  < uMajor; }

constexpr bool operator  <= (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return (gObj1.major <= gObj2.major && gObj1.minor <= gObj2.minor); }

constexpr bool operator  <= (resource_version const& gObj1, resource_version::value_type uMajor) noexcept
{ return gObj1.major  <= uMajor; }

constexpr bool operator  > (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return (gObj1.major > gObj2.major || gObj1.minor > gObj2.minor); }

constexpr bool operator  > (resource_version const& gObj1, resource_version::value_type uMajor) noexcept
{ return gObj1.major  > uMajor; }

constexpr bool operator  >= (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return (gObj1.major >= gObj2.major && gObj1.minor >= gObj2.minor); }

constexpr bool operator  >= (resource_version const& gObj1, resource_version::value_type uMajor) noexcept
{ return gObj1.major  >= uMajor; }

constexpr bool operator  == (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return (gObj1.major == gObj2.major && gObj1.minor == gObj2.minor); }

constexpr bool operator  != (resource_version const& gObj1, resource_version const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

// =========================================================

template <class C,
          class H,
          resource_handle::value_type NULL_VALUE = resource_handle::value_type ()>
class resource
{
public:
    typedef H                     value_type     ;
    typedef C                     connection_type;
    typedef resource_handle       handle_type    ;
    typedef resource* resource::* safe_bool      ;

    constexpr static handle_type null_value = NULL_VALUE;

    constexpr resource () noexcept = default;


    inline resource (resource&&    rc) noexcept
    : _M_connection (rc._M_connection),
      _M_handle     (rc._M_handle    )
    {
        rc._M_connection = connection_type ();
        rc._M_handle     = null_value;
    }

    inline resource& operator = (resource&& rc) noexcept
    {
        if (this != &rc)
        {
            _M_connection = rc._M_connection;
            _M_handle     = rc._M_handle    ;
            rc._M_handle  = null_value      ;
        }

        return *this;
    }

    resource (resource const&) = delete;
    resource& operator = (resource const&) = delete;

    virtual ~resource () { }

    constexpr connection_type connection () const noexcept
    { return _M_connection; }

    constexpr bool valid () const noexcept
    { return _M_connection && _M_handle != null_value; }

    template <typename T = value_type>
    constexpr T handle () const noexcept
    { return _M_handle.get<T> (); }

    void set_connection (connection_type conn) noexcept
    { _M_connection = conn; }

    void set_handle (handle_type handle) noexcept
    { _M_handle = handle; }

    constexpr resource (connection_type conn, value_type id) noexcept
    : _M_connection (conn),
      _M_handle     (id  )
    { }

    constexpr explicit operator safe_bool () const noexcept
    {
        return valid () ? this : nullptr;
    }

    template <class C_, class ID_>
    friend constexpr bool operator == (resource<C_, ID_> const&, resource<C_, ID_> const&);

private:
    connection_type _M_connection { };
    handle_type     _M_handle     { };
};

// =========================================================

template <class H, resource_handle::value_type NULL_VALUE>
class resource <void, H, NULL_VALUE>
{
public:
    typedef H                     value_type ;
    typedef resource_handle       handle_type;
    typedef resource* resource::* safe_bool  ;

    constexpr static handle_type null_value = NULL_VALUE;

    constexpr resource () noexcept = default;

    inline resource (resource&& rc) noexcept
    : _M_handle     (rc._M_handle )
    {
        rc._M_handle = null_value;
    }

    inline resource& operator = (resource&& rc) noexcept
    {
        if (this != &rc)
        {
            _M_handle    = rc._M_handle;
            rc._M_handle = null_value  ;
        }

        return *this;
    }
    resource (resource const&) = delete;
    resource& operator = (resource const&) = delete;

    virtual ~resource () { }

    constexpr bool valid () const noexcept
    { return _M_handle != null_value; }

    template <typename T = value_type>
    constexpr T handle () const noexcept
    { return _M_handle.get<T> (); }

    void set_handle (handle_type handle) noexcept
    { _M_handle = handle; }

    constexpr resource (value_type id) noexcept
    : _M_handle (id)
    { }

    constexpr explicit operator safe_bool () const noexcept
    {
        return valid () ? this : nullptr;
    }

    template <class ID_>
    friend constexpr bool operator == (resource<void, ID_> const&,
                                       resource<void, ID_> const&);

private:
    handle_type _M_handle { };
};

// =========================================================

template <class ID>
constexpr bool operator == (resource<void, ID> const& gObj1,
                            resource<void, ID> const& gObj2)
{ return gObj1._M_handle == gObj2._M_handle; }

template <class ID>
constexpr bool operator != (resource<void, ID> const& gObj1,
                            resource<void, ID> const& gObj2)
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

template <typename CharT, typename Traits>
basic_ostream<CharT, Traits>&
operator << (std::basic_ostream<CharT, Traits>& stream, cppual::resource_version const& u)
{ return stream << u.major << '.' << u.minor; }

template <>
struct hash<cppual::resource_version>
{
    typedef cppual::resource_version::value_type value_type;

    /// Compute individual hash values for major,
    /// and minor and combine them using XOR
    /// and bit shifting
    size_t operator () (cppual::resource_version const& version) const
    {
        return ((hash<value_type>()(version.major) ^ (hash<value_type>()(version.minor) << 1)) >> 1);
    }
};

// =========================================================

} // namespace std

#endif // __cplusplus
#endif // CPPUAL_GFX_RESOURCE_H_
