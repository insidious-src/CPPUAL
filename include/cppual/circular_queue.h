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

#ifndef CPPUAL_CIRCULAR_QUEUE_H_
#define CPPUAL_CIRCULAR_QUEUE_H_
#ifdef __cplusplus

#include <cppual/memory_resource>
#include <cppual/noncopyable>
#include <cppual/meta_type>
#include <cppual/concepts>
#include <cppual/iterator>

#include <atomic>
#include <memory>
#include <cstring>
#include <iterator>
#include <algorithm>
#include <type_traits>

namespace cppual {

// ====================================================

using memory::allocator_like;

// ====================================================

template <non_void    T,
          allocator_like A = memory::allocator<T>,
          bool   Atomic = false
          >
class SHARED_API circular_queue : private A
{
public:
    static_assert (move_constructible<T>, "T is not move constructible!");
    static_assert (move_assignable   <T>, "T is not move assignable!"   );

    typedef circular_queue<T, A, Atomic>            self_type             ;
    typedef memory::allocator_traits<A>             traits_type           ;
    typedef traits_type::allocator_type             allocator_type        ;
    typedef remove_cref_t<T>                        value_type            ;
    typedef value_type *                            pointer               ;
    typedef value_type const*                       const_pointer         ;
    typedef value_type &                            reference             ;
    typedef value_type const&                       const_reference       ;
    typedef traits_type::size_type                  size_type             ;
    typedef size_type const                         const_size            ;
    typedef traits_type::difference_type            difference_type       ;
    typedef bi_iterator<self_type>                  iterator              ;
    typedef bi_iterator<self_type const>            const_iterator        ;
    typedef std::reverse_iterator<iterator>         reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
    typedef std::pair<pointer, size_type>           array_range           ;
    typedef std::pair<const_pointer, size_type>     const_array_range     ;
    typedef std::pair<iterator, bool>               iterator_pair         ;

    inline constexpr static size_type const npos = size_type (-1);

    self_type& operator = (self_type const&);

    void resize (size_type new_capacity);
    void erase  (const_iterator it);

    constexpr const_pointer   data    () const noexcept { return  _M_pArray  ; }
    constexpr const_reference front   () const noexcept { return *_M_beginPos; }
    constexpr const_reference back    () const noexcept { return *_M_endPos  ; }
    constexpr reference       front   ()       noexcept { return *_M_beginPos; }
    constexpr reference       back    ()       noexcept { return *_M_endPos  ; }

    constexpr allocator_type  get_allocator () const noexcept { return  *this; }
    inline    void            pop_front     () { if (!empty ()) _pop_front (); }
    inline    void            pop_back      () { if (!empty ()) _pop_back  (); }

    constexpr iterator begin () noexcept
    { return  iterator (*this, size_type ()); }

    constexpr const_iterator begin () const noexcept
    { return const_iterator (*this, size_type ()); }

    constexpr const_iterator cbegin () noexcept
    { return const_iterator (*this, size_type ()); }

    constexpr const_iterator cbegin () const noexcept
    { return const_iterator (*this, size_type ()); }

    constexpr iterator end () noexcept
    { return iterator (*this, size ()); }

    constexpr const_iterator end () const noexcept
    { return const_iterator (*this, size ()); }

    constexpr const_iterator cend () noexcept
    { return const_iterator (*this, size ()); }

    constexpr const_iterator cend () const noexcept
    { return const_iterator (*this, size ()); }

    constexpr reverse_iterator rbegin () noexcept
    { return reverse_iterator (iterator (*this, size () - 1)); }

    constexpr const_reverse_iterator rbegin () const noexcept
    { return const_reverse_iterator (const_iterator (*this, size () - 1)); }

    constexpr const_reverse_iterator crbegin () noexcept
    { return const_reverse_iterator (const_iterator (*this, size () - 1)); }

    constexpr const_reverse_iterator crbegin () const noexcept
    { return const_reverse_iterator (const_iterator (*this, size () - 1)); }

    constexpr reverse_iterator rend () noexcept
    { return reverse_iterator (iterator (*this, npos)); }

    constexpr const_reverse_iterator rend () const noexcept
    { return const_reverse_iterator (const_iterator (*this, npos)); }

