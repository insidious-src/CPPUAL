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

#ifndef CPPUAL_SIGNAL_H_
#define CPPUAL_SIGNAL_H_
#ifdef __cplusplus

#include <cppual/circular_queue.h>
#include <cppual/functional.h>

#include <vector>

namespace cppual {

template <typename T,
          typename = std::allocator< Function<T> >
          >
class Signal;

template <typename T,
          typename = std::allocator< Function<T> >
          >
class ScopedConnection;

// =========================================================

template <typename TRetType, typename... Args, typename Allocator>
class Signal <TRetType(Args...), Allocator>
{
public:
    typedef Allocator                                    allocator_type;
    typedef DelegateType<typename Allocator::value_type> value_type;
    typedef CircularQueue<value_type, allocator_type>    container_type;
    typedef typename container_type::size_type           size_type;
    typedef typename container_type::reference           reference;
    typedef typename container_type::const_reference     const_reference;
    typedef typename container_type::iterator            iterator;
    typedef typename container_type::const_iterator      const_iterator;
    typedef typename container_type::const_iterator      slot_type;
    typedef std::vector<TRetType>                        collector_type;

    void clear () noexcept
    { _M_slots.clear (); }

    bool empty () const noexcept
    { return _M_slots.empty (); }

    const_iterator cbegin () const noexcept
    { return _M_slots.cbegin (); }

    iterator begin () noexcept
    { return _M_slots.begin (); }

    const_iterator cend () const noexcept
    { return _M_slots.cend (); }

    iterator end () noexcept
    { return _M_slots.end (); }

    // emit signal to connected slots
    collector_type operator () (Args... args)
    {
        collector_type collection;
        size_type size = _M_slots.size ();

        if (size == 0) return std::move (collection);

        collection.reserve (size);

        for (auto i = 0; i < size; ++i)
        {
            if (_M_slots[i] != nullptr)
                collection.emplace_back
                        (std::move (_M_slots[i] (std::forward<Args> (args)...)));
        }

        return std::move (collection);
    }

    collector_type operator () (Args... args) const
    {
        collector_type collection;
        size_type size = _M_slots.size ();

        if (size == 0) return std::move (collection);

        collection.reserve (size);

        for (auto i = 0; i < size; ++i)
        {
            if (_M_slots[i] != nullptr)
                collection.emplace_back
                        (std::move (_M_slots[i] (std::forward<Args> (args)...)));
        }

        return std::move (collection);
    }

    Signal () noexcept
    : _M_slots ()
    { }

    Signal (allocator_type const& gAlloc) noexcept
    : _M_slots (gAlloc)
    { }

    template <typename Call,
              typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type
    connect (Signal<TRetType_(TArgs_...), Allocator_>&,
             Call&&,
             bool,
             LambdaNonCapturePtr<Call>);

    template <typename Call,
              typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type
    connect (Signal<TRetType_(TArgs_...), Allocator_>&,
             Call&&,
             bool,
             LambdaCapturePtr<Call>);

    template <typename TClass,
              typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type
    connect (Signal<TRetType_(TArgs_...), Allocator_>&,
             ClassType<TClass>&,
             TRetType_ (TClass::*)(TArgs_...),
             bool);

    template <typename TClass,
              typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend void
    disconnect (Signal<TRetType_(TArgs_...), Allocator_>&,
                typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type&);

    template <typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend void
    disconnect (Signal<TRetType_(TArgs_...), Allocator_>&,
                typename Signal<TRetType_(TArgs_...), Allocator_>::const_reference);

private:
    container_type _M_slots;
};

// =========================================================

template <typename... Args, typename Allocator>
class Signal <void(Args...), Allocator>
{
public:
    typedef Allocator                                    allocator_type;
    typedef DelegateType<typename Allocator::value_type> value_type;
    typedef typename Allocator::size_type                size_type;
    typedef typename Allocator::reference                reference;
    typedef typename Allocator::const_reference          const_reference;
    typedef CircularQueue<value_type, allocator_type>    container_type;
    typedef typename container_type::iterator            iterator;
    typedef typename container_type::const_iterator      const_iterator;
    typedef typename container_type::const_iterator      slot_type;
    typedef void                                         collector_type;

    void clear () noexcept
    { _M_slots.clear (); }

    bool empty () const noexcept
    { return _M_slots.empty (); }

    const_iterator cbegin () const noexcept
    { return _M_slots.cbegin (); }

    iterator begin () noexcept
    { return _M_slots.begin (); }

    const_iterator cend () const noexcept
    { return _M_slots.cend (); }

    iterator end () noexcept
    { return _M_slots.end (); }

    // emit signal to connected slots
    void operator () (Args... args)
    {
        auto size = _M_slots.size ();

        for (auto i = 0U; i < size; ++i)
            if (_M_slots[i] != nullptr) _M_slots[i] (std::forward<Args> (args)...);
    }

    void operator () (Args... args) const
    {
        auto size = _M_slots.size ();

        for (auto i = 0U; i < size; ++i)
            if (_M_slots[i] != nullptr) _M_slots[i] (std::forward<Args> (args)...);
    }

    Signal () noexcept
    : _M_slots ()
    { }

    Signal (allocator_type const& gAlloc) noexcept
    : _M_slots (gAlloc)
    { }

