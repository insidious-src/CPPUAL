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

#ifndef CPPUAL_MEMORY_ALLOCATOR_H_
#define CPPUAL_MEMORY_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/types.h>

#include <limits>
#include <memory>

namespace cppual { namespace Memory {

// =========================================================
// std::memory_resource replaced for compute usage
// =========================================================

struct MemoryResource
{
protected:
    static constexpr std::size_t max_align = alignof(std::max_align_t);

public:
    typedef std::size_t     align_type;
    typedef std::size_t     size_type;
    typedef u8*             math_pointer;
    typedef void*           pointer;
    typedef cvoid*          const_pointer;
    typedef std::ptrdiff_t  difference_type;

    virtual ~MemoryResource();

    virtual bool is_thread_safe () const noexcept { return false; }
    virtual bool   is_lock_free () const noexcept { return false; }
    virtual bool      is_shared () const noexcept { return false; }

    virtual MemoryResource& owner () const noexcept
    { return const_cast<MemoryResource&> (*this); }

    virtual size_type max_size () const
    { return std::numeric_limits<size_type>::max (); }

    virtual size_type capacity () const
    { return std::numeric_limits<size_type>::max (); }

    void* allocate(size_type bytes, size_type alignment = max_align)
    { return do_allocate(bytes, alignment); }

    void deallocate(void* p, size_type bytes, size_type alignment = max_align)
    { return do_deallocate(p, bytes, alignment); }

    bool is_equal(MemoryResource const& other) const noexcept
    { return do_is_equal(other); }

protected:
    virtual void* do_allocate(size_type bytes, size_type alignment) = 0;
    virtual void  do_deallocate(void* p, size_type bytes, size_type alignment) = 0;
    virtual bool  do_is_equal(MemoryResource const& other) const noexcept = 0;
};

// =========================================================

inline bool
operator == (MemoryResource const& a, MemoryResource const& b) noexcept
{ return &a == &b || a.is_equal(b); }

inline bool
operator != (MemoryResource const& a, MemoryResource const& b) noexcept
{ return !(a == b); }

// =========================================================

MemoryResource* new_delete_resource () noexcept;
MemoryResource* null_memory_resource() noexcept;
MemoryResource* get_default_resource() noexcept;
void            set_default_resource(MemoryResource&) noexcept;

// =========================================================
// MemoryResource Concept
// =========================================================

template <class T>
struct is_memory_resource_helper : public std::conditional
        <
         std::is_base_of<MemoryResource, T>::value,
         std::true_type, std::false_type
         >::type
{ };

template <>
struct is_memory_resource_helper < MemoryResource > : public std::true_type
{ };

template <class T>
struct is_memory_resource : public is_memory_resource_helper<T>
{ };

template <class T>
using MemoryResourceType = typename
std::enable_if<is_memory_resource<T>::value, T>::type;

// =========================================================
// Redefined memory allocator
// =========================================================

template <typename T, typename R = MemoryResource>
class Allocator
{
public:
    typedef T               value_type;
    typedef T *             pointer;
    typedef T const*        const_pointer;
    typedef T &             reference;
    typedef T const&        const_reference;
    typedef R               resource_type;
    typedef R*              resource_pointer;
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;
    typedef std::false_type propagate_on_container_copy_assignment;
    typedef std::true_type  propagate_on_container_move_assignment;
    typedef std::true_type  propagate_on_container_swap;

    static_assert (is_memory_resource<resource_type>::value, "invalid memory_resource object type!");

    inline    Allocator (Allocator &&    )             noexcept = default;
    constexpr Allocator (Allocator const&)             noexcept = default;
    inline    Allocator& operator = (Allocator &&    ) noexcept = default;
    inline    Allocator& operator = (Allocator const&) noexcept = default;

    inline ~Allocator() { }

    template <class U>
    struct rebind { typedef Allocator<U, resource_type> other; };

    void deallocate (pointer p, size_type n)
    { resource ()->deallocate (p, n * sizeof (T)); }

    pointer allocate (size_type n, cvoid* = nullptr)
    { return static_cast<pointer> (resource ()->allocate (sizeof (T) * n, alignof (T))); }

    size_type max_size () const noexcept
    { return resource ()->max_size () / sizeof (T); }

    static pointer address (reference x) noexcept
    { return std::addressof (x); }

    static const_pointer address (const_reference x) noexcept
    { return std::addressof (x); }

    template<class U, typename... Args>
    static void construct (U* p, Args... args)
    { new (p) U (std::forward<Args> (args)...); }

    template <class U>
    static void destroy (U* p)
    { p->~U (); }

    Allocator select_on_container_copy_construction() const
    { return Allocator (); }

    constexpr resource_pointer resource () const noexcept
    { return m_pRc; }

    constexpr explicit Allocator (resource_type& rc) noexcept
    : m_pRc (&rc)
    { }

    constexpr Allocator () noexcept
    : m_pRc (get_default_resource())
    { }

    template <class U>
    constexpr
    explicit
    Allocator (Allocator<U, resource_type> const& ator) noexcept
    : m_pRc (ator.m_pRc)
    { }

    template <typename, typename>
    friend class Allocator;

private:
    resource_pointer m_pRc;
};

template <class T1, class T2>
bool operator == (Allocator<T1> const& lhs, Allocator<T2> const& rhs) noexcept
{ return *lhs.resource() == *rhs.resource(); }

template <class T1, class T2>
bool operator != (Allocator<T1> const& lhs, Allocator<T2> const& rhs) noexcept
{ return *lhs.resource() != *rhs.resource(); }

// =========================================================
// Allocator Concept
// =========================================================

template <typename>
struct is_allocator_helper : public std::false_type
{ };

template <typename T>
struct is_allocator_helper < std::allocator<T> > : public std::true_type
{ };

template <typename T, typename U>
struct is_allocator_helper < Allocator<T, U> > : public std::true_type
{ };

template <typename T>
struct is_allocator : public is_allocator_helper<T>
{ static_assert (is_allocator<T>::value, "invalid allocator object type!"); };

template <typename T>
using AllocatorType = typename
std::enable_if<is_allocator<T>::value, T>::type;

// =========================================================
// Generic Declarations
// =========================================================

template <typename T>
using PolymorphicAllocator = Allocator<T, MemoryResource>;

// =========================================================
// Allocator Comparisons
// =========================================================

template <class T, class U, class A>
inline
bool
operator == (Allocator<T, A> const& x,
             Allocator<U, A> const& y) noexcept
{ return x.resource () == y.resource (); }

template <class T, class U, class A>
inline
bool
operator != (Allocator<T, A> const& x,
             Allocator<U, A> const& y) noexcept
{ return !(x == y); }

template <class T, class A, class RAllocator>
constexpr
bool
operator != (Allocator<T, A> const&, RAllocator const&) noexcept
{ return true; }

template <class T, class A, class RAllocator>
constexpr
bool
operator == (Allocator<T, A> const&, RAllocator const&) noexcept
{ return false; }

template <class T, class U>
constexpr
bool
operator != (Allocator<T, void> const&, std::allocator<U> const&) noexcept
{ return false; }

template <class T, class U>
constexpr
bool
operator == (Allocator<T, void> const&, std::allocator<U> const&) noexcept
{ return true; }

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_ALLOCATOR_H_