    constexpr const_reverse_iterator crend () noexcept
    { return const_reverse_iterator (const_iterator (*this, npos)); }

    constexpr const_reverse_iterator crend () const noexcept
    { return const_reverse_iterator (const_iterator (*this, npos)); }

    constexpr circular_queue () noexcept
    : allocator_type ()
    , _M_pArray      ()
    , _M_beginPos    ()
    , _M_endPos      ()
    , _M_uCapacity   ()
    { }

    constexpr circular_queue (size_type uCapacity, allocator_type const& gAtor = allocator_type ())
    : allocator_type (gAtor)
    , _M_pArray      (uCapacity ? allocator_type::allocate (uCapacity) : pointer ())
    , _M_beginPos    ()
    , _M_endPos      ()
    , _M_uCapacity   (_M_pArray ? uCapacity : size_type ())
    { if (!_M_pArray && uCapacity) throw std::bad_alloc (); }

    constexpr circular_queue (allocator_type const& gAtor)
    : allocator_type (gAtor)
    , _M_pArray      ()
    , _M_beginPos    ()
    , _M_endPos      ()
    , _M_uCapacity   ()
    { }

    inline circular_queue (std::initializer_list<value_type> list)
    : allocator_type ()
    , _M_pArray      (allocator_type::allocate (list.size ()))
    , _M_beginPos    ()
    , _M_endPos      ()
    , _M_uCapacity   (_M_pArray ? list.size () : size_type ())
    {
        if (_M_pArray && list.size ()) for (auto& val : list) push_back (std::move (val));
    }

    inline circular_queue (self_type const& gObj)
    : allocator_type (gObj.allocator_type::select_on_container_copy_construction ())
    , _M_pArray      (!gObj.empty () ? allocator_type::allocate (gObj.size ()) : pointer ())
    , _M_beginPos    (_M_pArray && !gObj.empty () ? _M_pArray : pointer ())
    , _M_endPos      (_M_pArray && !gObj.empty () ? _M_pArray + (gObj.size () - 1) : pointer ())
    , _M_uCapacity   (_M_pArray && !gObj.empty () ? gObj.size () : size_type ())
    {
        if (!gObj.empty () && !_M_pArray) throw std::bad_alloc ();

        for (iterator it = gObj.begin (), dst_it = begin (); it != gObj.end (); ++it, ++dst_it)
        {
            allocator_type::construct (&(*dst_it), *it);
        }
    }

    inline circular_queue (self_type&& gObj) noexcept
    : allocator_type (gObj)
    , _M_pArray      (gObj._M_pArray   )
    , _M_beginPos    (gObj._M_beginPos )
    , _M_endPos      (gObj._M_endPos   )
    , _M_uCapacity   (gObj._M_uCapacity)
    {
        gObj._M_pArray    = gObj._M_beginPos = gObj._M_endPos = nullptr;
        gObj._M_uCapacity = size_type ();
    }

    template <common_iterator Iterator>
    inline circular_queue (Iterator gBegin, Iterator gEnd,
                           allocator_type const&     gAtor = allocator_type ())
    : allocator_type (gAtor),
      _M_pArray      (diff (gEnd, gBegin) ? allocator_type::allocate (diff (gEnd, gBegin)) : pointer ()),
      _M_beginPos    (diff (gEnd, gBegin) ? _M_pArray : pointer ()),
      _M_endPos      (diff (gEnd, gBegin) ? _M_pArray + (diff (gEnd, gBegin) - 1) : pointer ()),
      _M_uCapacity   (_M_pArray ? diff (gEnd, gBegin) : size_type ())
    {
        if (_M_pArray && _M_uCapacity)
        {
            for (auto dst_it = begin (); gBegin != gEnd; ++gBegin, ++dst_it)
            {
                traits_type::construct (*this, &(*dst_it), *gBegin);
            }
        }
    }

    ~circular_queue () noexcept
    {
        if (!capacity()) return;

        clear ();
        allocator_type::deallocate (_M_pArray, _M_uCapacity);
    }

    inline self_type& operator = (self_type&& gObj)
    {
        if (this == &gObj) return *this;

        swap (gObj);
        gObj.dispose ();
        return *this;
    }

    constexpr reference operator [] (size_type n)
    { return _M_pArray[index_to_subscript (n)]; }

