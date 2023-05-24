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

#ifndef CPPUAL_MEMORY_ALLOCATOR_H_
#define CPPUAL_MEMORY_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/noncopyable.h>

#include <thread>
#include <memory>
#include <limits>
#include <algorithm>

namespace cppual { namespace memory {

// =========================================================

/// std::pmr::memory_resource replacement for compute usage
class SHARED_API memory_resource : public non_copyable_movable_virtual
{
public:
    typedef memory_resource  base_type             ;
    typedef base_type*       base_pointer          ;
    typedef base_type&       base_reference        ;
    typedef base_type const& base_const_reference  ;
    typedef base_type*&      base_pointer_reference;
    typedef base_type**      base_double_pointer   ;
    typedef std::size_t      align_type            ;
    typedef std::size_t      size_type             ;
    typedef byte*            math_pointer          ;
    typedef void*            pointer               ;
    typedef cvoid*           const_pointer         ;
    typedef std::ptrdiff_t   difference_type       ;

    /// maximum alignment depending on memory width
    static constexpr const align_type max_align  = alignof (std::max_align_t);

    /// maximum align ajustment for memory allocation
    /// using smart pointers (ex. std::shared_ptr & std::unique_ptr)
    static constexpr const align_type max_adjust = alignof (std::max_align_t) * 2;

    constexpr memory_resource () = default;
    virtual  ~memory_resource ();

    /// is thread safe (ex. using mutex)
    inline virtual bool is_thread_safe () const noexcept { return false; }

    /// is inter-process shared memory
    inline virtual bool is_shared () const noexcept { return false; }

    /// upstream memory_resource owne
    inline virtual base_reference owner () const noexcept
    { return const_cast<base_reference> (*this); }

    /// maximum bytes that can be allocated
    inline virtual size_type max_size () const
    { return std::numeric_limits<size_type>::max () - max_align; }

    /// memory capacity in bytes
    inline virtual size_type capacity () const
    { return std::numeric_limits<size_type>::max () - max_align; }

    inline
    void* allocate (size_type bytes, size_type alignment = max_align)
    { return do_allocate (bytes, alignment); }

    inline
    void* reallocate (void* p, size_type old_size, size_type new_size, size_type alignment = max_align)
    { return do_reallocate (p, old_size, new_size, alignment); }

    inline
    void deallocate (void* p, size_type bytes = size_type(), size_type alignment = max_align)
    { do_deallocate (p, bytes, alignment); }

    inline
    bool is_equal (base_const_reference other) const
    { return do_is_equal (other); }

protected:
    virtual void* do_allocate (size_type bytes, align_type alignment) = 0;
    virtual void  do_deallocate (void* p, size_type bytes, align_type alignment) = 0;
    virtual bool  do_is_equal (base_const_reference other) const = 0;

    virtual void* do_reallocate (void* p, size_type old_size, size_type new_size, align_type alignment)
    {
        if (p && old_size && old_size == new_size) return p;

        auto const old_p = static_cast<math_pointer> (p);
        auto const new_p = static_cast<math_pointer>
                (allocate (new_size > old_size ? new_size : old_size, alignment));

        if (p)
        {
            std::copy (old_p, old_p + (old_size <= new_size ? old_size : new_size), new_p);
            deallocate (p, old_size, alignment);
        }

        return new_p;
    }
};

// =========================================================

inline bool operator == (memory_resource const& a, memory_resource const& b) noexcept
{ return a.is_equal(b); }

inline bool
operator != (memory_resource const& a, memory_resource const& b) noexcept
{ return !(a == b); }

// =========================================================

/// statically constructed memory_resource using new & delete operators
memory_resource* SHARED_API new_delete_resource () noexcept;

/// statically constructed memory_resource for testing purposes.
/// on allocation throws std::bad_alloc on deallocation does nothing
memory_resource* SHARED_API null_resource () noexcept;

/// statically constructed memory_resource using malloc & free functions
memory_resource* SHARED_API malloc_resource () noexcept;

/// get the default memory_resource
memory_resource* SHARED_API get_default_resource () noexcept;

/// get the default memory_resource for the current thread
memory_resource* SHARED_API get_default_thread_resource () noexcept;

/// set the default memory_resource
void SHARED_API set_default_resource (memory_resource& resource) noexcept;

/// set the default memory_resource for the current thread
void SHARED_API set_default_thread_resource (memory_resource& resource) noexcept;

/// get main thread id
std::thread::id SHARED_API main_thread_id () noexcept;

// =========================================================
// memory_resource Concept
// =========================================================

template <class T>
struct is_memory_resource_helper : public std::is_base_of<memory_resource, T>
{ };

template <>
struct is_memory_resource_helper <memory_resource> : public std::true_type
{ };

template <class T>
struct is_memory_resource : public is_memory_resource_helper<T>
{ static_assert (is_memory_resource<T>::value, "T is not derived from memory_resource!"); };

/// memory_resource concept type
template <class T>
using ResourceType = typename
std::enable_if<is_memory_resource<T>::value, T>::type;

// =========================================================

/// redefined polymorphic memory allocator
template <typename T>
class SHARED_API allocator
{
public:
    typedef T               value_type                            ;
    typedef T *             pointer                               ;
    typedef T const*        const_pointer                         ;
    typedef T &             reference                             ;
    typedef T const&        const_reference                       ;
    typedef memory_resource resource_type                         ;
    typedef resource_type*  resource_pointer                      ;
    typedef resource_type&  resource_reference                    ;
    typedef std::size_t     size_type                             ;
    typedef std::ptrdiff_t  difference_type                       ;
    typedef std::true_type  propagate_on_container_copy_assignment;
    typedef std::true_type  propagate_on_container_move_assignment;
    typedef std::true_type  propagate_on_container_swap           ;

