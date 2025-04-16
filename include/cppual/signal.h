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

#ifndef CPPUAL_SIGNAL_H_
#define CPPUAL_SIGNAL_H_
#ifdef __cplusplus

#include <cppual/type_meta.h>
#include <cppual/functional.h>
#include <cppual/circular_queue.h>
#include <cppual/memory/allocator.h>
#include <cppual/concept/concepts.h>

#include <iterator>
#include <vector>

namespace cppual {

// =========================================================

using memory::allocator_t;

// =========================================================

template <typename T>
using fn_type = function<T>;

template <typename S>
using signal_allocator = memory::allocator<fn_type<S>>;

template <typename T, allocator_t A = signal_allocator<T>>
using signal_queue = circular_queue<T, A>;

// =========================================================

template <typename T, allocator_t = signal_allocator<T>>
class signal;

template <typename T, allocator_t = signal_allocator<T>>
class scoped_connection;

// =========================================================

/// how many slots to be reserved at construction
inline constexpr std::size_t const reserve_slot_count_v = 5;

// =========================================================

template <typename R, typename... Args, allocator_t A>
class SHARED_API signal <R(Args...), A>
{
public:
    typedef signal<R(Args...), A>                          self_type             ;
    typedef std::allocator_traits<A>                       alloc_traits          ;
    typedef alloc_traits::allocator_type                   allocator_type        ;
    typedef alloc_traits::size_type                        size_type             ;
    typedef size_type const                                const_size            ;
    typedef FunctionalType<fn_type<R(Args...)>>            value_type            ;
    typedef value_type &                                   reference             ;
    typedef value_type const&                              const_reference       ;
    typedef signal_queue<value_type, allocator_type>       container_type        ;
    typedef container_type&                                container_ref         ;
    typedef container_type const&                          container_const_ref   ;
    typedef container_type::iterator                       iterator              ;
    typedef container_type::const_iterator                 const_iterator        ;
    typedef std::reverse_iterator<iterator>                reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>          const_reverse_iterator;
    typedef const_iterator                                 slot_type             ;
    typedef R                                              return_type           ;
    typedef allocator_type::template rebind_t<return_type> return_allocator      ;
    typedef std::vector<return_type, return_allocator>     collector_type        ;

    using scoped_connection_type = scoped_connection<R(Args...), allocator_type>;

    constexpr container_ref get_slots () noexcept
    { return _M_slots; }

    constexpr container_const_ref get_slots () const noexcept
    { return _M_slots; }

    inline void clear () noexcept
    {  _M_slots.clear (); }

    constexpr bool empty () const noexcept
    { return _M_slots.empty (); }

    constexpr size_type size () const noexcept
    { return _M_slots.size (); }

    constexpr iterator begin () noexcept
    { return _M_slots.begin (); }

    constexpr const_iterator begin () const noexcept
    { return _M_slots.begin (); }

    constexpr const_iterator cbegin () noexcept
    { return _M_slots.cbegin (); }

    constexpr const_iterator cbegin () const noexcept
    { return _M_slots.cbegin (); }

    constexpr reverse_iterator rbegin () noexcept
    { return _M_slots.rbegin (); }

    constexpr const_reverse_iterator rbegin () const noexcept
    { return _M_slots.rbegin (); }

    constexpr const_reverse_iterator crbegin () noexcept
    { return _M_slots.crbegin (); }

    constexpr const_reverse_iterator crbegin () const noexcept
    { return _M_slots.crbegin (); }

    constexpr iterator end () noexcept
    { return _M_slots.end (); }

    constexpr const_iterator end () const noexcept
    { return _M_slots.end (); }

    constexpr const_iterator cend () noexcept
    { return _M_slots.cend (); }

    constexpr const_iterator cend () const noexcept
    { return _M_slots.cend (); }

    constexpr reverse_iterator rend () noexcept
    { return _M_slots.rend (); }

    constexpr const_reverse_iterator rend () const noexcept
    { return _M_slots.rend (); }

    constexpr const_reverse_iterator crend () noexcept
    { return _M_slots.crend (); }

    constexpr const_reverse_iterator crend () const noexcept
    { return _M_slots.crend (); }

    constexpr signal (allocator_type const& ator      = allocator_type (),
                      size_type             reserve_n = reserve_slot_count_v) noexcept
    : _M_slots (ator)
    {
        if (reserve_n) _M_slots.reserve (reserve_n);
    }

