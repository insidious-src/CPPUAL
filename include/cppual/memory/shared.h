/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#ifndef CPPUAL_MEMORY_SHARED_H_
#define CPPUAL_MEMORY_SHARED_H_
#ifdef __cplusplus

#include <memory>
#include <cppual/types.h>
#include <cppual/noncopyable.h>

using std::string;

namespace cppual { namespace Memory {

enum class Mode : unsigned char
{
    Create,
    CreateOrOpen,
    Open
};

enum class State : unsigned char
{
    ReadOnly,
    ReadWrite,
    CopyOnWrite,
    ReadPrivate,
    Invalid
};

class SharedObject final : public NonCopyable
{
public:
    typedef std::size_t size_type;
    typedef u16         offset;

    SharedObject (string const&, Mode = Mode::CreateOrOpen, State = State::ReadWrite);
    ~SharedObject () noexcept;
    bool truncate (size_type mem_size) noexcept;

    inline State  state   () const noexcept { return m_eState; }
    inline Mode   mode    () const noexcept { return m_eMode; }
    inline int    id      () const noexcept { return m_nId; }
    inline bool   isValid () const noexcept { return m_nId != -1; }
    inline string name    () const noexcept { return m_gName; }

    inline SharedObject () noexcept
    : m_gName (),
      m_eMode (),
      m_eState (),
      m_nId (-1)
    { }

private:
    string m_gName;
    Mode   m_eMode;
    State  m_eState;
    int    m_nId;
};

class SharedRegion final : public NonCopyable
{
public:
    typedef std::size_t size_type;
    typedef u16         offset;

    SharedRegion (SharedObject&, size_type, bool writable = true);
    ~SharedRegion () noexcept;

    inline bool isWritable () const noexcept
    { return m_bWritable; }

    inline bool isValid () const noexcept
    { return m_pRegion; }

    inline void* address () const noexcept
    { return m_pRegion; }

    inline size_type size () const noexcept
    { return m_uSize; }

    inline SharedObject& object () const noexcept
    { return m_gObject; }

    template <typename T>
    constexpr T* ptr (T* pOffset) const noexcept
    { return pOffset + m_pRegion; }

private:
    SharedObject& m_gObject;
    void*         m_pRegion;
    size_type     m_uSize;
    bool          m_bWritable;
};

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_SHARED_H_