    template <class U>
    struct rebind { typedef allocator<U> other; };

    inline void deallocate (pointer p, size_type n)
    { resource ()->deallocate (p, n * sizeof (value_type), alignof (value_type)); }

    inline
    pointer allocate (size_type n, cvoid* = nullptr)
    {
        return static_cast<pointer> (resource ()->allocate (sizeof (value_type) * n,
                                                            alignof(value_type)));
    }

    inline
    pointer reallocate (pointer p, size_type old_n, size_type n)
    {
        return static_cast<pointer> (resource ()->reallocate (p,
                                                              sizeof (value_type) * old_n,
                                                              sizeof (value_type) * n,
                                                              alignof(value_type)));
    }

    inline size_type max_size () const noexcept
    { return resource ()->max_size () / sizeof (value_type); }

    inline static pointer address (reference x) noexcept
    { return std::addressof (x); }

    inline static const_pointer address (const_reference x) noexcept
    { return std::addressof (x); }

    template <typename... Args>
    inline static void construct (pointer p, Args&&... args)
    { ::new (p) value_type (std::forward<Args> (args)...); }

    inline static void destroy (pointer p)
    { p->~value_type (); }

    constexpr allocator select_on_container_copy_construction () const
    { return allocator (*this); }

    constexpr resource_pointer resource () const noexcept
    { return _M_pRc; }

    constexpr
    explicit
    allocator (resource_reference rc)
    : _M_pRc  (&rc)
    { }

    inline
    allocator () noexcept
    : _M_pRc  (std::this_thread::get_id () == main_thread_id () ?
                   get_default_resource ()  : get_default_thread_resource ())
    { }

    template <typename U>
    constexpr
    explicit
    allocator (allocator<U> const& ator) noexcept
    : _M_pRc  (ator._M_pRc)
    { }

    template <typename U>
    inline
    explicit
    allocator (allocator<U>&& ator) noexcept
    : _M_pRc  (ator._M_pRc)
    {
        ator._M_pRc = nullptr;
    }

    template <typename U>
    inline
    allocator& operator = (allocator<U> const& ator) noexcept
    {
        _M_pRc = ator._M_pRc;
        return *this;
    }

    template <typename U>
    inline
    allocator& operator = (allocator<U>&& ator) noexcept
    {
        _M_pRc      = ator._M_pRc;
        ator._M_pRc = nullptr    ;

        return *this;
    }

    template <typename U>
    inline
    explicit
    allocator (std::allocator<U> const&) noexcept
    : _M_pRc  (new_delete_resource ())
    { }

    template <typename U>
    inline
    explicit
    allocator (std::allocator<U>&&) noexcept
    : _M_pRc  (new_delete_resource ())
    { }

    template <typename U>
    inline
    allocator& operator = (std::allocator<U> const&) noexcept
    {
        _M_pRc = new_delete_resource ();
        return *this;
    }

