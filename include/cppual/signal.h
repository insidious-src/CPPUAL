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

#include <cppual/concepts>
#include <cppual/meta_type>
#include <cppual/functional>
#include <cppual/circular_queue>
#include <cppual/memory_allocator>

#include <iterator>
#include <vector>
//#include <array>

// =========================================================

namespace cppual {

// =========================================================

template <typename A>
concept slot_allocator = allocator_like<A> && requires
{
    typename A::value_type          ;
    typename A::value_type::sig_type;

    are_same<typename A::value_type, function<typename A::value_type::sig_type>>;
};

// =========================================================

template <fn_sig S, slot_allocator = memory::allocator<function<S>>>
class signal;

template <fn_sig S, slot_allocator = memory::allocator<function<S>>>
class scoped_connection;

// =========================================================

/// how many slots to be reserved at construction
inline constexpr static const std::size_t reserve_slot_count_v = 5;

// =========================================================

template <typename R, typename... Args, slot_allocator A>
class SHARED_API signal <R(Args...), A> : public circular_queue<function<R(Args...)>, A>
{
public:
    typedef signal<R(Args...), A>                      self_type             ;
    typedef circular_queue<function<R(Args...)>, A>    base_type             ;
    typedef memory::allocator_traits<A>                alloc_traits          ;
    typedef alloc_traits::allocator_type               allocator_type        ;
    typedef alloc_traits::size_type                    size_type             ;
    typedef size_type const                            const_size            ;
    typedef base_type::value_type                      value_type            ;
    typedef value_type &                               reference             ;
    typedef value_type const&                          const_reference       ;
    typedef circular_queue<value_type, allocator_type> container_type        ;
    typedef container_type &                           container_ref         ;
    typedef container_type const&                      container_const_ref   ;
    typedef container_type::iterator                   iterator              ;
    typedef container_type::const_iterator             const_iterator        ;
    typedef std::reverse_iterator<iterator>            reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
    typedef const_iterator                             slot_type             ;
    typedef value_type::return_type                    return_type           ;
    typedef allocator_type::template rebind_t<R>       return_allocator      ;
    typedef std::vector<R, return_allocator>           collector_type        ;

    using scoped_connection_type = scoped_connection<return_type(Args...), allocator_type>;
    using static_fn_ref          = return_type(&)(Args...);

    using base_type::empty        ;
    using base_type::size         ;
    using base_type::reserve      ;
    using base_type::begin        ;
    using base_type::end          ;
    using base_type::cbegin       ;
    using base_type::cend         ;
    using base_type::rbegin       ;
    using base_type::rend         ;
    using base_type::crbegin      ;
    using base_type::crend        ;
    using base_type::push_back    ;
    using base_type::emplace_back ;
    using base_type::push_front   ;
    using base_type::emplace_front;
    using base_type::pop_back     ;
    using base_type::pop_front    ;
    using base_type::clear        ;

    constexpr signal (allocator_type const& ator      = allocator_type (),
                      size_type             reserve_n = reserve_slot_count_v) noexcept
    : base_type (ator)
    {
        if (reserve_n) reserve (reserve_n);
    }

    //! emit signal to connected slots
    std::conditional_t<void_t<return_type>, void, collector_type>
    operator () (Args... args) const
    {
        if constexpr (void_t<return_type>)
        {
            for (const_reference slot : *this) slot (std::forward<Args> (args)...);
        }
        else
        {
            collector_type collection (return_allocator (this->get_allocator ()));

            if (!empty ()) collection.reserve (size ());

            for (const_reference slot : *this)
            {
                collection.emplace_back (std::move (slot (std::forward<Args> (args)...)));
            }

            return std::move (collection);
        }
    }

    //! (signal/slot) connect
    constexpr self_type& operator << (value_type&& fn)
    {
        connect (*this, std::move (fn));
        return   *this;
    }

    //! (signal/slot) connect
    constexpr self_type& operator << (const_reference fn)
    {
        connect (*this, fn);
        return   *this;
    }

    //! (signal/slot) connect
    constexpr self_type& operator << (static_fn_ref fn)
    {
        connect (*this, fn);
        return   *this;
    }

    template <class_and_non_functional C>
    constexpr self_type& operator << (std::pair<C&, return_type (C::*)(Args...)> pair) const
    {
        connect (pair.first, pair.second);
        return *this;
    }

