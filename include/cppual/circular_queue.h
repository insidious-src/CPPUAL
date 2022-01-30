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

#ifndef CPPUAL_CIRCULAR_QUEUE_H_
#define CPPUAL_CIRCULAR_QUEUE_H_
#ifdef __cplusplus

#include <cppual/concepts.h>
#include <cppual/iterator.h>
#include <cppual/noncopyable.h>
#include <cppual/memory/allocator.h>

#include <atomic>
#include <memory>
#include <limits>
#include <cstring>
#include <type_traits>
#include <algorithm>

namespace cppual {

// ====================================================

template <typename T,
          typename Allocator = memory::allocator<T>,
          bool     Atomic    = false
          >
class circular_queue : private Allocator
{
public:
    static_assert (!std::is_void<T>::value              , "T is void");
    static_assert ( std::is_move_constructible<T>::value, "T is not move constructible!");
    static_assert ( std::is_move_assignable<T>::value   , "T is not move assignable!");

    typedef std::allocator_traits<Allocator>           allocator_traits      ;
    typedef typename allocator_traits::allocator_type  allocator_type        ;
    typedef T                                          value_type            ;
    typedef value_type*                                pointer               ;
    typedef value_type const*                          const_pointer         ;
    typedef value_type&                                reference             ;
    typedef value_type const&                          const_reference       ;
    typedef typename allocator_traits::size_type       size_type             ;
    typedef typename allocator_traits::size_type const const_size            ;
    typedef typename allocator_traits::difference_type difference_type       ;
    typedef circular_queue<T, allocator_type, Atomic>  self_type             ;
    typedef bidirectional_iterator<self_type>          iterator              ;
    typedef bidirectional_iterator<self_type const>    const_iterator        ;
    typedef std::reverse_iterator<iterator>            reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
    typedef std::pair<pointer, size_type>              array_range           ;
    typedef std::pair<const_pointer, size_type>        const_array_range     ;

    constexpr const static size_type npos = size_type(-1);

    circular_queue& operator = (circular_queue const&);
    void resize   (size_type new_capacity);
    void erase    (iterator&);

    constexpr const_pointer          data    () const noexcept { return _M_pArray; }
    constexpr iterator               begin   () noexcept { return iterator (*this, size_type()); }
    constexpr const_iterator         begin   () const noexcept { return const_iterator (*this, size_type()); }
    constexpr const_iterator         cbegin  () const noexcept { return const_iterator (*this, size_type()); }
    constexpr iterator               end     () noexcept { return iterator (*this, size ()); }
    constexpr const_iterator         end     () const noexcept { return const_iterator (*this, size ()); }
    constexpr const_iterator         cend    () const noexcept { return const_iterator (*this, size ()); }
    constexpr const_reference        front   () const  { return _M_pArray[_M_uBeginPos]; }
    constexpr const_reference        back    () const  { return _M_pArray[_M_uEndPos  ]; }
    constexpr reference              front   ()        { return _M_pArray[_M_uBeginPos]; }
    constexpr reference              back    ()        { return _M_pArray[_M_uEndPos  ]; }
    constexpr allocator_type         get_allocator () const noexcept { return  *this; }
    inline    void                   pop_front     () { if (!empty ()) _pop_front (); }
    inline    void                   pop_back      () { if (!empty ()) _pop_back  (); }

    constexpr reverse_iterator rbegin () noexcept
    { return reverse_iterator (size () ? end () - 1 : end ()); }

    constexpr const_reverse_iterator rbegin () const noexcept
    { return const_reverse_iterator (size () ? end () - 1 : end ()); }

    constexpr const_reverse_iterator crbegin () const noexcept
    { return const_reverse_iterator (size () ? end () - 1 : end ()); }

    constexpr reverse_iterator rend () noexcept
    { return reverse_iterator (begin ()); }

    constexpr const_reverse_iterator rend () const noexcept
    { return const_reverse_iterator (begin ()); }

    constexpr const_reverse_iterator crend () const noexcept
    { return const_reverse_iterator (begin ()); }

    explicit
    circular_queue (size_type             uCapacity = 0U,
                    allocator_type const& gAtor     = allocator_type ())
    : allocator_type (gAtor),
      _M_pArray      (uCapacity ? allocator_type::allocate (uCapacity) : nullptr),
      _M_uBeginPos   (),
      _M_uEndPos     (),
      _M_uCapacity   (_M_pArray ? uCapacity : size_type ())
    { if (_M_pArray && !_M_uCapacity) throw std::bad_array_new_length (); }

