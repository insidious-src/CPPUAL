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

#include <cppual/casts>
#include <cppual/types>
#include <cppual/concepts>
#include <cppual/bitflags>
#include <cppual/noncopyable>

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
    pipeline        = 1 << 17, //! TODO: rename to more generic name
    queue           = 1 << 18,
    event           = 1 << 19,
    state           = 1 << 20,
    sampler         = 1 << 21,
    render_pass     = 1 << 22, //! TODO: rename to more generic name
    descriptor_pool = 1 << 23, //! TODO: rename to more generic name
    socket          = 1 << 24,
    custom          = 1 << 30
};

// =========================================================

typedef bitset<resource_type> resource_types;

// =========================================================

template <typename T>
concept resource_handle_c = std::is_void_v<T>    ||
                            std::is_pointer_v<T> ||
                            is_integer_v<T>      ||
                            std::is_class_v<T>   ||
                            std::is_same_v<T, resource_connection>;

template <typename T>
concept resource_handle_h = std::is_pointer_v<T> ||
                            is_integer_v<T>      ||
                            std::is_same_v<T, resource_handle>;

// =========================================================

class SHARED_API resource_handle
{
public:
    union   handle_value                           ;
    typedef resource_handle           self_type    ;
    typedef uptr                      value_type   ;
    typedef value_type const          const_value  ;
    typedef void *                    pointer      ;
    typedef cvoid*                    const_pointer;
    typedef handle_value self_type::* safe_bool    ;

    inline constexpr static const_value npos = static_cast<value_type> (-1);

    typedef union handle_value
    {
        value_type value { };
        pointer    ptr      ;

        constexpr handle_value () noexcept = default;
        constexpr handle_value (value_type _handle) noexcept : value (_handle) { }
        constexpr handle_value (pointer    _handle) noexcept : ptr   (_handle) { }
        constexpr handle_value (std::nullptr_t    ) noexcept : ptr   ()        { }

        constexpr handle_value& operator = (value_type handle) noexcept
        { value = handle; return *this; }

        constexpr handle_value& operator = (pointer handle) noexcept
        { ptr = handle; return *this; }

        constexpr handle_value& operator = (std::nullptr_t) noexcept
        { ptr = nullptr; return *this; }
    }
    const const_handle;

    constexpr resource_handle () noexcept = default;
    constexpr resource_handle (value_type handle) noexcept : _M_handle (handle) { }
    constexpr resource_handle (pointer    handle) noexcept : _M_handle (handle) { }
    constexpr resource_handle (std::nullptr_t   ) noexcept : _M_handle ()       { }

    constexpr resource_handle (self_type &&)           noexcept = default;
    constexpr resource_handle (self_type const&)       noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    template <integer T>
    constexpr resource_handle (T const _handle) noexcept
    : _M_handle (is_unsigned_integer_v<T> ? _handle : static_cast<value_type> (_handle))
    {
        static_assert (sizeof (T) <= sizeof (value_type), "T is bigger than the size of uptr!");
    }

    constexpr operator pointer    () const noexcept { return _M_handle.ptr  ; }
    constexpr operator value_type () const noexcept { return _M_handle.value; }

    constexpr bool valid () const noexcept
    {
        return (_M_handle.value > 0 && _M_handle.value != npos) || _M_handle.ptr != nullptr;
    }

    template <integer T = value_type>
    constexpr T get () const noexcept
    {
        static_assert (sizeof (T) <= sizeof (value_type), "T is bigger than the size of uptr!");
        return static_cast<T> (_M_handle.value);
    }
    template <ptr T>
    constexpr T get () const noexcept
    {
        return direct_cast<T> (_M_handle.ptr);
    }

    template <struct_or_class T, std::enable_if_t<!std::is_same_v<remove_cref_t<T>, self_type>, void>>
    constexpr T* get () const noexcept
    {
        return direct_cast<T*> (_M_handle.ptr);
    }

    template <struct_or_class T, std::enable_if_t<std::is_same_v<remove_cref_t<T>, self_type>, void>>
    constexpr self_type get () const noexcept
    {
        return *this;
    }

    consteval operator safe_bool () const noexcept
    {
        return valid () ? &self_type::_M_handle : nullptr;
    }

    constexpr self_type& operator = (long64 val) noexcept
    {
        _M_handle.value = static_cast<value_type> (val);
        return *this;
    }

    constexpr self_type& operator = (value_type val) noexcept
    {
        _M_handle.value = val;
        return *this;
    }

    constexpr self_type& operator = (pointer ptr) noexcept
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
    constexpr bool operator == (self_type const&, const_value) noexcept;

    friend
    constexpr bool operator == (const_value, self_type const&) noexcept;

    friend
    constexpr bool operator == (self_type const&, const_pointer) noexcept;

    friend
    constexpr bool operator == (const_pointer, self_type const&) noexcept;

private:
    handle_value _M_handle { };
};

// =========================================================