    template <class_and_non_functional C>
    constexpr self_type& operator << (std::pair<C&, return_type (C::*)(Args...) const> pair) const
    {
        connect (pair.first, pair.second);
        return *this;
    }

    /// callable object (signal/slot) connect
    template <callable_class C, std::enable_if_t<!is_functional_v<C>, void>>
    constexpr self_type& operator << (C& obj) const
    {
        connect (*this, obj);
        return *this;
    }

    template <fn_sig, slot_allocator>
    friend class signal;
};

// =========================================================

template <typename... Args, slot_allocator A>
class SHARED_API signal <bool(Args...), A> : public circular_queue<function<bool(Args...)>, A>
{
public:
    typedef signal<bool(Args...), A>                   self_type             ;
    typedef circular_queue<function<bool(Args...)>, A> base_type             ;
    typedef memory::allocator_traits<A>                traits_type           ;
    typedef traits_type::allocator_type                allocator_type        ;
    typedef traits_type::size_type                     size_type             ;
    typedef size_type const                            const_size            ;
    typedef base_type::value_type                      value_type            ;
    typedef value_type &                               reference             ;
    typedef value_type const&                          const_reference       ;
    typedef circular_queue<value_type, allocator_type> container_type        ;
    typedef container_type&                            container_ref         ;
    typedef container_type const&                      container_const_ref   ;
    typedef container_type::iterator                   iterator              ;
    typedef container_type::const_iterator             const_iterator        ;
    typedef std::reverse_iterator<iterator>            reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
    typedef container_type::const_iterator             slot_type             ;
    typedef value_type::return_type                    return_type           ;

    using scoped_connection_type = scoped_connection<return_type(Args...), allocator_type>;
    using static_fn_ref          = return_type(&)(Args...);

    using base_type::empty        ;
    using base_type::size         ;
    using base_type::reserve      ;
    using base_type::begin        ;
    using base_type::end          ;
    using base_type::cbegin       ;
    using base_type::cend         ;
    using base_type::rbegin       ;
    using base_type::rend         ;
    using base_type::crbegin      ;
    using base_type::crend        ;
    using base_type::push_back    ;
    using base_type::emplace_back ;
    using base_type::push_front   ;
    using base_type::emplace_front;
    using base_type::pop_back     ;
    using base_type::pop_front    ;
    using base_type::clear        ;

    constexpr signal (allocator_type const& ator      = allocator_type (),
                      size_type             reserve_n = reserve_slot_count_v) noexcept
    : base_type (ator)
    {
        if (reserve_n) reserve (reserve_n);
    }

    //! emit signal to connected slots
    constexpr void operator () (Args... args) const
    {
        for (const_reference slot : *this)
            if (!(slot (std::forward<Args> (args)...))) return;
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
    constexpr self_type& operator << (static_fn_ref fn) const
    {
        connect (*this, fn);
        return   *this;
    }

    template <class_and_non_functional C>
    constexpr self_type& operator << (std::pair<C&, return_type (C::*)(Args...)> pair) const
    {
        connect (pair.first, pair.second);
        return *this;
    }

    template <class_and_non_functional C>
    constexpr self_type& operator << (std::pair<C&, return_type (C::*)(Args...) const> pair) const
    {
        connect (pair.first, pair.second);
        return *this;
    }

    /// callable object (signal/slot) connect
    template <callable_class C, std::enable_if_t<!is_functional_v<C>, void>>
    constexpr self_type& operator << (C& obj) const
    {
        if (this == &obj) return *this;

        static_assert (std::is_same_v<return_type, callable_return_t<C, Args...>>,
                      "C::operator () return type is NOT bool!");

        connect (*this, obj);
        return   *this;
    }

    template <fn_sig, slot_allocator>
    friend class signal;
};



// =========================================================

//! TODO: multiple variadic slots to be connected to a single signal
//! with connect function overloads

// =========================================================

template <typename    R,
          typename... Args,
          slot_allocator A
          >
inline
    typename signal<R(Args...), A>::slot_type
    connect (signal<R(Args...), A>& gSignal,
             typename signal<R(Args...), A>::value_type&& val,
             bool bTop = false)
{
    for (auto it = gSignal.begin (); it != gSignal.end (); ++it)
    {
        if (*it == val)
        {
            if (bTop)
            {
                gSignal.push_front (std::move (val));
                return gSignal.begin ();
            }

            gSignal.push_back (std::move (val));
            return --gSignal.end ();
        }
    }

    return --gSignal.end ();
}

// =========================================================

template <typename    R,
          typename... Args,
          slot_allocator A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal,
         typename signal<R(Args...), A>::const_reference val,
         bool bTop = false)
{
    for (auto it = gSignal.begin (); it != gSignal.end (); ++it)
    {
        if (*it == val)
        {
            if (bTop)
            {
                gSignal.push_front (val);
                return gSignal.begin ();
            }

            gSignal.push_back (val);
            return --gSignal.end ();
        }
    }

    return --gSignal.end ();
}

// =========================================================

template <lambda_non_capture Call,
          typename           R,
          typename...        Args,
          slot_allocator A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal,
         Call&& gFunc,
         bool bTop = false,
         LambdaNonCaptureType<Call>* = nullptr)
{
    for (auto it = gSignal.begin (); it != gSignal.end (); ++it)
    {
        auto fn = typename signal<R(Args...), A>::value_type (std::move (gFunc));

        if (*it == fn)
        {
            if (bTop)
            {
                gSignal.emplace_front (std::move (fn));
                return gSignal.begin ();
            }

            gSignal.emplace_back (std::move (fn));
            return --gSignal.end ();
        }
    }

    return --gSignal.end ();
}