    constexpr const_reference operator [] (size_type n) const
    { return _M_pArray[index_to_subscript (n)]; }

    constexpr size_type capacity () const noexcept
    { return _M_uCapacity; }

    constexpr size_type max_size () const noexcept
    { return capacity () - size (); }

    constexpr array_range array_one () noexcept
    {
        return !empty () ? array_range (_M_beginPos, is_linearized () ?
                                       (index_to_subscript (size () - 1) - index_to_subscript (0)) + 1 :
                                        capacity () - index_to_subscript (0)) :
                           array_range ();
    }

    constexpr const_array_range array_one () const noexcept
    {
        return !empty () ? const_array_range (_M_beginPos, is_linearized () ?
                                             (index_to_subscript (size () - 1) - index_to_subscript (0)) + 1 :
                                              capacity () - index_to_subscript (0)) :
                           const_array_range ();
    }

    constexpr array_range array_two () noexcept
    {
        return !is_linearized () && !empty () ?
                    array_range (_M_pArray, index_to_subscript (size () - 1) + 1) :
                    array_range ();
    }

    constexpr const_array_range array_two () const noexcept
    {
        return !is_linearized () && !empty () ?
                    const_array_range (_M_pArray, index_to_subscript (size () - 1) + 1) :
                    const_array_range ();
    }

    size_type size () const noexcept
    {
        return !empty () && capacity () ?
                    static_cast<size_type> ((((_M_endPos - _M_beginPos) + _capacity()) % _capacity()) + 1) :
                    size_type ();
    }

    constexpr bool is_linearized () const noexcept
    { return _M_beginPos <= _M_endPos; }

    constexpr bool empty () const noexcept
    { return _M_beginPos == nullptr || _M_endPos == nullptr; }

    bool full () const noexcept
    { return _M_beginPos == normalize (_M_endPos + 1); }

    inline void reserve (size_type uNewCapacity)
    { resize (uNewCapacity); }

    inline void shrink_to_fit ()
    { if (capacity () != size ()) swap (self_type (*this)); }

    inline iterator_pair push_back (value_type&& val)
    { return emplace_back (std::move (val)); }

    inline iterator_pair push_back (value_type const& val)
    { return emplace_back (val); }

    inline iterator_pair push_front (value_type&& val)
    { return emplace_front (std::move (val)); }

    inline iterator_pair push_front (value_type const& val)
    { return emplace_front (val); }

    inline const_reference at (size_type n) const
    { return size () > n ? (*this)[n] : throw std::out_of_range ("index is out of range!"); }

    inline reference at (size_type n)
    { return size () > n ? (*this)[n] : throw std::out_of_range ("index is out of range!"); }

    template <typename... Args>
    inline iterator_pair emplace_front (Args&&... args)
    {
        // ++front--
        // no storage allocated or full
        if (!capacity () || full ())
        {
            resize (capacity () + 1);
        }

        if (empty ()) set_first_pos ();
        else _M_beginPos = normalize (--_M_beginPos);

        traits_type::construct (*this, _M_beginPos, std::forward<Args> (args)...);

        return std::pair (begin (), true);
    }

    template <typename... Args>
    inline iterator_pair emplace_back (Args&&... args)
    {
        // --back++
        // no storage allocated or full
        if (!capacity () || full ())
        {
            resize (capacity () + 1);
        }

        if (empty ()) set_first_pos ();
        else _M_endPos = normalize (++_M_endPos);

        traits_type::construct (*this, _M_endPos, std::forward<Args> (args)...);

        return iterator_pair (iterator (*this, empty () ? 0 : size () - 1), true);
    }

    inline void swap (self_type& gObj) noexcept
    {
        std::swap (static_cast<allocator_type&> (*this), static_cast<allocator_type&> (gObj));

        std::swap (_M_pArray,    gObj._M_pArray   );
        std::swap (_M_beginPos,  gObj._M_beginPos );
        std::swap (_M_endPos,    gObj._M_endPos   );
        std::swap (_M_uCapacity, gObj._M_uCapacity);
    }

    inline void clear ()
    {
        auto i = size ();

        while (i-- > 0) traits_type::destroy (*this, &(*this)[i]);
        invalidate_pos ();
    }

private:
    constexpr difference_type _capacity () const noexcept
    { return static_cast<difference_type> (_M_uCapacity); }