constexpr bool operator == (resource_handle const& lh, resource_handle const& rh) noexcept
{ return lh._M_handle.value == rh._M_handle.value; }

constexpr bool operator == (resource_handle const& lh, std::nullptr_t) noexcept
{ return lh._M_handle.ptr == nullptr; }

constexpr bool operator == (std::nullptr_t, resource_handle const& rh) noexcept
{ return rh._M_handle.ptr == nullptr; }

constexpr bool operator == (resource_handle const& lh, resource_handle::const_value val) noexcept
{ return lh._M_handle.value == val; }

constexpr bool operator == (resource_handle::const_value val, resource_handle const& rh) noexcept
{ return rh._M_handle.value == val; }

constexpr bool operator == (resource_handle const& lh, resource_handle::const_pointer ptr) noexcept
{ return lh._M_handle.ptr == ptr; }

constexpr bool operator == (resource_handle::const_pointer ptr, resource_handle const& rh) noexcept
{ return rh._M_handle.ptr == ptr; }

constexpr bool operator != (resource_handle const& lh, resource_handle const& rh) noexcept
{ return !(lh == rh); }

constexpr bool operator != (resource_handle const& lh, std::nullptr_t) noexcept
{ return !(lh == nullptr); }

constexpr bool operator != (std::nullptr_t, resource_handle const& rh) noexcept
{ return !(rh == nullptr); }

constexpr bool operator != (resource_handle const& lh, resource_handle::const_value val) noexcept
{ return !(lh == val); }

constexpr bool operator != (resource_handle::const_value val, resource_handle const& rh) noexcept
{ return !(rh == val); }

constexpr bool operator != (resource_handle const& lh, resource_handle::const_pointer ptr) noexcept
{ return !(lh == ptr); }

constexpr bool operator != (resource_handle::const_pointer ptr, resource_handle const& rh) noexcept
{ return !(rh == ptr); }

// =========================================================

template <resource_handle_c C = resource_connection,
          resource_handle_h H = resource_handle    ,
          resource_handle::value_type NULL_V = 0>
class SHARED_API resource : public non_copyable_virtual
{
public:
    typedef resource<C, H, NULL_V>  self_type       ;
    typedef remove_cref_t<C>        connection_type ;
    typedef connection_type const   const_connection;
    typedef remove_cref_t<H>        value_type      ;
    typedef resource_handle         handle_type     ;
    typedef handle_type const       const_handle    ;
    typedef self_type* self_type::* safe_bool       ;

    inline constexpr static const handle_type::value_type npos = NULL_V;

    constexpr resource () noexcept = default;

    constexpr resource (self_type&& rc) noexcept
    : _M_connection (rc._M_connection),
      _M_handle     (rc._M_handle    )
    {
        rc._M_connection = connection_type ();
        rc._M_handle     = npos              ;
    }

    constexpr self_type& operator = (self_type&& rc) noexcept
    {
        if (this != &rc)
        {
            _M_connection = rc._M_connection;
            _M_handle     = rc._M_handle    ;
            rc._M_handle  = npos            ;
        }

        return *this;
    }

    constexpr resource (const_connection conn, const_handle handle) noexcept
    : _M_connection (conn  ),
      _M_handle     (handle)
    { }

    constexpr bool valid () const noexcept
    { return _M_handle.valid (); }

    constexpr connection_type connection () const noexcept
    { return _M_connection; }

    constexpr handle_type handle () const noexcept
    { return _M_handle; }

    template <integer_or_ptr T>
    constexpr T handle () const noexcept
    { return _M_handle.get<T> (); }

    constexpr void set_connection (const_connection conn) noexcept
    { _M_connection = conn; }

    constexpr void set_handle (const_handle handle) noexcept
    { _M_handle = handle; }

    constexpr explicit operator safe_bool () const noexcept
    { return valid () ? this : nullptr; }

    template <typename C_, typename H_, handle_type::value_type NULL_V_>
    friend constexpr bool operator == (resource<C_, H_, NULL_V_> const&,
                                       resource<C_, H_, NULL_V_> const&);

private:
    connection_type _M_connection {      };
    handle_type     _M_handle     { npos };
};

// =========================================================

template <resource_handle_h H, resource_handle::value_type NULL_V>
class SHARED_API resource <void, H, NULL_V> : public non_copyable_virtual
{
public:
    typedef resource<void, H, NULL_V> self_type   ;
    typedef remove_cref_t<H>          value_type  ;
    typedef value_type const          const_value ;
    typedef resource_handle           handle_type ;
    typedef handle_type const         const_handle;
    typedef self_type* self_type::*   safe_bool   ;

    inline constexpr static handle_type::const_value npos = NULL_V;

    constexpr resource () noexcept = default;

    constexpr explicit resource (const_handle handle) noexcept
    : _M_handle (handle)
    { }

    constexpr resource (self_type&& rc) noexcept
    : _M_handle (rc._M_handle)
    {
        rc._M_handle = npos;
    }

