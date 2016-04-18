/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

namespace cppual {

//template <typename> class Disposable { };

//enum class ResourceType : unsigned char
//{
//    Null = 0,
//    Device,
//    Image,
//    Font,
//    Macro,
//    Texture,
//    Shader,
//    Program,
//    Query,
//    Window,
//    Surface,
//    Buffer,
//    Pixmap,
//    Colormap,
//    Cursor,
//    GlyphCursor,
//    Context
//};

// =========================================================

class Handle
{
public:
    typedef void* pointer;

    inline    Handle () noexcept = default;
    constexpr Handle (pointer handle) noexcept : m_handle (handle) { }
    constexpr Handle (std::nullptr_t) noexcept : m_handle ()       { }

    constexpr operator pointer () const noexcept
    { return m_handle; }

    template <typename T>
    constexpr typename std::remove_pointer<T>::type* get () const noexcept
    { return static_cast<typename std::remove_pointer<T>::type*> (m_handle); }

    friend
    constexpr bool operator == (Handle const&, Handle const&) noexcept;

    friend
    constexpr bool operator == (Handle const&, std::nullptr_t) noexcept;

private:
    pointer m_handle;
};

constexpr bool operator == (Handle const& conn1, Handle const& conn2) noexcept
{ return conn1.m_handle == conn2.m_handle; }

constexpr bool operator == (Handle const& conn1, std::nullptr_t) noexcept
{ return conn1.m_handle == nullptr; }

constexpr bool operator != (Handle const& conn1, Handle const& conn2) noexcept
{ return !(conn1 == conn2); }

constexpr bool operator != (Handle const& conn1, std::nullptr_t) noexcept
{ return !(conn1 == nullptr); }

// =========================================================

template <class Controller, class ID>
class Resource
{
public:
    typedef ID         value_type;
    typedef Controller controller;

    Resource (Resource const&) = delete;
    Resource& operator = (Resource const&) = delete;
    ~Resource () { }

    controller   connection () const noexcept { return m_pCon; }
    value_type   id         () const noexcept { return m_id; }
    bool         valid      () const noexcept { return m_id; }

    Resource () noexcept
    : m_pCon (),
      m_id   ()
    { }

    Resource (Controller pCon, value_type id) noexcept
    : m_pCon (pCon),
      m_id   (id)
    { }

    template <class Controller_, class ID_>
    friend constexpr bool operator == (Resource<Controller_, ID_> const&,
                                       Resource<Controller_, ID_> const&);

private:
    controller m_pCon;
    value_type m_id;
};

// =========================================================

template <class ID>
class Resource < void, ID >
{
public:
    typedef ID   value_type;
    typedef void controller;

    Resource (Resource const&) = delete;
    Resource& operator = (Resource const&) = delete;
    ~Resource () { }

    value_type   id    () const noexcept { return m_id; }
    bool         valid () const noexcept { return m_id; }

    Resource () noexcept
    : m_id   ()
    { }

    Resource (value_type id) noexcept
    : m_id   (id)
    { }

    template <class ID_>
    friend constexpr bool operator == (Resource<void, ID_> const&,
                                       Resource<void, ID_> const&);

private:
    value_type m_id;
};

// =========================================================

template <class ID, class DisposableType>
constexpr bool operator == (Resource<void, ID> const& gObj1,
                            Resource<void, ID> const& gObj2)
{ return gObj1.m_id == gObj2.m_id and gObj1.m_eResType == gObj2.m_eResType; }

template <class ID, class DisposableType>
constexpr bool operator != (Resource<void, ID> const& gObj1,
                            Resource<void, ID> const& gObj2)
{ return !(gObj1 == gObj2); }

template <class Controller, class ID>
constexpr bool operator == (Resource<Controller, ID> const& gObj1,
                            Resource<Controller, ID> const& gObj2)
{
    return gObj1.m_pCon == gObj2.m_pCon and gObj1.m_id == gObj2.m_id and
            gObj1.m_eResType == gObj2.m_eResType;
}

template <class Controller, class ID>
constexpr bool operator != (Resource<Controller, ID> const& gObj1,
                            Resource<Controller, ID> const& gObj2)
{ return !(gObj1 == gObj2); }

} // cppual

#endif // __cplusplus
#endif // CPPUAL_GFX_RESOURCE_H_
