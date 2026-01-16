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

// =========================================================

using std::scoped_allocator_adaptor;

// =========================================================

template <allocator_like A>
struct allocator_traits : public std::allocator_traits<A>
{ typedef A allocator_type; };

// =========================================================

//! std::pmr::memory_resource replacement for compute usage
class SHARED_API memory_resource : public std::pmr::memory_resource
{
public:
    typedef memory_resource           self_type             ;
    typedef self_type                 base_type             ;
    typedef self_type *               base_pointer          ;
    typedef self_type &               base_reference        ;
    typedef self_type const&          base_const_reference  ;
    typedef self_type *&              base_pointer_reference;
    typedef self_type **              base_double_pointer   ;
    typedef std::pmr::memory_resource abs_base_type         ;
    typedef abs_base_type&            abs_base_reference    ;
    typedef std::size_t               size_type             ;
    typedef size_type const           const_size            ;
    typedef size_type                 align_type            ;
    typedef const_size                const_align           ;
    typedef byte *                    math_pointer          ;
    typedef cbyte*                    math_const_pointer    ;
    typedef void *                    pointer               ;
    typedef cvoid*                    const_pointer         ;
    typedef ptrdiff                   difference_type       ;
    typedef compute::device           device_type           ;
    typedef device_type *             device_pointer        ;
    typedef device_type &             device_reference      ;
    typedef device_type const&        device_const_reference;

    //! maximum alignment depending on memory width
    inline constexpr static const_align max_align  = alignof (std::max_align_t);

    //! maximum align ajustment for memory allocation
    inline constexpr static const_align max_adjust = alignof (std::max_align_t) * 2;

    constexpr memory_resource (device_reference dev = host_device ()) noexcept
    : _M_pDevice (&dev)
    { }

    constexpr memory_resource (abs_base_type const& other) noexcept
    : abs_base_type (other          )
    , _M_pDevice    (&host_device ())
    { }

    constexpr bool is_host_device () const noexcept
    { return _M_pDevice == &host_device (); }

    constexpr device_const_reference device () const noexcept
    { return *_M_pDevice; }

    constexpr device_reference device () noexcept
    { return *_M_pDevice; }

    //! is thread safe (ex. using mutex or atomic variables)
    constexpr virtual bool is_thread_safe () const noexcept
    { return false; }

    //! is either atomic for true or is using mutex for false.
    //! it can still be lock free if it has NO inter-thread synchronization
    constexpr virtual bool is_lock_free () const noexcept
    { return true; }

    //! is using mutex for synchronization
    constexpr bool is_using_mutex () const noexcept
    { return is_thread_safe () && !is_lock_free (); }

    //! is atomic (lock free and thread safe)
    constexpr bool is_atomic () const noexcept
    { return is_thread_safe () && is_lock_free (); }

    //! is inter-process shared memory
    constexpr virtual bool is_shared () const noexcept
    { return false; }

    //! upstream memory_resource owner
    constexpr virtual base_const_reference owner () const noexcept
    { return *this; }

    //! upstream memory_resource owner
    constexpr virtual base_reference owner () noexcept
    { return *this; }

    //! upstream memory_resource owner
    constexpr base_const_reference upstream_resource () const noexcept
    { return owner (); }

    //! upstream memory_resource owner
    constexpr base_reference upstream_resource () noexcept
    { return owner (); }

    //! maximum continues bytes that can be allocated at ones
    //! from the memory_resource
    constexpr virtual size_type max_size () const noexcept
    { return std::numeric_limits<size_type>::max () - max_align; }

    //! memory capacity in bytes
    constexpr virtual size_type capacity () const
    { return std::numeric_limits<size_type>::max () - max_align; }

