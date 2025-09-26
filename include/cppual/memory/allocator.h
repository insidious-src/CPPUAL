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

#ifndef CPPUAL_MEMORY_ALLOCATOR_H_
#define CPPUAL_MEMORY_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/decl>
#include <cppual/types>
#include <cppual/concepts>
#include <cppual/noncopyable>
#include <cppual/meta_functional>

#include <scoped_allocator>
#include <memory_resource>
#include <algorithm>
#include <thread>
#include <memory>
#include <limits>

// =========================================================

namespace cppual::compute { class device; } // namespace compute

// =========================================================

namespace cppual::memory {

// =========================================================

//! memory_resource_adaptor is a merger class for any memory_resource
//! implementation that combines 2 or more resource instances (objects) into one to
//! extend the capacity of the allocated memory (allocating more fixed memory).
class memory_resource_adaptor;

template <non_void T>
class allocator;

// =========================================================

using std::scoped_allocator_adaptor;

// =========================================================

template <non_void T, class_t A = allocator<T>>
struct allocator_traits : public std::allocator_traits<A>
{ typedef A allocator_type; };

// =========================================================

//! std::pmr::memory_resource replacement for compute usage
class SHARED_API memory_resource : public std::pmr::memory_resource
{
public:
    typedef memory_resource           self_type             ;
    typedef self_type                 base_type             ;
    typedef self_type*                base_pointer          ;
    typedef self_type&                base_reference        ;
    typedef self_type const&          base_const_reference  ;
    typedef self_type*&               base_pointer_reference;
    typedef self_type**               base_double_pointer   ;
    typedef std::pmr::memory_resource abs_base_type         ;
    typedef std::size_t               size_type             ;
    typedef size_type const           const_size            ;
    typedef size_type                 align_type            ;
    typedef const_size                const_align           ;
    typedef byte *                    math_pointer          ;
    typedef cbyte*                    math_const_pointer    ;
    typedef void *                    pointer               ;
    typedef cvoid*                    const_pointer         ;
    typedef std::ptrdiff_t            difference_type       ;
    typedef compute::device           device_type           ;
    typedef compute::device*          device_pointer        ;
    typedef compute::device&          device_reference      ;

    /// maximum alignment depending on memory width
    inline constexpr static const align_type max_align  = alignof (std::max_align_t);

    /// maximum align ajustment for memory allocation
    inline constexpr static const align_type max_adjust = alignof (std::max_align_t) * 2;

    constexpr memory_resource (device_reference dev = host_device ()) noexcept
    : _M_pDevice (&dev)
    { }

    inline bool is_host_device () const noexcept
    { return _M_pDevice == &host_device (); }

    constexpr device_reference device () const noexcept
    { return *_M_pDevice; }

    /// is thread safe (ex. using mutex or atomic variables)
    constexpr virtual bool is_thread_safe () const noexcept
    { return false; }

    /// is either atomic for true or is using mutex for false.
    /// it can still be lock free if it has NO inter-thread synchronization
    constexpr virtual bool is_lock_free () const noexcept
    { return true; }

    /// is inter-process shared memory
    constexpr virtual bool is_shared () const noexcept
    { return false; }

    /// upstream memory_resource owner
    constexpr virtual base_reference owner () noexcept
    { return *this; }

    constexpr virtual base_const_reference owner () const noexcept
    { return *this; }

    /// upstream memory_resource owner
    constexpr base_reference upstream_resource () noexcept
    { return owner (); }

    /// upstream memory_resource owner
    constexpr base_const_reference upstream_resource () const noexcept
    { return owner (); }

    /// maximum bytes that can be allocated from the memory_resource
    constexpr virtual size_type max_size () const
    { return std::numeric_limits<size_type>::max () - max_align; }

    /// maximum bytes that can be allocated from the memory_resource
    constexpr size_type max_allocate () const
    { return max_size (); }

    /// memory capacity in bytes
    constexpr virtual size_type capacity () const
    { return std::numeric_limits<size_type>::max () - max_align; }

