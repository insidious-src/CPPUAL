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

#ifndef CPPUAL_MEMORY_ALLOCATOR_H_
#define CPPUAL_MEMORY_ALLOCATOR_H_
#ifdef __cplusplus

#include <limits>
#include <memory>
#include <experimental/memory_resource>
#include <cppual/compute/device.h>

namespace std    { using std::experimental::pmr::memory_resource; }

namespace cppual { namespace Memory {

// =========================================================
// Extend std::memory_resource for compute usage
// =========================================================

struct Repository : public std::memory_resource
{
    typedef std::size_t     align_type;
    typedef std::size_t     size_type;
    typedef std::uint8_t*   math_pointer;
    typedef void*           pointer;
    typedef const void*     const_pointer;
    typedef std::ptrdiff_t  difference_type;
    typedef Compute::Device device_type;

    virtual  bool is_thread_safe () const noexcept { return false; }
    virtual  bool is_lock_free   () const noexcept { return false; }
    virtual  bool is_shared      () const noexcept { return false; }

    virtual  device_type& device () const noexcept
    { return device_type::host   (); }

    virtual  Repository& owner   () const noexcept
    { return const_cast<Repository&> (*this); }

    virtual  size_type  max_size () const
    { return std::numeric_limits<size_type>::max (); }

    virtual  size_type capacity  () const
    { return std::numeric_limits<size_type>::max (); }
};

// =========================================================
// Repository Concept
// =========================================================

template <class T>
struct is_repository_helper : public std::conditional
        <
         std::is_base_of<Repository, T>::value,
         std::true_type, std::false_type
         >::type
{ };

template <>
struct is_repository_helper < std::memory_resource > : public std::true_type
{ };

template <>
struct is_repository_helper < Repository >           : public std::true_type
{ };

template <class T>
struct is_repository : public is_repository_helper<T>
{ static_assert (is_repository<T>::value, "invalid Repository object type!"); };

template <class T>
using RepositoryType = typename
std::enable_if<is_repository<T>::value, T>::type;

// =========================================================
// Redefined allocator policy
// =========================================================

template <typename T, class R = void>
class Allocator
{
public:
    typedef T                 value_type;
    typedef T*                pointer;
    typedef T const*          const_pointer;
    typedef T &               reference;
    typedef T const&          const_reference;
    typedef RepositoryType<R> resource_type;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;
    typedef std::false_type   propagate_on_container_copy_assignment;
    typedef std::true_type    propagate_on_container_move_assignment;
    typedef std::true_type    propagate_on_container_swap;

    inline    Allocator (Allocator &&    )             noexcept = default;
    constexpr Allocator (Allocator const&)             noexcept = default;
    inline    Allocator& operator = (Allocator &&    ) noexcept = default;
    inline    Allocator& operator = (Allocator const&) noexcept = default;

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
    static void construct (U* p, Args&&... args)
    { new (p) U (std::forward<Args> (args)...); }

    template <class U>
    static void destroy (U* p)
    { p->~U (); }

    constexpr resource_type* resource () const noexcept
    { return m_pAtor; }

    constexpr Allocator (resource_type& gAtor) noexcept
    : m_pAtor (&gAtor)
    { }

    template <class U>
    constexpr
    explicit
    Allocator (Allocator<U, resource_type> const& gObj) noexcept
    : m_pAtor (gObj.m_pAtor)
    { }

    template <typename, class>
    friend class Allocator;

private:
    resource_type* m_pAtor;
};

// =========================================================
// Allocator Concept
// =========================================================

template <typename T>
struct is_allocator_helper : public std::false_type
{ };

template <typename T>
struct is_allocator_helper < std::allocator<T> > : public std::true_type
{ };

template <typename T, typename U>
struct is_allocator_helper < Allocator <T, U> > : public std::true_type
{ };

template <typename T>
struct is_allocator : public is_allocator_helper<T>
{ };

template <typename T>
using AllocatorType = typename
std::enable_if<is_allocator<T>::value, T>::type;

// =========================================================
// Generic Declarations
// =========================================================

template <typename T>
struct Allocator <T, void> : std::allocator<T>
{ using std::allocator<T>::allocator; };

template <typename T>
using PolymorphicAllocator = Allocator<T, Repository>;

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
operator != (Allocator<T, A> const&,
             RAllocator const&) noexcept
{ return true; }

template <class T, class A, class RAllocator>
constexpr
bool
operator == (Allocator<T, A> const&,
             RAllocator const&) noexcept
{ return false; }

template <class T, class U>
constexpr
bool
operator != (Allocator<T, void> const&,
             std::allocator<U> const&) noexcept
{ return false; }

template <class T, class U>
constexpr
bool
operator == (Allocator<T, void> const&,
             std::allocator<U> const&) noexcept
{ return true; }

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_ALLOCATOR_H_