// =========================================================

template <lambda_capture Call,
          typename       R,
          typename...    Args,
          slot_allocator A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal,
         Call&& gFunc,
         bool bTop = false,
         LambdaCaptureType<Call>* = nullptr)
{
    for (auto it = gSignal.begin (); it != gSignal.end (); ++it)
    {
        if (*it == typename signal<R(Args...), A>::value_type (gFunc))
        {
            if (bTop)
            {
                gSignal.emplace_front (std::move (gFunc));
                return gSignal.begin ();
            }

            gSignal.emplace_back (std::move (gFunc));
            return --gSignal.end ();
        }
    }

    return --gSignal.end ();
}

// =========================================================

template <structure   C,
          typename    R,
          typename... Args,
          slot_allocator A
          >
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal,
         std::decay_t<C>& pObj,
         R(C::* fn)(Args...),
         bool bTop = false)
{
    for (auto it = gSignal.begin (); it != gSignal.end (); ++it)
    {
        if (*it == make_fn (pObj, fn))
        {
            if (bTop)
            {
                gSignal.emplace_front (pObj, fn);
                return gSignal.begin ();
            }

            gSignal.emplace_back (pObj, fn);
            return --gSignal.end ();
        }
    }

    return --gSignal.end ();
}

// =========================================================

template <structure   C,
          typename    R,
          typename... Args,
          slot_allocator A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal,
         std::decay_t<C>& pObj,
         R(C::* fn)(Args...) const,
         bool bTop = false)
{
    for (auto it = gSignal.begin (); it != gSignal.end (); ++it)
    {
        if (*it == make_fn (pObj, fn))
        {
            if (bTop)
            {
                gSignal.emplace_front (pObj, fn);
                return gSignal.begin ();
            }

            gSignal.emplace_back (pObj, fn);
            return --gSignal.end ();
        }
    }

    return --gSignal.end ();
}

// =========================================================

template <callable_class C,
          typename       R,
          typename...    Args,
          slot_allocator      A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal, C& pObj, bool bTop = false)
{
    for (auto it = gSignal.begin (); it != gSignal.end (); ++it)
    {
        if (*it == make_fn (pObj))
        {
            if (bTop)
            {
                gSignal.emplace_front (pObj);
                return gSignal.begin ();
            }

            gSignal.emplace_back (pObj);
            return --gSignal.end ();
        }
    }

    return --gSignal.end ();
}

// =========================================================

template <typename    R,
          typename... Args,
          slot_allocator A
          >
inline
typename signal<R(Args...), A>::slot_type
connect (signal<R(Args...), A>& gSignal, R(& fn)(Args...), bool bTop = false)
{
    for (auto it = gSignal.begin (); it != gSignal.end (); ++it)
    {
        if (*it == fn)
        {
            if (bTop)
            {
                gSignal.emplace_front (fn);
                return gSignal.begin ();
            }

            gSignal.emplace_back (fn);
            return --gSignal.end ();
        }
    }

    return --gSignal.end ();
}