    template <typename U>
    inline
    allocator& operator = (std::allocator<U>&&) noexcept
    {
        _M_pRc = new_delete_resource ();
        return *this;
    }

    template <typename>
    friend class allocator;

private:
    resource_pointer _M_pRc;
};

// =========================================================
// Allocator Comparisons
// =========================================================

template <typename T>
inline bool operator == (allocator<T> const& lhs, allocator<T> const& rhs) noexcept
{
    return lhs.resource () == rhs.resource () ||
          (lhs.resource () && rhs.resource () && *lhs.resource () == *rhs.resource ());
}

template <typename T>
inline bool operator != (allocator<T> const& lhs, allocator<T> const& rhs) noexcept
{ return !(lhs == rhs); }

template <typename T1, typename T2>
inline bool operator == (allocator<T1> const& lhs, allocator<T2> const& rhs) noexcept
{
    return lhs.resource () == rhs.resource () ||
          (lhs.resource () && rhs.resource () && *lhs.resource () == *rhs.resource ());
}

template <typename T1, typename T2>
inline bool operator != (allocator<T1> const& lhs, allocator<T2> const& rhs) noexcept
{ return !(lhs == rhs); }

template <typename T>
constexpr bool operator == (allocator<T> const& lhs, std::allocator<T> const&) noexcept
{
    return lhs.resource () == new_delete_resource ();
}

template <typename T>
constexpr bool operator != (allocator<T> const& lhs, std::allocator<T> const& rhs) noexcept
{ return !(lhs == rhs); }

template <typename T>
constexpr bool operator == (std::allocator<T> const&, allocator<T> const& rhs) noexcept
{
    return rhs.resource () == new_delete_resource ();
}

template <typename T>
constexpr bool operator != (std::allocator<T> const& lhs, allocator<T> const& rhs) noexcept
{ return !(lhs == rhs); }

template <typename T1, typename T2>
constexpr bool operator == (allocator<T1> const& lhs, std::allocator<T2> const&) noexcept
{
    return lhs.resource () == new_delete_resource ();
}

template <typename T1, typename T2>
constexpr bool operator != (allocator<T1> const& lhs, std::allocator<T2> const& rhs) noexcept
{ return !(lhs == rhs); }

template <typename T1, typename T2>
constexpr bool operator == (std::allocator<T1> const&, allocator<T2> const& rhs) noexcept
{
    return rhs.resource () == new_delete_resource ();
}

template <typename T1, typename T2>
constexpr bool operator != (std::allocator<T1> const& lhs, allocator<T2> const& rhs) noexcept
{ return !(lhs == rhs); }

// =========================================================
// Allocator Concept
// =========================================================

template <typename>
struct is_allocator_helper : public std::false_type
{ };

template <typename T>
struct is_allocator_helper < allocator<T> > : public std::true_type
{ };

/// is allocator helper
template <typename T>
struct is_allocator : public is_allocator_helper<T>
{ static_assert (is_allocator<T>::value, "invalid allocator object type!"); };

/// is allocator helper -> value
template <typename T>
inline constexpr auto const is_allocator_v = is_allocator<T>::value;

/// polymorphic allocator concept type
template <typename T>
using AllocatorType = typename
std::enable_if<is_allocator<T>::value, T>::type;

// =========================================================

template <typename T, typename... Args>
inline std::shared_ptr<T> allocate_shared (memory_resource* rc, Args&&... args)
{
    return std::allocate_shared<T> (rc == nullptr ? allocator<T>() : allocator<T>(*rc),
                                    std::forward<Args> (args)...);
}

template <typename T, typename U, typename... Args>
inline std::shared_ptr<U> allocate_shared (memory_resource* rc, Args&&... args)
{
    return std::static_pointer_cast<U>
            (std::allocate_shared<T> (rc == nullptr ? allocator<T>() : allocator<T>(*rc),
                                      std::forward<Args> (args)...));
}

template <typename T, typename... Args>
inline std::shared_ptr<T> allocate_shared (allocator<T> const& ator, Args&&... args)
{
    return std::allocate_shared<T> (ator, std::forward<Args> (args)...);
}

template <typename T, typename U, typename... Args>
inline std::shared_ptr<U> allocate_shared (allocator<T> const& ator, Args&&... args)
{
    return std::static_pointer_cast<U>
            (std::allocate_shared<T> (ator, std::forward<Args> (args)...));
}

// =========================================================

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_ALLOCATOR_H_
