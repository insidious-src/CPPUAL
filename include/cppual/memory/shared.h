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

#ifndef CPPUAL_MEMORY_SHARED_H_
#define CPPUAL_MEMORY_SHARED_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/resource.h>
#include <cppual/containers.h>
#include <cppual/noncopyable.h>

//#include <memory>

namespace cppual::memory {

enum class mode_type : u8
{
    create,
    create_or_open,
    open
};

enum class state_type : u8
{
    read_only,
    read_write,
    copy_on_write,
    read_private,
    invalid
};

class SHARED_API shared_object : public non_copyable
{
public:
    typedef shared_object self_type  ;
    typedef std::size_t   size_type  ;
    typedef u16           offset_type;
    typedef string        string_type;

    shared_object  (string_type const& name,
                    mode_type          mode  = mode_type::create_or_open,
                    state_type         state = state_type::read_write);

    ~shared_object () noexcept;
    bool truncate  (size_type mem_size) noexcept;

    constexpr state_type  state () const noexcept { return _M_eState  ; }
    constexpr mode_type   mode  () const noexcept { return _M_eMode   ; }
    constexpr int         id    () const noexcept { return _M_nId     ; }
    constexpr bool        valid () const noexcept { return _M_nId > -1; }
    constexpr string_type name  () const noexcept { return _M_gName   ; }

    constexpr shared_object () noexcept
    : _M_gName  (  ),
      _M_eMode  (  ),
      _M_eState (  ),
      _M_nId    (-1)
    { }

private:
    string     _M_gName ;
    mode_type  _M_eMode ;
    state_type _M_eState;
    int        _M_nId   ;
};

class SHARED_API shared_memory : public non_copyable
{
public:
    typedef shared_memory   self_type  ;
    typedef shared_object   object_type;
    typedef std::size_t     size_type  ;
    typedef u16             offset_type;
    typedef void*           pointer    ;
    typedef resource_handle handle_type;

    shared_memory  (object_type& obj, size_type size, bool writable = true);
    ~shared_memory () noexcept;

    consteval bool writable () const noexcept
    { return _M_bWritable; }

    consteval bool valid () const noexcept
    { return _M_pRegion; }

    constexpr pointer address () const noexcept
    { return _M_pRegion.get<pointer> (); }

    consteval size_type size () const noexcept
    { return _M_uSize; }

    constexpr object_type const& object () const noexcept
    { return *_M_gObject; }

    constexpr object_type& object () noexcept
    { return *_M_gObject; }

    template <typename T>
    constexpr T* ptr (T* pOffset) const noexcept
    { return _M_pRegion.get<T*> () + pOffset; }

private:
    shared_memory () = delete;

private:
    object_type* _M_gObject   { };
    handle_type  _M_pRegion   { };
    size_type    _M_uSize     { };
    bool         _M_bWritable { };
};

} // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_SHARED_H_