// =========================================================

template <typename R, typename... Args, slot_allocator A>
inline
void
disconnect (signal<R(Args...), A>& gSignal,
            typename signal<R(Args...), A>::slot_type& it)
{
    if (it != gSignal.end ()) gSignal.erase (it);
}

template <typename R, typename... Args, slot_allocator A>
inline
void
disconnect (signal<R(Args...), A>& gSignal,
            typename signal<R(Args...), A>::const_reference fn)
{
    auto it = std::find (gSignal.begin (), gSignal.end (), fn);

    if (it != gSignal.end ()) gSignal.erase (it);
}

template <structure   C,
          typename    R,
          typename... Args,
          slot_allocator A
          >
inline
void
disconnect (signal<R(Args...), A>& gSignal, remove_const_t<C>& pObj, R(C::* fn)(Args...))
{
    using value_type = typename signal<R(Args...), A>::value_type;

    auto it = std::find (gSignal.begin (), gSignal.end (), value_type (pObj, fn));

    if (it != gSignal.end ()) gSignal.erase (it);
}



template <structure   C,
          typename    R,
          typename... Args,
          slot_allocator A
          >
inline
void
disconnect (signal<R(Args...), A>& gSignal, remove_const_t<C>& pObj, R(C::* fn)(Args...) const)
{
    using value_type = typename signal<R(Args...), A>::value_type;

    auto it = std::find (gSignal.begin (), gSignal.end (), value_type (pObj, fn));

    if (it != gSignal.end ()) gSignal.erase (it);
}

template <callable_class C,
          typename       R,
          typename...    Args,
          slot_allocator A
          >
inline
void
disconnect (signal<R(Args...), A>& gSignal, C& pObj)
{
    using value_type = typename signal<R(Args...), A>::value_type;

    auto it = std::find (gSignal.begin (), gSignal.end (), value_type (pObj));

    if (it != gSignal.end ()) gSignal.erase (it);
}

template <typename    R,
          typename... Args,
          slot_allocator A
          >
inline
void
disconnect (signal<R(Args...), A>& gSignal, R(& fn)(Args...))
{
    using value_type = typename signal<R(Args...), A>::value_type;

    auto it = std::find (gSignal.begin (), gSignal.end (), value_type (fn));

    if (it != gSignal.end ()) gSignal.erase (it);
}

// =========================================================

template <typename R, typename... Args, slot_allocator A>
class SHARED_API scoped_connection <R(Args...), A> : public non_copyable
{
public:
    typedef scoped_connection<R(Args...), A>   self_type      ;
    typedef memory::allocator_traits<A>        alloc_traits   ;
    typedef alloc_traits::allocator_type       allocator_type ;
    typedef signal<R(Args...), allocator_type> container_type ;
    typedef container_type::value_type         value_type     ;
    typedef value_type &                       reference      ;
    typedef value_type const&                  const_reference;
    typedef value_type *                       pointer        ;
    typedef value_type const*                  const_pointer  ;

    constexpr scoped_connection (scoped_connection&&)             noexcept = default;
    constexpr scoped_connection& operator = (scoped_connection&&) noexcept = default;

    //! move fn constructor
    constexpr scoped_connection (reference gSignal, value_type&& gFunc, bool bTop = false)
    : _M_signal (&gSignal         ),
      _M_fn     (std::move (gFunc))
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! copy fn constructor
    constexpr scoped_connection (reference gSignal, value_type const& gFunc, bool bTop = false)
    : _M_signal (&gSignal),
      _M_fn     (gFunc   )
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! non capture lambda constructor
    template <lambda_non_capture Call>
    constexpr scoped_connection (reference gSignal,
                                 Call&&    fn,
                                 bool      bTop = false,
                                 Call*     ptr  = nullptr)
    : _M_signal (&gSignal               ),
      _M_fn     (std::forward<Call> (fn))
    {
        connect (gSignal, _M_fn, bTop, ptr);
    }

    //! capture lambda constructor
    template <lambda_capture Call, slot_allocator LambdaAlloc>
    constexpr scoped_connection (container_type&    gSignal,
                                 Call&&             fn,
                                 bool               bTop      = false,
                                 LambdaAlloc const& call_ator = LambdaAlloc (),
                                 Call*              ptr       = nullptr)
    : _M_signal (&gSignal                          ),
      _M_fn     (std::forward<Call> (fn), call_ator)
    {
        connect (gSignal, _M_fn, bTop, ptr);
    }