    //! emit signal to connected slots
    collector_type operator () (Args... args) const
    {
        collector_type collection;
        const_size     size = _M_slots.size ();

        if (size == size_type ()) return std::move (collection);

        collection.reserve (size);

        for (size_type i = 0U; i < size; ++i)
        {
            if (_M_slots[i] != nullptr)
                collection.emplace_back (std::move (_M_slots[i] (std::forward<Args> (args)...)));
        }

        return std::move (collection);
    }

    /// (signal/slot) connect
    constexpr self_type& operator << (value_type&& fn)
    {
        connect (*this, std::move (fn));
        return   *this;
    }

    /// (signal/slot) connect
    constexpr self_type& operator << (const_reference fn)
    {
        connect (*this, fn);
        return   *this;
    }

    /// (signal/slot) connect
    constexpr self_type& operator << (R(& fn)(Args...))
    {
        connect (*this, fn);
        return   *this;
    }

    /// callable object (signal/slot) connect
    template <callable_class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr self_type& operator << (C& obj) const
    {
        if (this == &obj) return *this;

        static_assert (std::is_same_v<return_type, callable_return_t<C, Args...>>,
                      "C::operator () return type is NOT void!");

        connect (*this, obj);
        return   *this;
    }

    template <typename, allocator_t>
    friend class signal;

private:
    container_type _M_slots;
};

// =========================================================

template <typename... Args, allocator_t A>
class SHARED_API signal <void(Args...), A>
{
public:
    typedef signal<void(Args...), A>                 self_type             ;
    typedef std::allocator_traits<A>                 traits_type           ;
    typedef traits_type::allocator_type              allocator_type        ;
    typedef traits_type::size_type                   size_type             ;
    typedef size_type const                          const_size            ;
    typedef FunctionalType<fn_type<void(Args...)>>   value_type            ;
    typedef value_type &                             reference             ;
    typedef value_type const&                        const_reference       ;
    typedef signal_queue<value_type, allocator_type> container_type        ;
    typedef container_type&                          container_ref         ;
    typedef container_type const&                    container_const_ref   ;
    typedef container_type::iterator                 iterator              ;
    typedef container_type::const_iterator           const_iterator        ;
    typedef std::reverse_iterator<iterator>          reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;
    typedef container_type::const_iterator           slot_type             ;
    typedef void                                     return_type           ;

    using scoped_connection_type = scoped_connection<void(Args...), allocator_type>;

    constexpr container_ref get_slots () noexcept
    { return _M_slots; }

    constexpr container_const_ref get_slots () const noexcept
    { return _M_slots; }

    inline void clear () noexcept
    {  _M_slots.clear (); }

    constexpr bool empty () const noexcept
    { return _M_slots.empty (); }

    constexpr size_type size () const noexcept
    { return _M_slots.size (); }

    constexpr iterator begin () noexcept
    { return _M_slots.begin (); }

    constexpr const_iterator begin () const noexcept
    { return _M_slots.begin (); }

    constexpr const_iterator cbegin () noexcept
    { return _M_slots.cbegin (); }

    constexpr const_iterator cbegin () const noexcept
    { return _M_slots.cbegin (); }

    constexpr reverse_iterator rbegin () noexcept
    { return _M_slots.rbegin (); }

    constexpr const_reverse_iterator rbegin () const noexcept
    { return _M_slots.rbegin (); }

    constexpr const_reverse_iterator crbegin () noexcept
    { return _M_slots.crbegin (); }

    constexpr const_reverse_iterator crbegin () const noexcept
    { return _M_slots.crbegin (); }

    constexpr iterator end () noexcept
    { return _M_slots.end (); }

    constexpr const_iterator end () const noexcept
    { return _M_slots.end (); }

    constexpr const_iterator cend () noexcept
    { return _M_slots.cend (); }

    constexpr const_iterator cend () const noexcept
    { return _M_slots.cend (); }

    constexpr reverse_iterator rend () noexcept
    { return _M_slots.rend (); }

    constexpr const_reverse_iterator rend () const noexcept
    { return _M_slots.rend (); }

    constexpr const_reverse_iterator crend () noexcept
    { return _M_slots.crend (); }

    constexpr const_reverse_iterator crend () const noexcept
    { return _M_slots.crend (); }

