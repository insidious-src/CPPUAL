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
#include <cppual/meta.h>

#include <type_traits>

namespace cppual {

enum class ResourceType : std::size_t
{
    Null,
    Device,
    Image,
    Font,
    Macro,
    Texture,
    Shader,
    Program,
    Query,
    Window,
    Surface,
    Buffer,
    Pixmap,
    Colormap,
    Cursor,
    GlyphCursor,
    Context
};

// =========================================================

class Handle final
{
public:
    typedef void* pointer;

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
    constexpr Handle (T handle) noexcept : _M_handle (unsafe_direct_cast<pointer> (handle)) { }

    constexpr operator pointer () const noexcept
    { return _M_handle; }

    inline operator uptr () const noexcept
    { return reinterpret_cast<uptr> (_M_handle); }

    template <typename T,
              typename =
              typename std::enable_if<is_integer<T>::value>::type
              >
    constexpr T get () const noexcept
    {
        return unsafe_direct_cast<T> (_M_handle);
    }

    template <typename T,
              typename =
              typename std::enable_if<(std::is_pointer<T>::value  ||
                                       std::is_class  <T>::value) && !std::is_same<T, Handle>::value
                                      >::type
              >
    constexpr typename std::remove_pointer<T>::type* get () const noexcept
    {
        return static_cast<typename std::remove_pointer<T>::type*> (_M_handle);
    }

    template <typename T,
              typename =
              typename std::enable_if<std::is_same<T, Handle>::value>::type
              >
    constexpr Handle get () const noexcept
    {
        return *this;
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

template <class Controller, class ID>
class Resource
{
public:
    typedef ID         value_type ;
    typedef Controller controller ;
    typedef Handle     handle_type;

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
    typedef ID     value_type ;
    typedef void   controller ;
    typedef Handle handle_type;

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

typedef Handle Connection;

//static_assert (std::is_pod<Handle>::value, "Handle is not POD");

// =========================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_GFX_RESOURCE_H_