    constexpr void* reallocate (pointer p, size_type old_size, size_type new_size, align_type align = max_align)
    { return do_reallocate (p, old_size, new_size, align); }

protected:
    constexpr virtual void* do_reallocate (pointer p, size_type old_size, size_type new_size, align_type align)
    {
        if (p && old_size && old_size == new_size) return p;

        auto const old_p = static_cast<math_pointer> (p);
        auto const new_p = static_cast<math_pointer>
                (allocate (new_size > old_size ? new_size : old_size, align));

        if (p)
        {
            std::copy (old_p, old_p + (old_size <= new_size ? old_size : new_size), new_p);
            deallocate (p, old_size, align);
        }

        return new_p;
    }

private:
    static device_reference host_device () noexcept;

private:
    device_pointer _M_pDevice;
};

// =========================================================

constexpr bool operator == (memory_resource const& a, memory_resource const& b) noexcept
{ return &a == &b || a.is_equal (b); }

constexpr bool operator != (memory_resource const& a, memory_resource const& b) noexcept
{ return !(a == b); }

// =========================================================

/// statically constructed memory_resource using new & delete operators
memory_resource& SHARED_FN_API new_delete_resource () noexcept;

/// statically constructed memory_resource for testing purposes.
/// on allocation throws std::bad_alloc on deallocation does nothing
memory_resource& SHARED_FN_API null_resource () noexcept;

/// statically constructed memory_resource using malloc & free functions
memory_resource& SHARED_FN_API malloc_resource () noexcept;

/// get the default memory_resource
memory_resource& SHARED_FN_API get_default_resource () noexcept;

/// get the default memory_resource for the current thread
memory_resource& SHARED_FN_API get_default_thread_resource () noexcept;

/// set the default memory_resource
void SHARED_FN_API set_default_resource (memory_resource& resource) noexcept;

/// set the default memory_resource for the current thread
void SHARED_FN_API set_default_thread_resource (memory_resource& resource) noexcept;

/// get main thread id
std::thread::id SHARED_FN_API main_thread_id () noexcept;


/// =========================================================
/// memory_resource concept
/// =========================================================

template <class_t T>
struct is_memory_resource :
public std::conditional_t<std::is_base_of_v<memory_resource, T> ||
                          std::is_base_of_v<std::pmr::memory_resource, T>,
                          std::true_type,
                          std::false_type
                          >
{ static_assert (is_memory_resource<T>::value, "T is NOT derived from memory_resource!"); };

template <>
struct is_memory_resource <memory_resource> : public std::true_type
{ };

template <>
struct is_memory_resource <std::pmr::memory_resource> : public std::true_type
{ };

/// is_memory_resource -> value
template <class_t T>
inline constexpr cbool is_memory_resource_v = is_memory_resource<T>::value;

/// memory_resource concept type
template <class_t T>
using MemoryResourceType = std::enable_if_t<is_memory_resource_v<T>, T>;

/// memory_resource concept
template <class T>
concept memory_resource_t = is_memory_resource_v<T>;

// =========================================================

//! redefined polymorphic memory allocator
template <non_void T>
class SHARED_API allocator
{
public:
    typedef allocator<T>                    self_type                             ;
    typedef T                               value_type                            ;
    typedef T *                             pointer                               ;
    typedef T const*                        const_pointer                         ;
    typedef T &                             reference                             ;
    typedef T const&                        const_reference                       ;
    typedef memory_resource                 resource_type                         ;
    typedef resource_type*                  resource_pointer                      ;
    typedef resource_type&                  resource_reference                    ;
    typedef resource_type const&            resource_const_reference              ;
    typedef std::size_t                     size_type                             ;
    typedef size_type const                 const_size                            ;
    typedef size_type                       align_type                            ;
    typedef const_size                      const_align                           ;
    typedef std::ptrdiff_t                  difference_type                       ;
    typedef std::true_type                  propagate_on_container_copy_assignment;
    typedef std::true_type                  propagate_on_container_move_assignment;
    typedef std::true_type                  propagate_on_container_swap           ;
    typedef resource_type::device_type      device_type                           ;
    typedef resource_type::device_pointer   device_pointer                        ;
    typedef resource_type::device_reference device_reference                      ;