    circular_queue (allocator_type const& gAtor)
    : allocator_type (gAtor),
      _M_pArray      (),
      _M_uBeginPos   (),
      _M_uEndPos     (),
      _M_uCapacity   ()
    { }

    circular_queue (std::initializer_list<value_type> list)
    : allocator_type (),
      _M_pArray      (allocator_type::allocate (list.size())),
      _M_uBeginPos   (),
      _M_uEndPos     (),
      _M_uCapacity   (_M_pArray ? list.size() : size_type())
    {
        if (_M_pArray) for (reference val : list) push_back(std::move(val));
    }

    circular_queue (circular_queue const& gObj)
    : allocator_type (gObj),
      _M_pArray      (!gObj.empty () ? allocator_type::allocate (gObj.size ()) : pointer ()),
      _M_uBeginPos   (),
      _M_uEndPos     (_M_pArray ? gObj.size () - 1 : _M_uBeginPos),
      _M_uCapacity   (_M_pArray ? _M_uEndPos   + 1 : _M_uBeginPos)
    {
        if (!gObj.empty () and !_M_pArray) throw std::bad_array_new_length ();
        std::copy (gObj.cbegin (), gObj.cend (), begin ());
    }

    circular_queue (circular_queue&& gObj) noexcept
    : allocator_type (gObj),
      _M_pArray      (gObj._M_pArray),
      _M_uBeginPos   (gObj._M_uBeginPos),
      _M_uEndPos     (gObj._M_uEndPos),
      _M_uCapacity   (gObj._M_uCapacity)
    {
        gObj._M_pArray    = nullptr;
        gObj._M_uBeginPos = size_type();
        gObj._M_uEndPos   = size_type();
        gObj._M_uCapacity = size_type();
    }

    template <typename Iterator,
              typename = typename std::enable_if<is_iterator<Iterator>::value>::type
              >
    circular_queue (Iterator gBegin, Iterator gEnd,
                    allocator_type const&     gAtor = allocator_type ())
    : allocator_type (gAtor),
      _M_pArray      (diff (gEnd, gBegin) ? allocator_type::allocate (diff (gEnd, gBegin)) : nullptr),
      _M_uBeginPos   (),
      _M_uEndPos     (diff (gEnd, gBegin) - 1),
      _M_uCapacity   (_M_pArray ? diff (gEnd, gBegin) : size_type())
    {
        if (_M_pArray && _M_uCapacity) std::copy (gBegin, gEnd, begin ());
    }

    ~circular_queue () noexcept
    {
        if  (!capacity()) return;

        clear ();
        allocator_type::deallocate (_M_pArray, _M_uCapacity);
    }

    circular_queue& operator = (circular_queue&& gObj)
    {
        if (this == &gObj) return *this;

        swap (gObj);
        gObj.dispose();
        return *this;
    }

    reference operator [] (size_type n)
    { return _M_pArray[index_to_subscript (n)];  }

    constexpr const_reference operator [] (size_type n) const
    { return _M_pArray[index_to_subscript (n)];  }

    constexpr size_type capacity () const noexcept
    { return _M_uCapacity; }

    inline size_type max_size () const noexcept
    { return capacity() - size(); }

    array_range array_one () noexcept
    {
        if (!capacity()) return array_range ();
        return array_range (_M_pArray + _M_uBeginPos, is_linearized () ?
                                size () : capacity () - _M_uBeginPos);
    }

    constexpr const_array_range array_one () const noexcept
    {
        if (!capacity()) return const_array_range ();
        return const_array_range (_M_pArray + _M_uBeginPos, is_linearized () ?
                                      size () : capacity () - _M_uBeginPos);
    }

    array_range array_two () noexcept
    {
        if (!capacity()) return array_range ();
        return !is_linearized () ? array_range (_M_pArray + (_M_uEndPos - 1), _M_uEndPos) :
                                   array_range ();
    }

    const_array_range array_two () const noexcept
    {
        if (!capacity()) return const_array_range ();
        return !is_linearized () ? const_array_range (_M_pArray + (_M_uEndPos - 1), _M_uEndPos) :
                                   const_array_range ();
    }

    constexpr size_type size () const noexcept
    {
        return !empty () ? (((_M_uEndPos - _M_uBeginPos) + capacity()) % capacity()) + 1 : size_type ();
    }

    constexpr bool is_linearized () const noexcept
    { return _M_uBeginPos <= _M_uEndPos; }

    constexpr bool empty () const noexcept
    { return _M_uBeginPos == _M_uEndPos; }

