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
#include <memory>

namespace cppual {

// =========================================================

template <typename T>
using SignalAllocator = std::allocator< Function<T> >;

template <typename T,
          typename Allocator = SignalAllocator<T>
          >
using SignalQueue = CircularQueue<T, Allocator>;

// =========================================================

template <typename T,
          typename = SignalAllocator<T>
          >
class Signal;

template <typename T,
          typename = SignalAllocator<T>
          >
class ScopedConnection;

// =========================================================

template <typename T, typename... Args, typename Allocator>
class Signal <T(Args...), Allocator>
{
public:
    typedef Allocator                                    allocator_type;
    typedef DelegateType<typename Allocator::value_type> value_type;
    typedef SignalQueue<value_type, allocator_type>      container_type;
    typedef typename container_type::size_type           size_type;
    typedef typename container_type::reference           reference;
    typedef typename container_type::const_reference     const_reference;
    typedef typename container_type::iterator            iterator;
    typedef typename container_type::const_iterator      const_iterator;
    typedef typename container_type::const_iterator      slot_type;
    typedef ScopedConnection<T(Args...), Allocator>      scoped_connection_type;

    template <typename CollectorAllocator>
    using collector_type = std::vector<T, CollectorAllocator>;

    void clear () noexcept
    { _M_slots.clear (); }

    bool empty () const noexcept
    { return _M_slots.empty (); }

    size_type size () const noexcept
    { return _M_slots.size (); }

    const_iterator cbegin () const noexcept
    { return _M_slots.cbegin (); }

    iterator begin () noexcept
    { return _M_slots.begin (); }

    const_iterator begin () const noexcept
    { return _M_slots.begin (); }

    const_iterator cend () const noexcept
    { return _M_slots.cend (); }

    iterator end () noexcept
    { return _M_slots.end (); }

    const_iterator end () const noexcept
    { return _M_slots.end (); }

    //! emit signal to connected slots
    template <typename CollectorAllocator = std::allocator<T> >
    collector_type<CollectorAllocator> operator () (Args... args) const
    {
        static_assert (std::is_same<typename CollectorAllocator::value_type, T>::value,
                       "allocator value type is not the same as the return type");

        collector_type<CollectorAllocator> collection;
        size_type size = _M_slots.size ();

        if (size == 0) return std::move (collection);

        collection.reserve (size);

        for (size_type i = 0U; i < size; ++i)
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

    template <typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type
    connect (Signal<TRetType_(TArgs_...), Allocator_>&,
             typename Signal<TRetType_(TArgs_...), Allocator_>::const_reference,
             bool);

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

    template <typename TClass,
              typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend void
    disconnect (Signal<TRetType_(TArgs_...), Allocator_>&,
                ClassType<TClass>&,
                TRetType_ (TClass::*)(TArgs_...));

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
    typedef SignalQueue<value_type, allocator_type>      container_type;
    typedef typename Allocator::size_type                size_type;
    typedef typename Allocator::reference                reference;
    typedef typename Allocator::const_reference          const_reference;
    typedef typename container_type::iterator            iterator;
    typedef typename container_type::const_iterator      const_iterator;
    typedef typename container_type::const_iterator      slot_type;
    typedef void                                         collector_type;
    typedef ScopedConnection<void(Args...), Allocator>   scoped_connection_type;

    void clear () noexcept
    { _M_slots.clear (); }

    bool empty () const noexcept
    { return _M_slots.empty (); }

    size_type size () const noexcept
    { return _M_slots.size (); }

    const_iterator cbegin () const noexcept
    { return _M_slots.cbegin (); }

    iterator begin () noexcept
    { return _M_slots.begin (); }

    const_iterator begin () const noexcept
    { return _M_slots.begin (); }

    const_iterator cend () const noexcept
    { return _M_slots.cend (); }

    iterator end () noexcept
    { return _M_slots.end (); }

    const_iterator end () const noexcept
    { return _M_slots.end (); }

    //! emit signal to connected slots
    void operator () (Args... args) const
    {
        for (size_type i = 0U; i < _M_slots.size(); ++i)
            if (_M_slots[i] != nullptr) _M_slots[i] (std::forward<Args> (args)...);
    }

    Signal () noexcept
    : _M_slots ()
    { }

    Signal (allocator_type const& gAlloc) noexcept
    : _M_slots (gAlloc)
    { }

    template <typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend typename Signal<TRetType_(TArgs_...), Allocator_>::slot_type
    connect (Signal<TRetType_(TArgs_...), Allocator_>&,
             typename Signal<TRetType_(TArgs_...), Allocator_>::const_reference,
             bool);

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

    template <typename TClass,
              typename TRetType_,
              typename... TArgs_,
              typename Allocator_
              >
    friend void
    disconnect (Signal<TRetType_(TArgs_...), Allocator_>&,
                ClassType<TClass>&,
                TRetType_ (TClass::*)(TArgs_...));

private:
    container_type _M_slots;
};

// =========================================================

template <typename TRetType,
          typename... TArgs,
          typename Allocator
          >
typename Signal<TRetType(TArgs...), Allocator>::slot_type
connect (Signal<TRetType(TArgs...), Allocator>& gSignal,
         typename Signal<TRetType(TArgs...), Allocator>::const_reference val,
         bool bTop = false)
{
    if (bTop)
    {
        gSignal._M_slots.push_front (val);
        return gSignal._M_slots.cbegin ();
    }

    gSignal._M_slots.push_back (val);
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
    if (bTop)
    {
        gSignal._M_slots.emplace_front (std::forward<Call> (gFunc));
        return gSignal._M_slots.cbegin ();
    }

    gSignal._M_slots.emplace_back (std::forward<Call> (gFunc));
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
            typename Signal<TRetType(TArgs...), Allocator>::slot_type& it)
{
    if (it != gSignal.end())
    {
        gSignal._M_slots.erase(it);
    }
}

template <typename TRetType,
          typename... TArgs,
          typename Allocator
          >
inline
void
disconnect (Signal<TRetType(TArgs...), Allocator>& gSignal,
            typename Signal<TRetType(TArgs...), Allocator>::const_reference fn)
{
    auto it = std::find(gSignal.begin(), gSignal.end(), fn);

    if (it != gSignal.end())
    {
        gSignal._M_slots.erase(it);
    }
}

template <typename TClass  ,
          typename TRetType,
          typename... TArgs,
          typename Allocator
          >
inline
void
disconnect (Signal<TRetType(TArgs...), Allocator>& gSignal,
            ClassType<TClass>& pObj,
            TRetType (TClass::* fn)(TArgs...))
{
    using value_type = typename Signal<TRetType(TArgs...), Allocator>::value_type;

    auto it = std::find(gSignal.begin(), gSignal.end(), value_type(&pObj, fn));

    if (it != gSignal.end())
    {
        gSignal._M_slots.erase(it);
    }
}

// =========================================================

template <typename T, typename... TArgs, typename Allocator>
class ScopedConnection <T(TArgs...), Allocator>
{
public:
    typedef Signal<T(TArgs...), Allocator>   signal_type;
    typedef signal_type&                     reference  ;
    typedef signal_type*                     pointer    ;
    typedef typename signal_type::value_type value_type ;