    template <non_void U>
    struct rebind { typedef allocator<U> other; };

    template <non_void U>
    using rebind_t = rebind<U>::other;

    template <non_void U>
    using rebind_alloc = rebind_t<U>;

    template <non_void U>
    using self_type_t = allocator<U>;

    constexpr device_reference device () const noexcept
    { return resource ()->device (); }

    constexpr bool is_host_device () const noexcept
    { return resource ()->is_host_device (); }

    constexpr bool is_thread_safe () const noexcept
    { return resource ()->is_thread_safe (); }

    constexpr bool is_lock_free () const noexcept
    { return resource ()->is_lock_free (); }

    constexpr bool is_shared () const noexcept
    { return resource ()->is_shared (); }

    //! upstream memory_resource owner
    constexpr resource_reference resource_owner () noexcept
    { return  resource ()->owner (); }

    //! upstream const memory_resource owner
    constexpr resource_const_reference resource_owner () const noexcept
    { return  resource ()->owner (); }

    constexpr resource_pointer resource () const noexcept
    {
        static thread_local resource_pointer rc = std::this_thread::get_id () == main_thread_id () ?
                                                  &get_default_resource        () :
                                                  &get_default_thread_resource () ;

        return !_M_pRc ? _M_pRc = rc : _M_pRc;
    }

    constexpr pointer allocate (size_type n = 1)
    {
        return static_cast<pointer> (resource ()->allocate (sizeof  (value_type) * n,
                                                            alignof (value_type)));
    }

    constexpr pointer reallocate (pointer p, size_type old_n, size_type n)
    {
        return static_cast<pointer> (resource ()->reallocate (p,
                                                              sizeof (value_type) * old_n,
                                                              sizeof (value_type) * n,
                                                              alignof(value_type)));
    }

    constexpr void deallocate (pointer p, size_type n = 1)
    { resource ()->deallocate (p, n * sizeof (value_type), alignof (value_type)); }

    constexpr size_type max_size () const noexcept
    { return resource ()->max_size () / sizeof (value_type); }

    constexpr static pointer address (reference o) noexcept
    { return std::addressof (o); }

    constexpr static const_pointer address (const_reference o) noexcept
    { return std::addressof (o); }

    template <typename... Args>
    constexpr static void construct (pointer p, Args&&... args)
    { ::new (p) value_type (std::forward<Args> (args)...); }

    constexpr static void destroy (pointer p)
    { p->~value_type (); }

    constexpr void* allocate_bytes (size_type bytes, align_type align = alignof (std::max_align_t))
    { return resource ()->allocate (bytes, align); }

    constexpr void deallocate_bytes (void* p, size_type bytes, align_type align = alignof (std::max_align_t))
    { return resource ()->deallocate (p, bytes, align); }

    template <non_void U>
    constexpr U* allocate_object (size_type n = 1)
    { return static_cast<U*> (resource ()->allocate (n * sizeof (U), alignof (U))); }

    template <non_void U>
    constexpr void deallocate_object (U* p, size_type n = 1)
    { resource ()->deallocate (p, n * sizeof (U), alignof (U)); }

    template <non_void U, typename... Args>
    constexpr U* new_object (Args&&... args)
    {
        U* p = allocate_object<U> ();

        try { construct (p, std::forward<Args> (args)...); }
        catch (...)
        {
            deallocate_object (p);
            throw std::logic_error ("constructor of type U failed!");
        }

        return p;
    }

    template <non_void U>
    constexpr void delete_object (U* p)
    {
        destroy (p);
        deallocate_object (p);
    }

    constexpr self_type select_on_container_copy_construction () const noexcept
    { return  self_type (*this); }