    constexpr signal (allocator_type const& ator      = allocator_type (),
                      size_type             reserve_n = reserve_slot_count_v) noexcept
    : _M_slots (ator)
    {
        if (reserve_n) _M_slots.reserve (reserve_n);
    }

    //! emit signal to connected slots
    constexpr void operator () (Args... args) const
    {
        for (size_type i = 0U; i < _M_slots.size(); ++i)
            if (_M_slots[i] != nullptr) _M_slots[i] (std::forward<Args> (args)...);
    }

    /// (signal/slot) connect
    constexpr self_type& operator << (value_type&& fn) const
    {
        connect (*this, std::move (fn));
        return   *this;
    }

    /// (signal/slot) connect
    constexpr self_type& operator << (const_reference fn) const
    {
        connect (*this, fn);
        return   *this;
    }

    /// (signal/slot) connect
    constexpr self_type& operator << (void(& fn)(Args...)) const
    {
        connect (*this, fn);
        return   *this;
    }

    /// callable object (signal/slot) connect
    template <callable_class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr self_type& operator << (C& obj) const
    {
        if (this == &obj) return *this;

        static_assert (std::is_same_v<return_type, callable_return_t<C, Args...>>,
                      "C::operator () return type is NOT void!");

        connect (*this, obj);
        return   *this;
    }

    template <typename, allocator_t>
    friend class signal;

private:
    container_type _M_slots;
};

// =========================================================

template <typename... Args, allocator_t A>
class SHARED_API signal <bool(Args...), A>
{
public:
    typedef signal<void(Args...), A>                 self_type             ;
    typedef std::allocator_traits<A>                 traits_type           ;
    typedef traits_type::allocator_type              allocator_type        ;
    typedef traits_type::size_type                   size_type             ;
    typedef size_type const                          const_size            ;
    typedef FunctionalType<fn_type<void(Args...)>>   value_type            ;
    typedef value_type &                             reference             ;
    typedef value_type const&                        const_reference       ;
    typedef signal_queue<value_type, allocator_type> container_type        ;
    typedef container_type&                          container_ref         ;
    typedef container_type const&                    container_const_ref   ;
    typedef container_type::iterator                 iterator              ;
    typedef container_type::const_iterator           const_iterator        ;
    typedef std::reverse_iterator<iterator>          reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;
    typedef container_type::const_iterator           slot_type             ;
    typedef void                                     return_type           ;

    using scoped_connection_type = scoped_connection<void(Args...), allocator_type>;

    constexpr container_ref get_slots () noexcept
    { return _M_slots; }

    constexpr container_const_ref get_slots () const noexcept
    { return _M_slots; }

    inline void clear () noexcept
    {  _M_slots.clear (); }

    constexpr bool empty () const noexcept
    { return _M_slots.empty (); }

    constexpr size_type size () const noexcept
    { return _M_slots.size (); }

    constexpr iterator begin () noexcept
    { return _M_slots.begin (); }

    constexpr const_iterator begin () const noexcept
    { return _M_slots.begin (); }

    constexpr const_iterator cbegin () noexcept
    { return _M_slots.cbegin (); }

    constexpr const_iterator cbegin () const noexcept
    { return _M_slots.cbegin (); }

    constexpr reverse_iterator rbegin () noexcept
    { return _M_slots.rbegin (); }

    constexpr const_reverse_iterator rbegin () const noexcept
    { return _M_slots.rbegin (); }

    constexpr const_reverse_iterator crbegin () noexcept
    { return _M_slots.crbegin (); }

    constexpr const_reverse_iterator crbegin () const noexcept
    { return _M_slots.crbegin (); }

    constexpr iterator end () noexcept
    { return _M_slots.end (); }

    constexpr const_iterator end () const noexcept
    { return _M_slots.end (); }

    constexpr const_iterator cend () noexcept
    { return _M_slots.cend (); }

    constexpr const_iterator cend () const noexcept
    { return _M_slots.cend (); }

    constexpr reverse_iterator rend () noexcept
    { return _M_slots.rend (); }

    constexpr const_reverse_iterator rend () const noexcept
    { return _M_slots.rend (); }

    constexpr const_reverse_iterator crend () noexcept
    { return _M_slots.crend (); }

    constexpr const_reverse_iterator crend () const noexcept
    { return _M_slots.crend (); }