    constexpr bool full () const noexcept
    { return _M_uBeginPos == normalize (_M_uEndPos + 1); }

    void reserve (size_type uNewCapacity)
    { if (uNewCapacity > capacity ()) resize (uNewCapacity); }

    void shrink_to_fit ()
    { if (capacity () != size ()) swap (self_type (*this)); }

    void push_back (value_type&& val)
    { emplace_back (std::move(val)); }

    void push_back (value_type const& val)
    { emplace_back (val); }

    void push_front (value_type&& val)
    { emplace_front (std::move(val)); }

    void push_front (value_type const& val)
    { emplace_front (val); }

    const_reference at (size_type n) const
    { return size () > n ? (*this)[n] : throw std::out_of_range ("index is out of range"); }

    reference at (size_type n)
    { return size () > n ? (*this)[n] : throw std::out_of_range ("index is out of range"); }

    template <typename... Args>
    void emplace_front (Args&&... args)
    {
        // ++front--
        // no storage allocated or full
        if (!capacity () || full ())
        {
            resize (capacity () + 1);
        }

        size_type uNewBegin = _M_uBeginPos ? _M_uBeginPos - 1 : capacity () ? capacity () - 1 : size_type ();

        // add element
        allocator_traits::construct (*this, &_M_pArray[uNewBegin], std::forward<Args> (args)...);
        _M_uBeginPos = uNewBegin;
    }

    template <typename... Args>
    void emplace_back (Args&&... args)
    {
        // --back++
        // no storage allocated or full
        if (!capacity () /*|| full ()*/)
        {
            resize (capacity () + 1);
        }

        // add element
        allocator_traits::construct (*this, &_M_pArray[_M_uEndPos], std::forward<Args> (args)...);
        _M_uEndPos = normalize (_M_uEndPos + 1);
    }

    void swap (self_type& gObj) noexcept
    {
        std::swap (static_cast<allocator_type&> (*this),
                   static_cast<allocator_type&> (gObj));

        std::swap (_M_pArray,    gObj._M_pArray   );
        std::swap (_M_uBeginPos, gObj._M_uBeginPos);
        std::swap (_M_uEndPos,   gObj._M_uEndPos  );
        std::swap (_M_uCapacity, gObj._M_uCapacity);
    }

    void clear ()
    {
        auto i = size ();

        while (i > 0) allocator_traits::destroy (*this, &(*this)[--i]);
        _M_uBeginPos = _M_uEndPos = size_type ();
    }

private:
    void _pop_front ()
    {
        // ++front--
        allocator_traits::destroy (*this, &_M_pArray[_M_uBeginPos]);
        _M_uBeginPos = normalize (++_M_uBeginPos);
    }

    void _pop_back ()
    {
        // --back++
        allocator_traits::destroy (*this, &_M_pArray[_M_uEndPos]);
        _M_uEndPos = _M_uEndPos ? normalize (_M_uEndPos - 1) : capacity () - 1;
    }

    size_type normalize (size_type uIdx) const noexcept
    { return capacity() ? uIdx % capacity () : size_type (); }

    size_type index_to_subscript (size_type uIdx) const noexcept
    { return capacity() ? (_M_uBeginPos + uIdx) % capacity () : size_type (); }

    template <typename Iterator>
    constexpr static size_type diff (Iterator i1, Iterator i2) noexcept
    { return i1 >= i2 ? size_type (i1 - i2) : size_type (i2 - i1); }