    constexpr void* reallocate (pointer    p       ,
                                size_type  old_size,
                                size_type  new_size,
                                align_type align = max_align)
    { return do_reallocate (p, old_size, new_size, align); }

protected:
    constexpr virtual pointer do_reallocate (pointer    p       ,
                                             size_type  old_size,
                                             size_type  new_size,
                                             align_type align)
    {
        if (p && old_size && old_size == new_size) return p;

        auto const old_p = static_cast<math_pointer> (p);
        auto const new_p = static_cast<math_pointer> (allocate (new_size, align));

        if (p)
        {
            std::copy  (old_p, old_p + old_size, new_p);
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

//! statically constructed memory_resource using new & delete operators
memory_resource& SHARED_FN_API new_delete_resource () noexcept;

//! statically constructed memory_resource for testing purposes and move semantics.
//! on allocation throws std::bad_alloc on deallocation does nothing
memory_resource& SHARED_FN_API null_resource () noexcept;

//! statically constructed memory_resource using malloc & free functions
memory_resource& SHARED_FN_API malloc_resource () noexcept;

//! get the default memory_resource
memory_resource& SHARED_FN_API get_default_resource () noexcept;

//! get the default memory_resource for the current thread
memory_resource& SHARED_FN_API get_default_thread_resource () noexcept;

//! set the default memory_resource
void SHARED_FN_API set_default_resource (memory_resource& resource) noexcept;

//! set the default memory_resource for the current thread
void SHARED_FN_API set_default_thread_resource (memory_resource& resource) noexcept;

//! get main thread id
std::thread::id SHARED_FN_API main_thread_id () noexcept;

// =========================================================

//! redefined polymorphic memory allocator
template <non_void T>
class SHARED_API allocator
{
public:
    typedef allocator<T>               self_type                             ;
    typedef remove_const_t<T>          value_type                            ;
    typedef value_type *               pointer                               ;
    typedef value_type const*          const_pointer                         ;
    typedef value_type &               reference                             ;
    typedef value_type const&          const_reference                       ;
    typedef memory_resource            resource_type                         ;
    typedef resource_type *            resource_pointer                      ;
    typedef resource_type const*       resource_const_pointer                ;
    typedef resource_type &            resource_reference                    ;
    typedef resource_type const&       resource_const_reference              ;
    typedef std::size_t                size_type                             ;
    typedef size_type  const           const_size                            ;
    typedef size_type                  align_type                            ;
    typedef align_type const           const_align                           ;
    typedef ptrdiff                    difference_type                       ;
    typedef void *                     base_pointer                          ;
    typedef cvoid*                     base_const_pointer                    ;
    typedef std::true_type             propagate_on_container_copy_assignment;
    typedef std::true_type             propagate_on_container_move_assignment;
    typedef std::true_type             propagate_on_container_swap           ;
    typedef resource_type::device_type device_type                           ;
    typedef device_type*               device_pointer                        ;
    typedef device_type&               device_reference                      ;

    inline constexpr static const_align max_align = memory_resource::max_align;

    template <non_void U>
    struct rebind { typedef allocator<U> other; };

    template <non_void U>
    using rebind_t = rebind<U>::other;

    template <non_void U>
    using rebind_alloc = rebind_t<U>;

    template <non_void U>
    using self_type_t = allocator<U>;

    constexpr device_reference device () const noexcept
    {  return resource ().device (); }

    constexpr bool is_host_device () const noexcept
    {  return resource ().is_host_device (); }

    constexpr bool is_thread_safe () const noexcept
    {  return resource ().is_thread_safe (); }

    constexpr bool is_lock_free () const noexcept
    {  return resource ().is_lock_free (); }

    constexpr bool is_shared () const noexcept
    {  return resource ().is_shared (); }

    //! upstream memory_resource owner
    constexpr resource_reference resource_owner () noexcept
    {  return resource ().owner (); }

    //! upstream const memory_resource owner
    constexpr resource_const_reference resource_owner () const noexcept
    {  return resource ().owner (); }

    /**
     ** @brief get the memory_resource used by the allocator.
     ** if no resource is set, the default thread resource is used.
     ** the resource is cached after the first call.
     ** this way the default constructor can be consteval.
     **
     ** @return resource_pointer
     **/
    constexpr resource_reference resource () const noexcept
    {
        static thread_local resource_reference rc = get_default_thread_resource ();

        return _M_pRc != nullptr ? *_M_pRc : *(_M_pRc = &rc);
    }

    constexpr pointer allocate (size_type n = 1)
    {
        return static_cast<pointer> (resource ().allocate (sizeof  (value_type) * n,
                                                           alignof (value_type)));
    }

    constexpr pointer reallocate (pointer p, size_type old_n, size_type n)
    {
        return static_cast<pointer> (resource ().reallocate (p,
                                                             sizeof  (value_type) * old_n,
                                                             sizeof  (value_type) *     n,
                                                             alignof (value_type)));
    }

    constexpr void deallocate (const_pointer p, size_type n = 1)
    { resource ().deallocate (const_cast<pointer> (p), n * sizeof (value_type), alignof (value_type)); }

    constexpr size_type max_size () const noexcept
    { return resource ().max_size () / sizeof (value_type); }

    constexpr size_type max_count () const noexcept
    { return max_size (); }

    constexpr static pointer address (reference o) noexcept
    { return std::addressof (o); }

    constexpr static const_pointer address (const_reference o) noexcept
    { return std::addressof (o); }

    template <typename... Args>
    constexpr static void construct (pointer p, Args&&... args)
    { ::new (p) value_type (std::forward<Args> (args)...); }

    constexpr static void destroy (pointer p)
    { p->~value_type (); }

    constexpr void set_resource (resource_reference res) noexcept
    { _M_pRc = &res; }

    constexpr base_pointer allocate_bytes (size_type bytes, align_type align = max_align)
    { return resource ().allocate (bytes, align); }

    constexpr void deallocate_bytes (pointer p, size_type bytes, align_type align = max_align)
    { return resource ().deallocate (p, bytes, align); }

    template <non_void U>
    constexpr U* allocate_object (size_type n = 1)
    { return static_cast<U*> (resource ().allocate (n * sizeof (U), alignof (U))); }

    template <non_void U>
    constexpr void deallocate_object (U* p, size_type n = 1)
    { resource ().deallocate (p, n * sizeof (U), alignof (U)); }

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

    consteval allocator () noexcept = default;

    constexpr allocator (resource_reference res) noexcept
    : _M_pRc (&res)
    { }

    template <non_void U>
    constexpr allocator (self_type_t<U> const& rh) noexcept
    : _M_pRc (rh._M_pRc)
    { }

    template <non_void U>
    constexpr allocator (self_type_t<U>&& rh) noexcept
    : _M_pRc (rh._M_pRc)
    { rh._M_pRc = &null_resource (); }

    template <non_void U>
    constexpr allocator (std::allocator<U> const&) noexcept
    : _M_pRc (&new_delete_resource ())
    { }

    template <non_void U>
    constexpr allocator (std::allocator<U>&&) noexcept
    : _M_pRc (&new_delete_resource ())
    { }

    template <non_void U>
    constexpr allocator (std::pmr::polymorphic_allocator<U> const&) noexcept
    : _M_pRc (&get_default_resource ())
    { }

    template <non_void U>
    constexpr allocator (std::pmr::polymorphic_allocator<U>&& rh) noexcept
    : _M_pRc (&get_default_resource ())
    { rh = std::pmr::polymorphic_allocator<U> (&null_resource ()); }

    template <non_void U>
    constexpr self_type& operator = (self_type_t<U> const& rh) noexcept
    {
        if (this != &rh) _M_pRc = rh._M_pRc;
        return *this;
    }

    template <non_void U>
    constexpr self_type& operator = (self_type_t<U>&& rh) noexcept
    {
        if (this != &rh)
        {
            _M_pRc    = rh._M_pRc;
            rh._M_pRc = &null_resource ();
        }

        return *this;
    }

    template <non_void U>
    constexpr self_type& operator = (resource_reference res) noexcept
    {
        set_resource (res);
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

    template <non_void U>
    constexpr self_type& operator = (std::pmr::polymorphic_allocator<U> const&) noexcept
    {
        _M_pRc = &get_default_resource ();
        return *this;
    }

    template <non_void U>
    constexpr self_type& operator = (std::pmr::polymorphic_allocator<U>&& rh) noexcept
    {
        _M_pRc = &get_default_resource ();
        rh     = std::pmr::polymorphic_allocator<U> (&null_resource ());
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

//! =========================================================
//! allocator comparisons
//! =========================================================

template <non_void T>
constexpr bool operator == (allocator<T> const& lh, allocator<T> const& rh) noexcept
{
    return &lh.resource () == &rh.resource () || lh.resource ().is_equal (rh.resource ());
}

template <non_void T>
constexpr bool operator != (allocator<T> const& lh, allocator<T> const& rh) noexcept
{ return !(lh == rh); }

template <non_void T1, non_void T2>
constexpr bool operator == (allocator<T1> const& lh, allocator<T2> const& rh) noexcept
{
    return &lh.resource () == &rh.resource () || lh.resource ().is_equal (rh.resource ());
}

template <non_void T1, non_void T2>
constexpr bool operator != (allocator<T1> const& lh, allocator<T2> const& rh) noexcept
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

//! =========================================================
//! allocator swap
//! =========================================================

template <non_void U>
constexpr void swap (allocator<U>& lh, allocator<U>& rh) noexcept
{ std::swap (lh._M_pRc, rh._M_pRc); }

template <non_void U1, non_void U2>
constexpr void swap (allocator<U1>& lh, allocator<U2>& rh) noexcept
{ std::swap (lh._M_pRc, rh._M_pRc); }

// =========================================================

} // namespace Memory

// =========================================================

namespace std {

// =========================================================

using cppual::non_void               ;
using cppual::memory::allocator      ;
using cppual::memory::memory_resource;

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