    constexpr explicit allocator (resource_reference rc)
    : _M_pRc (&rc)
    { }

    constexpr allocator () noexcept
    : _M_pRc ()
    { }

    template <non_void U>
    constexpr allocator (self_type_t<U> const& ator) noexcept
    : _M_pRc (ator._M_pRc)
    { }

    template <non_void U>
    constexpr allocator (self_type_t<U>&& ator) noexcept
    : _M_pRc (ator._M_pRc)
    { ator._M_pRc = &get_default_resource (); }

    template <non_void U>
    constexpr explicit allocator (std::allocator<U> const&) noexcept
    : _M_pRc (&new_delete_resource ())
    { }

    template <non_void U>
    constexpr explicit allocator (std::allocator<U>&&) noexcept
    : _M_pRc (&new_delete_resource ())
    { }

    template <non_void U>
    constexpr self_type& operator = (self_type_t<U> const& ator) noexcept
    {
        _M_pRc = ator._M_pRc;
        return *this;
    }

    template <non_void U>
    constexpr self_type& operator = (self_type_t<U>&& ator) noexcept
    {
        if (this != &ator)
        {
            _M_pRc      = ator._M_pRc;
            ator._M_pRc = &get_default_resource ();
        }

        return *this;
    }

    template <non_void U>
    constexpr self_type& operator = (resource_reference& res) noexcept
    {
        _M_pRc = &res;
        return *this;
    }

    template <non_void U>
    constexpr self_type& operator = (std::allocator<U> const&) noexcept
    {
        _M_pRc = &new_delete_resource ();
        return *this;
    }

    template <non_void U>
    constexpr self_type& operator = (std::allocator<U>&&) noexcept
    {
        _M_pRc = &new_delete_resource ();
        return *this;
    }

    template <non_void>
    friend class allocator;

    template <non_void U>
    friend constexpr void swap (self_type_t<U>&, self_type_t<U>&) noexcept;