    ScopedConnection () = delete;
    ScopedConnection (ScopedConnection&&) noexcept = default;

    //! move constructor
    inline ScopedConnection (reference gSignal, value_type&& gFunc, bool bTop = false)
    : _M_signal (&gSignal),
      _M_fn (std::forward<value_type> (gFunc))
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! copy constructor
    inline ScopedConnection (reference gSignal, value_type const& gFunc, bool bTop = false)
    : _M_signal (&gSignal),
      _M_fn (gFunc)
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! non capture lambda constructor
    template <typename Call>
    inline ScopedConnection (reference gSignal,
                             Call&& fn,
                             bool bTop = false,
                             LambdaNonCapturePtr<Call> = nullptr)
    : _M_signal (&gSignal),
      _M_fn (std::forward<Call> (fn))
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! capture lambda constructor
    template <typename Call>
    inline ScopedConnection (reference gSignal,
                             Call&& fn,
                             bool bTop = false,
                             LambdaCapturePtr<Call> = nullptr)
    : _M_signal (&gSignal),
      _M_fn (std::forward<Call> (fn))
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! member function constructor
    template <typename TClass>
    inline ScopedConnection (reference gSignal,
                             ClassType<TClass>& pObj,
                             T (TClass::* fn)(TArgs...),
                             bool bTop = false)
    : _M_signal (&gSignal),
      _M_fn (&pObj, fn)
    {
        connect (gSignal, _M_fn, bTop);
    }

    //! static function constructor
    inline ScopedConnection (reference gSignal, T (* fn)(TArgs...), bool bTop = false)
    : _M_signal (&gSignal),
      _M_fn (fn)
    {
        connect (gSignal, _M_fn, bTop);
    }