    constexpr signal (allocator_type const& ator      = allocator_type (),
                      size_type             reserve_n = reserve_slot_count_v) noexcept
    : _M_slots (ator)
    {
        if (reserve_n) _M_slots.reserve (reserve_n);
    }

    //! emit signal to connected slots
    constexpr void operator () (Args... args) const
    {
        for (size_type i = 0U; i < _M_slots.size(); ++i)
            if (_M_slots[i] != nullptr && !(_M_slots[i] (std::forward<Args> (args)...))) return;
    }

    /// (signal/slot) connect
    inline self_type& operator << (value_type&& fn) const
    {
        connect (*this, std::move (fn));
        return   *this;
    }

    /// (signal/slot) connect
    inline self_type& operator << (const_reference fn) const
    {
        connect (*this, fn);
        return   *this;
    }

    /// (signal/slot) connect
    inline self_type& operator << (void(& fn)(Args...)) const
    {
        connect (*this, fn);
        return   *this;
    }

    /// callable object (signal/slot) connect
    template <callable_class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    inline self_type& operator << (C& obj) const
    {
        if (this == &obj) return *this;

        static_assert (std::is_same_v<return_type, callable_return_t<C, Args...>>,
                      "C::operator () return type is NOT bool!");

        connect (*this, obj);
        return   *this;
    }

    template <typename, allocator_t>
    friend class signal;

private:
    container_type _M_slots;
};

// =========================================================

template <typename    R,
          typename... Args,
          allocator_t A
          >
inline
    typename signal<R(Args...), A>::slot_type
    connect (signal<R(Args...), A>& gSignal,
             typename signal<R(Args...), A>::value_type&& val,
             bool bTop = false)
{
    auto it = std::find (gSignal.begin (), gSignal.end (), val);

    if (it != gSignal.end ()) return it;

    if (bTop)
    {
        gSignal.get_slots ().push_front (std::move (val));
        return gSignal.get_slots ().begin ();
    }

    gSignal.get_slots ().push_back (std::move (val));
    return --gSignal.get_slots ().end ();
}

template <typename    R,
          typename... Args,
          allocator_t A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal,
         typename signal<R(Args...), A>::const_reference val,
         bool bTop = false)
{
    auto it = std::find (gSignal.begin (), gSignal.end (), val);

    if (it != gSignal.end ()) return it;

    if (bTop)
    {
        gSignal.get_slots ().push_front (val);
        return gSignal.get_slots ().begin ();
    }

    gSignal.get_slots ().push_back (val);
    return --gSignal.get_slots ().end ();
}

template <lambda_non_capture_t Call,
          typename    R,
          typename... Args,
          allocator_t A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal,
         Call&& gFunc,
         bool bTop = false,
         LambdaNonCaptureType<Call>* = nullptr)
{
    auto it = std::find (gSignal.begin (),
                         gSignal.end   (),
                         typename signal<R(Args...), A>::value_type (std::forward<Call> (gFunc)));

    if (it != gSignal.end ()) return it;

    if (bTop)
    {
        gSignal.get_slots ().emplace_front (std::move (gFunc));
        return gSignal.get_slots ().begin ();
    }

    gSignal.get_slots ().emplace_back (std::move (gFunc));
    return --gSignal.get_slots ().end ();
}

template <lambda_capture_t Call,
          typename    R,
          typename... Args,
          allocator_t A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal,
         Call&& gFunc,
         bool bTop = false,
         LambdaCaptureType<Call>* = nullptr)
{
    auto it = std::find (gSignal.begin (),
                         gSignal.end   (),
                         typename signal<R(Args...), A>::value_type (std::forward<Call> (gFunc)));

    if (it != gSignal.end ()) return it;

    if (bTop)
    {
        gSignal.get_slots ().emplace_front (std::forward<Call> (gFunc));
        return gSignal.get_slots ().begin ();
    }

    gSignal.get_slots ().emplace_back (std::forward<Call> (gFunc));
    return --gSignal.get_slots ().end ();
}

template <class_t     C,
          typename    R,
          typename... Args,
          allocator_t A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal,
         std::decay_t<C>& pObj,
         R(C::* fn)(Args...),
         bool bTop = false)
{
    auto it = std::find (gSignal.begin (),
                         gSignal.end   (),
                         typename signal<R(Args...), A>::value_type (pObj, fn));

    if (it != gSignal.end ()) return it;

    if (bTop)
    {
        gSignal.get_slots ().emplace_front (pObj, fn);
        return --gSignal.get_slots ().begin ();
    }

    gSignal.get_slots ().emplace_back (pObj, fn);
    return --gSignal.get_slots ().end ();
}