    //! member function constructor
    template <structure C>
    constexpr scoped_connection (container_type& gSignal,
                                 C& pObj,
                                 R(C::* fn)(Args...),
                                 bool bTop = false)
    : _M_signal (&gSignal ),
      _M_fn     (pObj,  fn)
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! member function constructor
    template <structure C>
    constexpr scoped_connection (container_type& gSignal,
                                 C& pObj,
                                 R(C::* fn)(Args...) const,
                                 bool bTop = false)
    : _M_signal (&gSignal),
      _M_fn     (pObj, fn)
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! static function constructor
    constexpr scoped_connection (container_type& gSignal, R(& fn)(Args...), bool bTop = false)
    : _M_signal (&gSignal),
      _M_fn     (fn      )
    {
        connect (gSignal, _M_fn, bTop);
    }

    constexpr ~scoped_connection ()
    {
        if (_M_fn != nullptr) disconnect (*_M_signal, _M_fn);
    }

private:
    scoped_connection () = delete;

private:
    container_type* _M_signal;
    value_type      _M_fn    ;
};

// =========================================================

template <typename R,
          typename... Args,
          slot_allocator A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               typename signal<R(Args...), A>::value_type&& fn,
                               bool top = false)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> shared_ptr;
    return  shared_ptr (new scoped_connection<R(Args...), A> (_signal, std::move (fn), top));
}

template <lambda_non_capture Call,
          typename R,
          typename... Args,
          slot_allocator A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               Call&& fn,
                               bool top = false,
                               LambdaNonCaptureType<Call>* = nullptr)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> shared_ptr;
    return  shared_ptr (new scoped_connection<R(Args...), A> (_signal, std::forward<Call> (fn), top));
}

template <lambda_capture Call,
          typename R,
          typename... Args,
          slot_allocator A,
          slot_allocator CallableA
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               Call&& fn,
                               bool top = false,
                               LambdaCaptureType<Call>* = nullptr)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> shared_ptr;
    return  shared_ptr (new scoped_connection<R(Args...), A> (_signal, std::forward<Call> (fn), top));
}

template <typename R,
          typename... Args,
          slot_allocator A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               typename signal<R(Args...), A>::const_reference fn,
                               bool top = false)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> shared_ptr;
    return  shared_ptr (new scoped_connection<R(Args...), A> (_signal, fn, top));
}

template <structure   C,
          typename    R,
          typename... Args,
          slot_allocator A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               C& pObj,
                               R(C::* fn)(Args...),
                               bool top = false)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> shared_ptr;
    return  shared_ptr (new scoped_connection<R(Args...), A> (_signal, pObj, fn, top));
}

template <structure   C,
          typename    R,
          typename... Args,
          slot_allocator A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               C& pObj,
                               R(C::* fn)(Args...) const,
                               bool top = false)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> shared_ptr;
    return  shared_ptr (new scoped_connection<R(Args...), A> (_signal, pObj, fn, top));
}

template <typename R,
          typename... Args,
          slot_allocator A
          >
inline
std::shared_ptr<scoped_connection<R(Args...), A>>
make_shared_scoped_connection (signal<R(Args...), A>& _signal,
                               R(& fn)(Args...),
                               bool top = false)
{
    typedef std::shared_ptr<scoped_connection<R(Args...), A>> shared_ptr;
    return  shared_ptr (new scoped_connection<R(Args...), A> (_signal, fn, top));
}

// =========================================================

template <typename R,
          typename... Args,
          slot_allocator A,
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

template <lambda_non_capture Call,
          typename R,
          typename... Args,
          slot_allocator A,
          slot_allocator SharedA
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

template <lambda_capture Call,
          typename R,
          typename... Args,
          slot_allocator A,
          slot_allocator SharedA,
          slot_allocator CallableA
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
          slot_allocator A,
          slot_allocator SharedA
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

template <structure   C,
          typename    R,
          typename... Args,
          slot_allocator A,
          slot_allocator SharedA
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

template <structure   C,
          typename    R,
          typename... Args,
          slot_allocator A,
          slot_allocator SharedA
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

template <typename    R,
          typename... Args,
          slot_allocator A,
          slot_allocator SharedA
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
