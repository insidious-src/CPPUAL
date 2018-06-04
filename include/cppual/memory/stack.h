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

#ifndef CPPUAL_MEMORY_STACKED_ALLOCATOR_H_
#define CPPUAL_MEMORY_STACKED_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/noncopyable.h>
#include <cppual/memory/allocator.h>

using std::string;

namespace cppual { namespace Memory {

class StackedPool final : Repository
{
public:
    StackedPool  (size_type capacity);
    StackedPool  (Repository& allocator, size_type capacity);
    //StackedPool  (string& shared_name, size_type size);
    ~StackedPool ();

    void   clear  ()       noexcept {        m_pCurMarker = m_pBegin; }
    cvoid* marker () const noexcept { return m_pCurMarker;            }

    bool is_equal (Repository const& gObj) const noexcept
    { return &gObj == &m_gOwner; }

    size_type max_size () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (m_pEnd) -
                                       static_cast<math_pointer> (m_pCurMarker));
    }

    size_type capacity () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (m_pEnd) -
                                       static_cast<math_pointer> (m_pBegin));
    }

    Repository& owner     () const noexcept { return m_gOwner; }
    bool        is_shared () const noexcept { return m_bIsMemShared; }

private:
    Repository&   m_gOwner;
    pointer       m_pCurMarker;
    pointer const m_pBegin;
    pointer const m_pEnd;
    cbool         m_bIsMemShared;

    void* do_allocate   (size_type capacity, align_type align) noexcept;
    void  do_deallocate (void* p, size_type capacity, align_type align);

    bool  do_is_equal   (std::memory_resource const& gObj)    const noexcept
    { return &gObj == &m_gOwner; }
};

// =========================================================

template <class T>
using StackedPolicy = Allocator <T, StackedPool>;

// =========================================================

class DStackedPool final : public Repository, NonCopyable
{
public:
    DStackedPool  (size_type capacity, size_type marker_hint);
    DStackedPool  (Repository& allocator, size_type capacity, size_type marker_hint);
    //DStackedPool  (string& shared, size_type size, size_type marker_hint);
    ~DStackedPool ();



    size_type hint () const noexcept { return m_uHint; }
    void      setHint (size_type uHint) noexcept { m_uHint = uHint; }
    cvoid*    bottomMarker () const noexcept { return m_pBottomMarker; }
    cvoid*    topMarker () const noexcept { return m_pTopMarker; }

    bool is_shared () const noexcept
    { return m_bIsMemShared;  }

    Repository& owner () const noexcept
    { return m_gOwner; }

    size_type max_size () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (m_pBottomMarker) -
                                       static_cast<math_pointer> (m_pTopMarker));
    }

    size_type capacity () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (m_pEnd) -
                                       static_cast<math_pointer> (m_pBegin));
    }

    void clear () noexcept
    {
        m_pTopMarker    = m_pBegin;
        m_pBottomMarker = m_pEnd;
    }

private:
    Repository&   m_gOwner;
    pointer       m_pTopMarker;
    pointer       m_pBottomMarker;
    pointer const m_pBegin;
    size_type     m_uHint;
    pointer const m_pEnd;
    cbool         m_bIsMemShared;

    void* do_allocate   (size_type capacity, align_type align) noexcept;
    void  do_deallocate (void* p, size_type capacity, align_type align);

    bool  do_is_equal   (std::memory_resource const& gObj)    const noexcept
    { return &gObj == &m_gOwner; }
};

// =========================================================

template <class T>
using DStackedPolicy = Allocator <T, DStackedPool>;

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_ALLOCATOR_H_