    void dispose ();

private:
    pointer   _M_pArray    { };
    size_type _M_uBeginPos { }, _M_uEndPos { }, _M_uCapacity { };
};

// ====================================================

template <typename T, typename Allocator, bool Atomic>
circular_queue<T, Allocator, Atomic>&
circular_queue<T, Allocator, Atomic>::operator = (circular_queue const& gObj)
{
    if (this == &gObj) return *this;

    allocator_type::operator = (gObj);
    size_type new_size = gObj.size ();

    if (capacity () < new_size)
    {
        dispose  ();
        _M_pArray     = allocator_type::allocate (new_size);
        _M_uCapacity  = new_size;
    }
    else _M_uBeginPos = size_type ();

    _M_uEndPos = index_to_subscript (--new_size);
    std::copy (gObj.cbegin (), gObj.cend (), begin ());
    return *this;
}

template <typename T, typename Allocator, bool Atomic>
void circular_queue<T, Allocator, Atomic>::resize (size_type uNewCapacity)
{
    if (capacity () >= uNewCapacity) return;

    self_type gObj (uNewCapacity, *this);

    for (reference elem : *this) gObj.push_back(std::move(elem));

    swap(gObj);
}

template <typename T, typename Allocator, bool Atomic>
void circular_queue<T, Allocator, Atomic>::erase (iterator& gIt)
{
    if (empty () || gIt < begin () || end () <= gIt)
        throw std::out_of_range ("iterator is out of range");

    auto const pos = index_to_subscript (gIt.pos ());

    if (pos == _M_uBeginPos) _pop_front ();
    else if (pos == index_to_subscript (size () - 1)) _pop_back ();
    else
    {
        allocator_traits::destroy (*this, &(*gIt));

        if (end() - gIt <= gIt - begin())
        {
            std::move(gIt + 1, end(), gIt);
            _M_uEndPos = _M_uEndPos ? _M_uEndPos - 1 : capacity() - 1;
        }
        else
        {
            std::move_backward(begin(), gIt, gIt + 1);
            _M_uBeginPos = normalize (++_M_uBeginPos);
        }
    }
}

template <typename T, typename Allocator, bool Atomic>
void circular_queue<T, Allocator, Atomic>::dispose ()
{
    if (!capacity()) return;

    clear ();
    allocator_type::deallocate (_M_pArray, capacity ());
    _M_uCapacity = size_type ();
    _M_pArray    = pointer   ();
}

// ====================================================

// lock-free circular queue (1 producer / 1 consumer)
// ex. sufficient for event handling
template <typename T, std::size_t N>
class uniform_queue : non_copyable
{
public:
    static_assert (!std::is_void<T>::value              , "T is void");
    static_assert ( std::is_move_constructible<T>::value, "T is not move constructible!");
    static_assert ( std::is_move_assignable<T>::value   , "T is not move assignable!");

    typedef T                   value_type     ;
    typedef T*                  pointer        ;
    typedef T const*            const_pointer  ;
    typedef T&                  reference      ;
    typedef T const&            const_reference;
    typedef std::size_t         size_type      ;
    typedef std::size_t const   const_size     ;
    typedef std::ptrdiff_t      difference_type;
    typedef uniform_queue<T, N> self_type      ;

    inline size_type size () const noexcept
    {
        const_size uBeginPos = _M_uReadPos.load  (std::memory_order_relaxed);
        const_size uEndPos   = _M_uWritePos.load (std::memory_order_relaxed);

        return (uEndPos - uBeginPos + capacity()) % capacity();
    }

    constexpr bool is_linearized () const noexcept
    {
        return _M_uReadPos .load (std::memory_order_relaxed) <=
               _M_uWritePos.load (std::memory_order_relaxed);
    }

    constexpr static size_type capacity () noexcept
    { return N; }

    bool is_lock_free () const noexcept
    { return _M_uWritePos.is_lock_free () and _M_uReadPos.is_lock_free (); }

    bool empty () const noexcept
    { return _M_uReadPos == _M_uWritePos; }

    bool full () const noexcept
    { return ((_M_uWritePos.load () + 1) % capacity ()) == _M_uReadPos.load (); }

    void push_back (value_type const& value)
    { push_back (const_cast<value_type&> (value)); }

    void emplace_back (value_type const& value)
    { push_back (const_cast<value_type&> (value)); }

    void emplace_back (value_type&& value)
    { push_back (std::forward<value_type> (value)); }

    template <typename... Args>
    void emplace_back (Args&&... args)
    { push_back (std::forward<Args> (args)...); }

    constexpr uniform_queue () noexcept
    : _M_uReadPos (), _M_uWritePos ()
    { }

    template <typename Iterator>
    uniform_queue  (InputIteratorType<Iterator> gBegin, InputIteratorType<Iterator> gEnd) noexcept
    : _M_uReadPos  (),
      _M_uWritePos (size_type (gEnd - gBegin) > N ? N : size_type (gEnd - gBegin))
    { std::copy (gBegin, gBegin + _M_uWritePos, _M_Array); }

    void push_back (value_type&& value)
    {
        const_size uCurWrite  = _M_uWritePos.load (std::memory_order_relaxed);
        const_size uNextWrite = next (uCurWrite);

        if (uNextWrite != _M_uReadPos.load (std::memory_order_acq_rel))
        {
            new (&_M_Array[uCurWrite]) T (std::forward<value_type> (value));
            _M_uWritePos = uNextWrite;
        }
        else _M_Array[uCurWrite] = std::forward<value_type> (value);
    }