    inline ~ScopedConnection ()
    {
        if (_M_fn != nullptr && _M_signal != nullptr) disconnect (*_M_signal, _M_fn);
    }

private:
    pointer    _M_signal;
    value_type _M_fn    ;
};

// =========================================================

template <typename T,
          typename... TArgs,
          typename Allocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
make_shared_scoped_connection (Signal<T(TArgs...), Allocator>& signal,
                               typename Signal<T(TArgs...), Allocator>::value_type&& fn,
                               bool top = false)
{
    return new ScopedConnection<T(TArgs...), Allocator>(signal, fn, top);
}

template <typename Call,
          typename T,
          typename... TArgs,
          typename Allocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
make_shared_scoped_connection (Signal<T(TArgs...), Allocator>& signal,
                               Call&& fn,
                               bool top = false,
                               LambdaNonCapturePtr<Call> = nullptr)
{
    return new ScopedConnection<T(TArgs...), Allocator>(signal, std::forward<Call>(fn), top);
}

template <typename Call,
          typename T,
          typename... TArgs,
          typename Allocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
make_shared_scoped_connection (Signal<T(TArgs...), Allocator>& signal,
                               Call&& fn,
                               bool top = false,
                               LambdaCapturePtr<Call> = nullptr)
{
    return new ScopedConnection<T(TArgs...), Allocator>(signal, std::forward<Call>(fn), top);
}

template <typename T,
          typename... TArgs,
          typename Allocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
make_shared_scoped_connection (Signal<T(TArgs...), Allocator>& signal,
                               typename Signal<T(TArgs...), Allocator>::const_reference fn,
                               bool top = false)
{
    return new ScopedConnection<T(TArgs...), Allocator>(signal, fn, top);
}

template <typename TClass,
          typename T,
          typename... TArgs,
          typename Allocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
make_shared_scoped_connection (Signal<T(TArgs...), Allocator>& signal,
                               ClassType<TClass>& pObj,
                               T (TClass::* fn)(TArgs...),
                               bool top = false)
{
    return new ScopedConnection<T(TArgs...), Allocator>(signal, pObj, fn, top);
}

template <typename T,
          typename... TArgs,
          typename Allocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
make_shared_scoped_connection (Signal<T(TArgs...), Allocator>& signal,
                               T (* fn)(TArgs...),
                               bool top = false)
{
    return new ScopedConnection<T(TArgs...), Allocator>(signal, fn, top);
}

// =========================================================

template <typename T,
          typename... TArgs,
          typename Allocator,
          typename SharedAllocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
allocate_shared_scoped_connection (SharedAllocator alloc,
                                   Signal<T(TArgs...), Allocator>& signal,
                                   typename Signal<T(TArgs...), Allocator>::value_type&& fn,
                                   bool top = false)
{
    return std::allocate_shared (alloc,
                                 std::move(ScopedConnection<T(TArgs...), Allocator>(signal, fn, top)));
}

template <typename Call,
          typename T,
          typename... TArgs,
          typename Allocator,
          typename SharedAllocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
allocate_shared_scoped_connection (SharedAllocator alloc,
                                   Signal<T(TArgs...), Allocator>& signal,
                                   Call&& fn,
                                   bool top = false,
                                   LambdaNonCapturePtr<Call> = nullptr)
{
    return std::allocate_shared (alloc,
                                 std::move(ScopedConnection<T(TArgs...), Allocator>
                                           (signal, std::forward<Call>(fn), top)));
}

template <typename Call,
          typename T,
          typename... TArgs,
          typename Allocator,
          typename SharedAllocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
allocate_shared_scoped_connection (SharedAllocator alloc,
                                   Signal<T(TArgs...), Allocator>& signal,
                                   Call&& fn,
                                   bool top = false,
                                   LambdaCapturePtr<Call> = nullptr)
{
    return std::allocate_shared (alloc,
                                 std::move(ScopedConnection<T(TArgs...), Allocator>
                                           (signal, std::forward<Call>(fn), top)));
}

template <typename T,
          typename... TArgs,
          typename Allocator,
          typename SharedAllocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
allocate_shared_scoped_connection (SharedAllocator alloc,
                                   Signal<T(TArgs...), Allocator>& signal,
                                   typename Signal<T(TArgs...), Allocator>::const_reference fn,
                                   bool top = false)
{
    return std::allocate_shared (alloc,
                                 std::move(ScopedConnection<T(TArgs...), Allocator>(signal, fn, top)));
}

template <typename TClass,
          typename T,
          typename... TArgs,
          typename Allocator,
          typename SharedAllocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
allocate_shared_scoped_connection (SharedAllocator alloc,
                                   Signal<T(TArgs...), Allocator>& signal,
                                   ClassType<TClass>& pObj,
                                   T (TClass::* fn)(TArgs...),
                                   bool top = false)
{
    return std::allocate_shared (alloc,
                                 std::move(ScopedConnection<T(TArgs...), Allocator>
                                           (signal, pObj, fn, top)));
}

template <typename T,
          typename... TArgs,
          typename Allocator,
          typename SharedAllocator
          >
std::shared_ptr<ScopedConnection<T(TArgs...), Allocator> >
allocate_shared_scoped_connection (SharedAllocator alloc,
                                   Signal<T(TArgs...), Allocator>& signal,
                                   T (* fn)(TArgs...),
                                   bool top = false)
{
    return std::allocate_shared (alloc,
                                 std::move(ScopedConnection<T(TArgs...), Allocator>(signal, fn, top)));
}

} // cppual

#endif // __cplusplus
#endif // CPPUAL_SIGNAL_H_
