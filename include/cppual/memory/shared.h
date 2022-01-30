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

#ifndef CPPUAL_MEMORY_SHARED_H_
#define CPPUAL_MEMORY_SHARED_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/string.h>
#include <cppual/noncopyable.h>

#include <memory>

namespace cppual { namespace memory {

enum class Mode : byte
{
    Create,
    CreateOrOpen,
    Open
};

enum class State : byte
{
    ReadOnly,
    ReadWrite,
    CopyOnWrite,
    ReadPrivate,
    Invalid
};

class shared_object final : public non_copyable
{
public:
    typedef std::size_t size_type;
    typedef u16         offset;

    shared_object  (string const&, Mode = Mode::CreateOrOpen, State = State::ReadWrite);
    ~shared_object () noexcept;
    bool truncate (size_type mem_size) noexcept;

    inline State  state () const noexcept { return _M_eState   ; }
    inline Mode   mode  () const noexcept { return _M_eMode    ; }
    inline int    id    () const noexcept { return _M_nId      ; }
    inline bool   valid () const noexcept { return _M_nId != -1; }
    inline string name  () const noexcept { return _M_gName    ; }

    inline shared_object () noexcept
    : _M_gName  (),
      _M_eMode  (),
      _M_eState (),
      _M_nId    (-1)
    { }

private:
    string _M_gName ;
    Mode   _M_eMode ;
    State  _M_eState;
    int    _M_nId   ;
};

class shared_memory final : public non_copyable
{
public:
    typedef std::size_t size_type;
    typedef u16         offset   ;

    shared_memory (shared_object&, size_type, bool writable = true);
    ~shared_memory () noexcept;

    inline bool writable () const noexcept
    { return _M_bWritable; }

    inline bool valid () const noexcept
    { return _M_pRegion; }

    inline void* address () const noexcept
    { return _M_pRegion; }

    inline size_type size () const noexcept
    { return _M_uSize; }

    inline shared_object& object () const noexcept
    { return _M_gObject; }

    template <typename T>
    constexpr T* ptr (T* pOffset) const noexcept
    { return _M_pRegion + pOffset; }

private:
    shared_object& _M_gObject  ;
    void*          _M_pRegion  ;
    size_type      _M_uSize    ;
    bool           _M_bWritable;
};

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_SHARED_H_