    constexpr self_type& operator = (self_type&& rc) noexcept
    {
        if (this != &rc)
        {
            _M_handle    = rc._M_handle;
            rc._M_handle = npos        ;
        }

        return *this;
    }

    constexpr bool valid () const noexcept
    { return _M_handle.valid (); }

    constexpr handle_type handle () const noexcept
    { return _M_handle; }

    template <integer_or_ptr T>
    constexpr T handle () const noexcept
    { return _M_handle.get<T> (); }

    constexpr void set_handle (const_handle handle) noexcept
    { _M_handle = handle; }

    consteval explicit operator safe_bool () const noexcept
    { return valid () ? this : nullptr; }

    template <class ID_, handle_type::value_type NULL_V_>
    friend constexpr bool operator == (resource<void, ID_, NULL_V_> const&,
                                       resource<void, ID_, NULL_V_> const&);

private:
    handle_type _M_handle { npos };
};

// =========================================================

template <class ID, resource_handle::value_type NULL_V>
constexpr bool operator == (resource<void, ID, NULL_V> const& lh, resource<void, ID, NULL_V> const& rh)
{ return lh._M_handle == rh._M_handle; }

template <class ID, resource_handle::value_type NULL_V>
constexpr bool operator != (resource<void, ID, NULL_V> const& lh, resource<void, ID, NULL_V> const& rh)
{ return !(lh == rh); }

template <class C, class ID, resource_handle::value_type NULL_V>
constexpr bool operator  == (resource<C, ID, NULL_V> const& lh, resource<C, ID, NULL_V> const& rh)
{
    return lh._M_connection == rh._M_connection && lh._M_handle == rh._M_handle;
}

template <class C, class ID, resource_handle::value_type NULL_V>
constexpr bool operator  != (resource<C, ID, NULL_V> const& lh, resource<C, ID, NULL_V> const& rh)
{ return !(lh == rh); }

// =========================================================

struct SHARED_API resource_version
{
    typedef resource_version self_type  ;
    typedef int              value_type ;
    typedef value_type const const_value;
    typedef string           string_type;

    typedef enum class version_parts : u8
    {
        only_major = 1,
        to_minor      ,
        to_patch      ,
        all
    }
    const const_parts;

    consteval resource_version () noexcept = default;

    constexpr resource_version (const_value _major,
                                const_value _minor,
                                const_value _patch = value_type (),
                                const_value _rev   = value_type ()) noexcept
    : major    (_major),
      minor    (_minor),
      patch    (_patch),
      revision (_rev  )
    { }

    inline string_type to_string (const_parts parts = version_parts::all) const noexcept
    {
        ostringstream stream;

        stream << major;

        if (parts >= version_parts::to_minor) stream << '.' << minor;
        if (parts >= version_parts::to_patch) stream << '.' << patch;
        if (parts == version_parts::all     ) stream << '-' << revision;

        return stream.str ();
    }

    value_type major    { };
    value_type minor    { };
    value_type patch    { };
    value_type revision { };
};

// ====================================================

constexpr bool operator < (resource_version const& lh, resource_version const& rh) noexcept
{ return lh.major < rh.major || lh.minor < rh.minor; }

constexpr bool operator < (resource_version const& lh, resource_version::const_value uMajor) noexcept
{ return lh.major < uMajor; }

constexpr bool operator <= (resource_version const& lh, resource_version const& rh) noexcept
{ return (lh.major <= rh.major && lh.minor <= rh.minor); }

constexpr bool operator <= (resource_version const& lh, resource_version::const_value uMajor) noexcept
{ return lh.major <= uMajor; }

constexpr bool operator > (resource_version const& lh, resource_version const& rh) noexcept
{ return (lh.major > rh.major || lh.minor > rh.minor); }

constexpr bool operator > (resource_version const& lh, resource_version::const_value uMajor) noexcept
{ return lh.major > uMajor; }

constexpr bool operator >= (resource_version const& lh, resource_version const& rh) noexcept
{ return (lh.major >= rh.major && lh.minor >= rh.minor); }

constexpr bool operator >= (resource_version const& lh, resource_version::const_value uMajor) noexcept
{ return lh.major >= uMajor; }

constexpr bool operator == (resource_version const& lh, resource_version const& rh) noexcept
{ return (lh.major == rh.major && lh.minor == rh.minor); }

constexpr bool operator != (resource_version const& lh, resource_version const& rh) noexcept
{ return !(lh == rh); }

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
    typedef std::size_t                  size_type ;

    /// Compute individual hash values for major,
    /// and minor and combine them using XOR
    /// and bit shifting
    size_type operator () (resource_version const& version) const
    {
        return ((hash<value_type>()(version.major)          ^
                (hash<value_type>()(version.minor)    << 1) ^
                (hash<value_type>()(version.patch)    << 1) ^
                (hash<value_type>()(version.revision) << 1)) >> 1);
    }
};

// =========================================================

} // namespace std

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_RESOURCE_H_