    template <typename Call,
              typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type
    connect (Signal<TRetType_(TArgs_...), Allocator_>&,
             Call&&,
             bool,
             LambdaNonCapturePtr<Call>);

    template <typename Call,
              typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type
    connect (Signal<TRetType_(TArgs_...), Allocator_>&,
             Call&&,
             bool,
             LambdaCapturePtr<Call>);

    template <typename TClass,
              typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type
    connect (Signal<TRetType_(TArgs_...), Allocator_>&,
             ClassType<TClass>&,
             TRetType_ (TClass::*)(TArgs_...),
             bool);

    template <typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type
    connect (Signal<TRetType_(TArgs_...), Allocator_>&,
             TRetType_ (*)(TArgs_...),
             bool);

    template <typename TClass,
              typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend void
    disconnect (Signal<TRetType_(TArgs_...), Allocator_>&,
                typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type&);

    template <typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend void
    disconnect (Signal<TRetType_(TArgs_...), Allocator_>&,
                typename Signal<TRetType_(TArgs_...), Allocator_>::const_reference);

private:
    container_type _M_slots;
};

// =========================================================

template <typename Call,
          typename TRetType,
          typename... TArgs,
          typename Allocator
          >
inline
typename Signal<TRetType(TArgs...), Allocator>::slot_type
connect (Signal<TRetType(TArgs...), Allocator>& gSignal,
         Call&& gFunc,
         bool bTop = false,
         LambdaNonCapturePtr<Call> = nullptr)
{
    if (bTop)
    {
        gSignal._M_slots.emplace_front (std::forward<Call> (gFunc));
        return gSignal._M_slots.cbegin ();
    }

    gSignal._M_slots.emplace_back (std::forward<Call> (gFunc));
    return --gSignal._M_slots.cend ();
}

template <typename Call,
          typename TRetType,
          typename... TArgs,
          typename Allocator
          >
inline
typename Signal<TRetType(TArgs...), Allocator>::slot_type
connect (Signal<TRetType(TArgs...), Allocator>& gSignal,
         Call&& gFunc,
         bool bTop = false,
         LambdaCapturePtr<Call> = nullptr)
{
    using AllocatorCallable = typename Allocator::template rebind<CallableType<Call>>::other;

    if (bTop)
    {
        gSignal._M_slots.emplace_front (std::forward<Call> (gFunc), AllocatorCallable());
        return gSignal._M_slots.cbegin ();
    }

    gSignal._M_slots.emplace_back (std::forward<Call> (gFunc), AllocatorCallable());
    return --gSignal._M_slots.cend ();
}

template <typename TClass  ,
          typename TRetType,
          typename... TArgs,
          typename Allocator
          >
inline
typename Signal<TRetType(TArgs...), Allocator>::slot_type
connect (Signal<TRetType(TArgs...), Allocator>& gSignal,
         ClassType<TClass>& pObj,
         TRetType (TClass::* fn)(TArgs...),
         bool bTop = false)
{
    if (bTop)
    {
        gSignal._M_slots.emplace_front (&pObj, fn);
        return --gSignal._M_slots.cbegin ();
    }

    gSignal._M_slots.emplace_back (&pObj, fn);
    return --gSignal._M_slots.cend ();
}

template <typename TRetType,
          typename... TArgs,
          typename Allocator
          >
inline
typename Signal<TRetType(TArgs...), Allocator>::slot_type
connect (Signal<TRetType(TArgs...), Allocator>& gSignal,
         TRetType (* fn)(TArgs...),
         bool bTop = false)
{
    if (bTop)
    {
        gSignal._M_slots.emplace_front (fn);
        return gSignal._M_slots.cbegin ();
    }

    gSignal._M_slots.emplace_back (fn);
    return --gSignal._M_slots.cend ();
}

// =========================================================

template <typename TRetType,
          typename... TArgs,
          typename Allocator
          >
inline
void
disconnect (Signal<TRetType(TArgs...), Allocator>& gSignal,
            typename Signal<TRetType(TArgs...), Allocator>::slot_type& gConn)
{ gSignal._M_slots.erase (gConn); }

template <typename TRetType,
          typename... TArgs,
          typename Allocator
          >
inline
void
disconnect (Signal<TRetType(TArgs...), Allocator>& gSignal,
            typename Signal<TRetType(TArgs...), Allocator>::const_reference fn)
{
    auto it = std::find(gSignal.cbegin(), gSignal.cend(), fn);

    if (it != gSignal.cend()) gSignal._M_slots.erase (it);
}

// =========================================================

template <typename T, typename... TArgs, typename Allocator>
class ScopedConnection <T(TArgs...), Allocator>
{
public:
    typedef Signal<T(TArgs...), Allocator>   signal_type;
    typedef typename signal_type::slot_type  slot_type;
    typedef typename signal_type::value_type value_type;

    ScopedConnection () = delete;
    ScopedConnection (ScopedConnection&&) = default;

    inline ScopedConnection (signal_type& gSignal,
                             value_type&& gFunc,
                             bool         bTop = false)
    : _M_signal (&gSignal),
      _M_fn (std::move (connect (gSignal, std::forward<value_type> (gFunc), bTop)))
    { }

    inline ~ScopedConnection ()
    { if (_M_fn != nullptr) disconnect (*_M_signal, _M_fn); }

private:
    signal_type* _M_signal;
    slot_type    _M_fn    ;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_SIGNAL_H_