    inline void _pop_front ()
    {
        // ++front--
        traits_type::destroy (*this, _M_beginPos);

        if (size () == 1) invalidate_pos ();
        else _M_beginPos = normalize (++_M_beginPos);
    }

    inline void _pop_back ()
    {
        // --back++
        traits_type::destroy (*this, _M_endPos);

        if (size () == 1) invalidate_pos ();
        else _M_endPos = normalize (--_M_endPos);
    }

    inline void invalidate_pos () noexcept
    { _M_beginPos = _M_endPos = pointer (); }

    inline void set_first_pos () noexcept
    { _M_beginPos = _M_endPos = _M_pArray; }

    constexpr pointer normalize (pointer ptr) const noexcept
    {
        return capacity () ?
                    _M_pArray + (((ptr - _M_pArray) + _capacity ()) % _capacity ()) :
                    pointer ();
    }

    constexpr size_type index_to_subscript (size_type uIdx) const noexcept
    {
        return !empty () && capacity () ?
                    static_cast<size_type> (((_M_beginPos + uIdx) - _M_pArray) % _capacity ()) :
                    npos;
    }

    template <common_iterator Iterator>
    constexpr static size_type diff (Iterator i1, Iterator i2) noexcept
    { return i1 >= i2 ? size_type (i1 - i2) : size_type (i2 - i1); }

    void dispose ();

private:
    pointer   _M_pArray    { };
    pointer   _M_beginPos  { };
    pointer   _M_endPos    { };
    size_type _M_uCapacity { };
};

// ====================================================

template <non_void T, allocator_like A, bool Atomic>
circular_queue<T, A, Atomic>::self_type&
circular_queue<T, A, Atomic>::operator = (self_type const& gObj)
{
    if (this == &gObj) return *this;

    auto const old_size = size      ();
    auto const new_size = gObj.size ();
    auto       end_it   = gObj.cend ();

    if (capacity () < new_size)
    {
        self_type new_obj (new_size, gObj);

        for (auto it = begin (); it != end (); ++it) new_obj.push_back (std::move (*it));

        swap (new_obj);
    }
    else if (old_size > new_size)
    {
        auto size_diff = old_size - new_size;

        while (size_diff-- > 0) pop_back ();
    }
    else if (old_size < new_size)
    {
        end_it -= static_cast<difference_type> (new_size - old_size);

        for (auto it = end_it; it != gObj.cend (); ++it) push_back (*it);
    }

    std::copy (gObj.cbegin (), end_it, begin ());

    return *this;
}

template <non_void T, allocator_like A, bool Atomic>
void circular_queue<T, A, Atomic>::resize (size_type uNewCapacity)
{
    if (capacity () >= uNewCapacity) return;

    self_type gObj (uNewCapacity, *this);

    for (auto it = begin (); it != end (); ++it) gObj.push_back (std::move (*it));

    swap (gObj);
}

template <non_void T, allocator_like A, bool Atomic>
void circular_queue<T, A, Atomic>::erase (const_iterator gIt)
{
    if (empty () || gIt < cbegin () || cend () <= gIt)
        throw std::out_of_range ("iterator is out of range!");

    if (&(*gIt) == _M_beginPos) _pop_front ();
    else if (&(*gIt) == _M_endPos) _pop_back ();
    else
    {
        iterator it (gIt);

        if (end () - it <= it - begin ())
        {
            std::move (it + 1, end (), it);

            traits_type::destroy (*this, _M_endPos);

            _M_endPos = normalize (--_M_endPos);
        }
        else
        {
            std::move_backward (begin (), it, it + 1);

            traits_type::destroy (*this, _M_beginPos);

            _M_beginPos = normalize (++_M_beginPos);
        }
    }
}

template <non_void T, allocator_like A, bool Atomic>
void circular_queue<T, A, Atomic>::dispose ()
{
    if (!capacity ()) return;

    clear ();
    allocator_type::deallocate (_M_pArray, capacity ());

    _M_uCapacity = size_type ();
    _M_pArray    = pointer   ();
}

// ====================================================

//! [UNFINISHED] feature complete lock-free multi-producer/multi-consumer bidirectional queue
//! ex. game messaging queue
template <typename T, allocator_like A>
class SHARED_API circular_queue <T, A, true> : private A, public non_copyable
{
public:
    static_assert (!std::is_void_v<T>              , "T is void");
    static_assert ( std::is_move_constructible_v<T>, "T is not move constructible!");
    static_assert ( std::is_move_assignable_v<T>   , "T is not move assignable!");