    template <typename... Args>
    void push_back (Args&&... args)
    {
        const_size uCurWrite  = _M_uWritePos.load (std::memory_order_relaxed);
        const_size uNextWrite = next (uCurWrite);

        if (uNextWrite != _M_uReadPos.load (std::memory_order_acq_rel))
        {
            new (&_M_Array[uCurWrite]) T (std::forward<Args> (args)...);
            _M_uWritePos = uNextWrite;
        }
        else
        {
            _M_Array[uCurWrite].~value_type ();
            new (&_M_Array[uCurWrite]) T (std::forward<Args> (args)...);
        }
    }

    bool pop_front (T& elem)
    {
        const_size uCurRead = _M_uReadPos.load (std::memory_order_relaxed);

        if (uCurRead == _M_uWritePos.load (std::memory_order_acq_rel))
            return false; // empty

        elem = _M_Array[uCurRead];
        _M_Array[uCurRead].~value_type ();

        _M_uReadPos = next (uCurRead);
        return true;
    }

private:
    static size_type next (size_type uIdx) noexcept
    { return ++uIdx % N; }

private:
    std::atomic_size_t _M_uReadPos, _M_uWritePos;
    T                  _M_Array[capacity ()];
};

// ====================================================

// feature complete lock-free multi-producer/multi-consumer bidirectional queue
// ex. game messaging queue
template <typename T, class Allocator>
class circular_queue <T, Allocator, true> : Allocator, non_copyable
{
public:
    static_assert (!std::is_void<T>::value              , "T is void");
    static_assert ( std::is_move_constructible<T>::value, "T is not move constructible!");
    static_assert ( std::is_move_assignable<T>::value   , "T is not move assignable!");

    typedef std::allocator_traits<Allocator>           allocator_traits;
    typedef typename allocator_traits::allocator_type  allocator_type  ;
    typedef T                                          value_type      ;
    typedef T*                                         pointer         ;
    typedef T const*                                   const_pointer   ;
    typedef T&                                         reference       ;
    typedef T const&                                   const_reference ;
    typedef std::atomic_size_t                         atomic_size     ;
    typedef typename allocator_traits::size_type       size_type       ;
    typedef typename allocator_traits::size_type const const_size      ;
    typedef typename allocator_traits::difference_type difference_type ;
    typedef circular_queue<T, Allocator, true>         self_type       ;

    circular_queue () noexcept;

    void reserve (size_type);
    void reserve_unsafe (size_type);
    bool push_back (const_reference);
    bool bounded_push_back (const_reference);
    bool unsynchronized_push_back (const_reference);
    bool pop_front (reference);
    bool unsynchronized_pop_front (reference);

    inline size_type size () const noexcept
    {
        const_size uBeginPos = _M_uBeginPos.load (std::memory_order_relaxed);
        const_size uEndPos   = _M_uEndPos.load (std::memory_order_relaxed);

        return (uEndPos - uBeginPos + capacity()) % capacity();
    }

    constexpr bool is_linearized () const noexcept
    {
        return _M_uBeginPos.load (std::memory_order_relaxed) <=
               _M_uEndPos.load (std::memory_order_relaxed);
    }

    constexpr size_type capacity () noexcept
    { return _M_uCapacity; }

    bool is_lock_free () const noexcept
    { return _M_uEndPos.is_lock_free () and _M_uBeginPos.is_lock_free (); }

    bool empty () const noexcept
    { return _M_uBeginPos == _M_uEndPos; }

    bool full () const noexcept
    { return ((_M_uEndPos.load () + 1) % capacity ()) == _M_uBeginPos.load (); }

    template <typename Functor>
    bool consume_one (Functor& fn)
    {
        value_type element;
        bool       success = pop_front (element);

        if (success) fn (element);
        return success;
    }

    template <typename Functor>
    bool consume_one (Functor const& fn)
    {
        value_type element;
        bool       success = pop_front (element);

        if (success) fn (element);
        return success;
    }

    template <typename Functor>
    size_type consume_all (Functor& fn)
    {
        size_type element_count = 0;

        for (value_type element; pop_front (element); ++element_count) fn (element);
        return element_count;
    }

    template <typename Functor>
    size_type consume_all (Functor const& fn)
    {
        size_type element_count = 0;

        for (value_type element; pop_front (element); ++element_count) fn (element);
        return element_count;
    }

private:
    pointer     _M_pArray;
    atomic_size _M_uBeginPos, _M_uEndPos;
    size_type   _M_uCapacity;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_CIRCULAR_QUEUE_H_