template <class_t     C,
          typename    R,
          typename... Args,
          allocator_t A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal,
         std::decay_t<C>& pObj,
         R(C::* fn)(Args...) const,
         bool bTop = false)
{
    auto it = std::find (gSignal.begin (),
                         gSignal.end   (),
                         typename signal<R(Args...), A>::value_type (pObj, fn));

    if (it != gSignal.end ()) return it;

    if (bTop)
    {
        gSignal.get_slots ().emplace_front (pObj, fn);
        return --gSignal.get_slots ().begin ();
    }

    gSignal.get_slots ().emplace_back (pObj, fn);
    return --gSignal.get_slots ().end ();
}

template <callable_class_t C,
          typename         R,
          typename...      Args,
          allocator_t      A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal, C& pObj, bool bTop = false)
{
    auto it = std::find (gSignal.begin (),
                         gSignal.end   (),
                         typename signal<R(Args...), A>::value_type (pObj));

    if (it != gSignal.end ()) return it;

    if (bTop)
    {
        gSignal.get_slots ().emplace_front (pObj);
        return --gSignal.get_slots ().begin ();
    }

    gSignal.get_slots ().emplace_back (pObj);
    return --gSignal.get_slots ().end ();
}

template <typename    R,
          typename... Args,
          allocator_t A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal, R(& fn)(Args...), bool bTop = false)
{
    auto it = std::find (gSignal.begin (),
                         gSignal.end (),
                         typename signal<R(Args...), A>::value_type (fn));

    if (it != gSignal.end ()) return it;

    if (bTop)
    {
        gSignal.get_slots ().emplace_front (fn);
        return gSignal.get_slots ().begin ();
    }

    gSignal.get_slots ().emplace_back (fn);
    return --gSignal.get_slots ().end ();
}

// =========================================================

template <typename    R,
          typename... Args,
          allocator_t A
          >
inline
void
disconnect (signal<R(Args...), A>& gSignal,
            typename signal<R(Args...), A>::slot_type& it)
{
    if (it != gSignal.end ()) gSignal.get_slots ().erase (it);
}

template <typename    R,
          typename... Args,
          allocator_t A
          >
inline
void
disconnect (signal<R(Args...), A>& gSignal,
            typename signal<R(Args...), A>::const_reference fn)
{
    auto it = std::find (gSignal.begin (), gSignal.end (), fn);

    if (it != gSignal.end ()) gSignal.get_slots ().erase (it);
}

template <class_t     C,
          typename    R,
          typename... Args,
          allocator_t A
          >
inline
void disconnect (signal<R(Args...), A>& gSignal, std::remove_const_t<C>& pObj, R(C::* fn)(Args...))
{
    using value_type = typename signal<R(Args...), A>::value_type;

    auto it = std::find (gSignal.begin (), gSignal.end (), value_type (pObj, fn));

    if (it != gSignal.end ()) gSignal.get_slots ().erase (it);
}



template <class_t     C,
          typename    R,
          typename... Args,
          allocator_t A
          >
inline
void disconnect (signal<R(Args...), A>& gSignal, std::remove_const_t<C>& pObj, R(C::* fn)(Args...) const)
{
    using value_type = typename signal<R(Args...), A>::value_type;

    auto it = std::find (gSignal.begin (), gSignal.end (), value_type (pObj, fn));

    if (it != gSignal.end ()) gSignal.get_slots ().erase (it);
}

template <callable_class_t C,
          typename         R,
          typename...      Args,
          allocator_t      A
          >
inline
void
disconnect (signal<R(Args...), A>& gSignal, C& pObj)
{
    using value_type = typename signal<R(Args...), A>::value_type;

    auto it = std::find (gSignal.begin (), gSignal.end (), value_type (pObj));

    if (it != gSignal.end ()) gSignal.get_slots ().erase (it);
}

template <typename    R,
          typename... Args,
          allocator_t A
          >
inline
void
disconnect (signal<R(Args...), A>& gSignal, R(& fn)(Args...))
{
    using value_type = typename signal<R(Args...), A>::value_type;

    auto it = std::find (gSignal.begin (), gSignal.end (), value_type (fn));

    if (it != gSignal.end ()) gSignal.get_slots ().erase (it);
}