    typedef circular_queue<T, A, true>   self_type      ;
    typedef std::allocator_traits<A>     traits_type    ;
    typedef traits_type::allocator_type  allocator_type ;
    typedef remove_cref_t<T>       value_type     ;
    typedef value_type *                 pointer        ;
    typedef value_type const*            const_pointer  ;
    typedef value_type &                 reference      ;
    typedef value_type const&            const_reference;
    typedef std::atomic_size_t           atomic_size    ;
    typedef traits_type::size_type       size_type      ;
    typedef traits_type::size_type const const_size     ;
    typedef traits_type::difference_type ifference_type ;

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
        const_size uBeginPos = _M_beginPos.load (std::memory_order_relaxed);
        const_size uEndPos   = _M_endPos.load (std::memory_order_relaxed);

        return ((uEndPos - uBeginPos) + capacity()) % capacity();
    }

    constexpr bool is_linearized () const noexcept
    {
        return _M_beginPos.load (std::memory_order_relaxed) <=
               _M_endPos.load (std::memory_order_relaxed);
    }

    constexpr size_type capacity () noexcept
    { return _M_uCapacity; }

    bool is_lock_free () const noexcept
    { return _M_endPos.is_lock_free () and _M_beginPos.is_lock_free (); }

    inline bool empty () const noexcept
    { return _M_beginPos == _M_endPos; }

    inline bool full () const noexcept
    { return ((_M_endPos.load () + 1) % capacity ()) == _M_beginPos.load (); }

    template <typename F>
    inline bool consume_one (F& fn)
    {
        value_type element;
        bool       success = pop_front (element);

        if (success) fn (element);
        return success;
    }

    template <typename F>
    inline bool consume_one (F const& fn)
    {
        value_type element;
        bool       success = pop_front (element);

        if (success) fn (element);
        return success;
    }

    template <typename F>
    inline size_type consume_all (F& fn)
    {
        size_type element_count = 0;

        for (value_type element; pop_front (element); ++element_count) fn (element);
        return element_count;
    }

    template <typename F>
    inline size_type consume_all (F const& fn)
    {
        size_type element_count = 0;

        for (value_type element; pop_front (element); ++element_count) fn (element);
        return element_count;
    }

private:
    pointer     _M_pArray;
    atomic_size _M_beginPos, _M_endPos;
    size_type   _M_uCapacity;
};

// ====================================================

//! [UNFINISHED] lock-free circular queue (1 producer / 1 consumer)
//! ex. sufficient for event handling
template <typename T, std::size_t N>
class SHARED_API uniform_queue : public non_copyable
{
public:
    static_assert (!std::is_void_v<T>              , "T is void");
    static_assert ( std::is_move_constructible_v<T>, "T is not move constructible!");
    static_assert ( std::is_move_assignable_v<T>   , "T is not move assignable!");

    typedef uniform_queue<T, N>    self_type      ;
    typedef remove_cref_t<T> value_type     ;
    typedef value_type *           pointer        ;
    typedef value_type const*      const_pointer  ;
    typedef value_type &           reference      ;
    typedef value_type const&      const_reference;
    typedef std::size_t            size_type      ;
    typedef std::size_t const      const_size     ;
    typedef std::ptrdiff_t         difference_type;

    inline size_type size () const noexcept
    {
        const_size uBeginPos = _M_uReadPos.load  (std::memory_order_relaxed);
        const_size uEndPos   = _M_uWritePos.load (std::memory_order_relaxed);

        return ((uEndPos - uBeginPos) + capacity()) % capacity();
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

} // cppual

// ====================================================

namespace std {

template <typename T, cppual::memory::allocator_like A, bool Atomic>
struct uses_allocator <cppual::circular_queue<T, A, Atomic>, A> : std::true_type
{ };

} // namespace std

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_CIRCULAR_QUEUE_H_