    template <non_void U1, non_void U2>
    friend constexpr void swap (self_type_t<U1>&, self_type_t<U2>&) noexcept;

private:
    resource_pointer mutable _M_pRc { };
};

/// =========================================================
/// allocator comparisons
/// =========================================================

template <non_void T>
constexpr bool operator == (allocator<T> const& lh, allocator<T> const& rh) noexcept
{
    return lh.resource () == rh.resource () ||
          (lh.resource () && rh.resource () && lh.resource ()->is_equal (*rh.resource ()));
}

template <non_void T>
constexpr bool operator != (allocator<T> const& lh, allocator<T> const& rh) noexcept
{ return !(lh == rh); }

template <non_void T1, non_void T2>
constexpr bool operator == (allocator<T1> const& lh, allocator<T2> const& rh) noexcept
{
    return lh.resource () == rh.resource () ||
          (lh.resource () && rh.resource () && lh.resource ()->is_equal (*rh.resource ()));
}

template <non_void T1, non_void T2>
inline bool operator != (allocator<T1> const& lh, allocator<T2> const& rh) noexcept
{ return !(lh == rh); }

template <non_void T>
constexpr bool operator == (allocator<T> const& lh, std::allocator<T> const&) noexcept
{ return lh.resource () == &new_delete_resource (); }

template <non_void T>
constexpr bool operator != (allocator<T> const& lh, std::allocator<T> const& rh) noexcept
{ return !(lh == rh); }

template <non_void T>
constexpr bool operator == (std::allocator<T> const&, allocator<T> const& rh) noexcept
{ return rh.resource () == &new_delete_resource (); }

template <non_void T>
constexpr bool operator != (std::allocator<T> const& lh, allocator<T> const& rh) noexcept
{ return !(lh == rh); }

template <non_void T1, non_void T2>
constexpr bool operator == (allocator<T1> const& lh, std::allocator<T2> const&) noexcept
{ return lh.resource () == &new_delete_resource (); }

template <non_void T1, non_void T2>
constexpr bool operator != (allocator<T1> const& lh, std::allocator<T2> const& rh) noexcept
{ return !(lh == rh); }

template <non_void T1, non_void T2>
constexpr bool operator == (std::allocator<T1> const&, allocator<T2> const& rh) noexcept
{ return rh.resource () == &new_delete_resource (); }

template <non_void T1, non_void T2>
constexpr bool operator != (std::allocator<T1> const& lh, allocator<T2> const& rh) noexcept
{ return !(lh == rh); }

/// =========================================================
/// allocator swap
/// =========================================================

template <non_void U>
constexpr void swap (allocator<U>& lh, allocator<U>& rh) noexcept
{ std::swap (lh._M_pRc, rh._M_pRc); }

template <non_void U1, non_void U2>
constexpr void swap (allocator<U1>& lh, allocator<U2>& rh) noexcept
{ std::swap (lh._M_pRc, rh._M_pRc); }

/// =========================================================
/// allocator concept
/// =========================================================

/// is allocator primary template declaration
template <class_t A>
struct is_allocator_helper : public std::false_type
{ typedef A type; };

template <non_void T>
struct is_allocator_helper <allocator<T>> : public std::true_type
{ typedef allocator<T> type; };

template <non_void T>
struct is_allocator_helper <std::allocator<T>> : public std::true_type
{ typedef std::allocator<T> type; };

template <non_void T>
struct is_allocator_helper <std::pmr::polymorphic_allocator<T>> : public std::true_type
{ typedef std::pmr::polymorphic_allocator<T> type; };

template <class_t A>
struct is_allocator : public is_allocator_helper<A>
{ static_assert (is_allocator_helper<A>::value, "A is NOT an allocator!"); };

template <class_t A>
using is_allocator_t = is_allocator<A>::type;

/// is allocator -> value
template <class_t A>
inline constexpr cbool is_allocator_v = is_allocator<A>::value;

/// polymorphic allocator concept type
template <typename A>
using AllocatorType = std::enable_if_t<is_allocator_v<A>, A>;

/// polymorphic allocator concept
template <typename A>
concept allocator_t = is_allocator_v<A> || requires (A& ator)
{
    typename A::value_type     ;
    typename A::pointer        ;
    typename A::const_pointer  ;
    typename A::reference      ;
    typename A::const_reference;
    typename A::size_type      ;
    typename A::difference_type;

    { ator.allocate (typename A::size_type {}) } -> std::same_as<typename A::pointer>;
    { ator.deallocate (typename A::pointer {}, typename A::size_type {}) } -> std::same_as<void>;
    { ator.max_size () } -> std::same_as<typename A::size_type>;
    { ator.select_on_container_copy_construction () } -> std::same_as<A>;
};

// =========================================================

} // namespace Memory

// =========================================================

namespace std {

// =========================================================

using namespace cppual;

// =========================================================

template <typename U, non_void T, typename... Args>
constexpr std::shared_ptr<U> make_shared (Args&&... args)
{
    return std::static_pointer_cast<U>
            (std::allocate_shared<T> (memory::allocator<T> (), std::forward<Args> (args)...));
}

// =========================================================

template <non_void T, typename... Args>
constexpr std::shared_ptr<T> allocate_shared (memory::memory_resource& rc, Args&&... args)
{
    return std::allocate_shared<T> (memory::allocator<T> (rc), std::forward<Args> (args)...);
}

template <typename U, non_void T, typename... Args>
constexpr std::shared_ptr<U> allocate_shared (memory::memory_resource& rc, Args&&... args)
{
    return std::static_pointer_cast<U>
          (std::allocate_shared<T> (memory::allocator<T> (rc), std::forward<Args> (args)...));
}

template <typename U, non_void T, typename... Args>
constexpr std::shared_ptr<U> allocate_shared (memory::allocator<T> const& ator, Args&&... args)
{
    return std::static_pointer_cast<U>
            (std::allocate_shared<T> (ator, std::forward<Args> (args)...));
}

// =========================================================

} // namespace std

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_MEMORY_ALLOCATOR_H_