// =========================================================

template <typename R, typename... Args, allocator_t A>
class SHARED_API scoped_connection <R(Args...), A> : public non_copyable
{
public:
    typedef scoped_connection <R(Args...), A>                           self_type           ;
    typedef std::allocator_traits<A>                                    traits_type         ;
    typedef memory::AllocatorType<typename traits_type::allocator_type> allocator_type      ;
    typedef signal<R(Args...), allocator_type>                          signal_type         ;
    typedef signal_type                                                 value_type          ;
    typedef value_type&                                                 reference           ;
    typedef value_type const&                                           const_reference     ;
    typedef value_type*                                                 pointer             ;
    typedef value_type const*                                           const_pointer       ;
    typedef value_type::value_type                                      fn_type             ;

    scoped_connection () = delete;

    inline scoped_connection (scoped_connection&&)             noexcept = default;
    inline scoped_connection& operator = (scoped_connection&&) noexcept = default;

    //! move fn constructor
    inline scoped_connection (reference gSignal, fn_type&& gFunc, bool bTop = false)
    : _M_signal (&gSignal         ),
      _M_fn     (std::move (gFunc))
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! copy fn constructor
    inline scoped_connection (reference gSignal, fn_type const& gFunc, bool bTop = false)
    : _M_signal (&gSignal),
      _M_fn     (gFunc   )
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! non capture lambda constructor
    template <lambda_non_capture_t Call>
    inline scoped_connection (reference gSignal,
                              Call&&    fn,
                              bool      bTop = false,
                              LambdaNonCaptureType<Call>* = nullptr)
    : _M_signal (&gSignal               ),
      _M_fn     (std::forward<Call> (fn))
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! capture lambda constructor
    template <lambda_capture_t Call, typename CallableAlloc>
    inline scoped_connection (reference            gSignal,
                              Call&&               fn,
                              bool                 bTop = false,
                              CallableAlloc const& call_ator = CallableAlloc (),
                              LambdaCaptureType<Call>* = nullptr)
    : _M_signal (&gSignal                          ),
      _M_fn     (std::forward<Call> (fn), call_ator)
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! member function constructor
    template <class_t C>
    inline scoped_connection (reference gSignal,
                              C& pObj,
                              R(C::* fn)(Args...),
                              bool bTop = false)
    : _M_signal (&gSignal ),
      _M_fn     (pObj,  fn)
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! member function constructor
    template <class_t C>
    inline scoped_connection (reference gSignal,
                              C& pObj,
                              R(C::* fn)(Args...) const,
                              bool bTop = false)
    : _M_signal (&gSignal),
      _M_fn     (pObj, fn)
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! static function constructor
    inline scoped_connection (reference gSignal, R(& fn)(Args...), bool bTop = false)
    : _M_signal (&gSignal),
      _M_fn     (fn      )
    {
        connect (gSignal, _M_fn, bTop);
    }

    inline ~scoped_connection ()
    {
        if (_M_fn != nullptr) disconnect (*_M_signal, _M_fn);
    }

private:
    pointer _M_signal;
    fn_type _M_fn    ;
};

// =========================================================

template <typename R,
          typename... Args,
          allocator_t A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               typename signal<R(Args...), A>::value_type&& fn,
                               bool top = false)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> pointer;
    return pointer (new scoped_connection<R(Args...), A> (_signal, std::move (fn), top));
}

template <lambda_non_capture_t Call,
          typename R,
          typename... Args,
          allocator_t A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               Call&& fn,
                               bool top = false,
                               LambdaNonCaptureType<Call>* = nullptr)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> pointer;
    return pointer (new scoped_connection<R(Args...), A> (_signal, std::forward<Call> (fn), top));
}

template <lambda_capture_t Call,
          typename R,
          typename... Args,
          allocator_t A,
          allocator_t CallableA
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               Call&& fn,
                               bool top = false,
                               LambdaCaptureType<Call>* = nullptr)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> pointer;
    return pointer (new scoped_connection<R(Args...), A> (_signal, std::forward<Call> (fn), top));
}

template <typename R,
          typename... Args,
          allocator_t A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               typename signal<R(Args...), A>::const_reference fn,
                               bool top = false)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> pointer;
    return pointer (new scoped_connection<R(Args...), A> (_signal, fn, top));
}

template <class_t  C,
          typename R,
          typename... Args,
          allocator_t A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               C& pObj,
                               R(C::* fn)(Args...),
                               bool top = false)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> pointer;
    return pointer (new scoped_connection<R(Args...), A> (_signal, pObj, fn, top));
}

template <class_t  C,
          typename R,
          typename... Args,
          allocator_t A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               C& pObj,
                               R(C::* fn)(Args...) const,
                               bool top = false)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> pointer;
    return pointer (new scoped_connection<R(Args...), A> (_signal, pObj, fn, top));
}

template <typename R,
          typename... Args,
          allocator_t A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               R(& fn)(Args...),
                               bool top = false)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> pointer;
    return pointer (new scoped_connection<R(Args...), A> (_signal, fn, top));
}

// =========================================================

template <typename R,
          typename... Args,
          allocator_t A,
          typename SharedA
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
allocate_shared_scoped_connection (SharedA alloc,
                                   signal<R(Args...), A>& _signal,
                                   typename signal<R(Args...), A>::value_type&& fn,
                                   bool top = false)
{
    return std::allocate_shared (alloc,
                                 std::move (scoped_connection<R(Args...), A> (_signal, fn, top)));
}

template <lambda_non_capture_t Call,
          typename R,
          typename... Args,
          allocator_t A,
          allocator_t SharedA
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
allocate_shared_scoped_connection (SharedA alloc,
                                   signal<R(Args...), A>& _signal,
                                   Call&& fn,
                                   bool top = false,
                                   LambdaNonCaptureType<Call>* = nullptr)
{
    return std::allocate_shared (alloc,
                                 std::move (scoped_connection<R(Args...), A>
                                           (_signal, std::forward<Call>(fn), top)));
}

template <lambda_capture_t Call,
          typename R,
          typename... Args,
          allocator_t A,
          allocator_t SharedA,
          allocator_t CallableA
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A> >
allocate_shared_scoped_connection (SharedA alloc,
                                   signal<R(Args...), A>& _signal,
                                   Call&& fn,
                                   bool top = false,
                                   CallableA const& call_ator = CallableA (),
                                   LambdaCaptureType<Call>* = nullptr)
{
    return std::allocate_shared (alloc,
                                 std::move (scoped_connection<R(Args...), A>
                                           (_signal, std::forward<Call>(fn), top, call_ator)));
}

template <typename R,
          typename... Args,
          allocator_t A,
          allocator_t SharedA
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A> >
allocate_shared_scoped_connection (SharedA alloc,
                                   signal<R(Args...), A>& _signal,
                                   typename signal<R(Args...), A>::const_reference fn,
                                   bool top = false)
{
    return std::allocate_shared (alloc,
                                 std::move (scoped_connection<R(Args...), A> (_signal, fn, top)));
}

template <class_t  C,
          typename R,
          typename... Args,
          allocator_t A,
          allocator_t SharedA
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A> >
allocate_shared_scoped_connection (SharedA alloc,
                                   signal<R(Args...), A>& _signal,
                                   C& pObj,
                                   R(C::* fn)(Args...),
                                   bool top = false)
{
    return std::allocate_shared (alloc,
                                 std::move (scoped_connection<R(Args...), A> (_signal, pObj, fn, top)));
}

template <class_t  C,
          typename R,
          typename... Args,
          allocator_t A,
          allocator_t SharedA
          >
inline
    std::shared_ptr<scoped_connection<R(Args...), A> >
    allocate_shared_scoped_connection (SharedA alloc,
                                       signal<R(Args...), A>& _signal,
                                       C& pObj,
                                       R(C::* fn)(Args...) const,
                                       bool top = false)
{
    return std::allocate_shared (alloc,
                                 std::move (scoped_connection<R(Args...), A> (_signal, pObj, fn, top)));
}

template <typename R,
          typename... Args,
          allocator_t A,
          allocator_t SharedA
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A> >
allocate_shared_scoped_connection (SharedA alloc,
                                   signal<R(Args...), A>& _signal,
                                   R(& fn)(Args...),
                                   bool top = false)
{
    return std::allocate_shared (alloc,
                                 std::move (scoped_connection<R(Args...), A> (_signal, fn, top)));
}

// =========================================================

} // namespace cppual

#endif // __cplusplus
#endif // CPPUAL_SIGNAL_H_
