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

#ifndef CPPUAL_CIRCULAR_QUEUE_H_
#define CPPUAL_CIRCULAR_QUEUE_H_
#ifdef __cplusplus

#include <atomic>
#include <cppual/concepts.h>
#include <cppual/iterator.h>
#include <cppual/noncopyable.h>
#include <cppual/memory/allocator.h>

using std::atomic_size_t;
using std::atomic_bool  ;

namespace cppual {

template <typename T>
struct expand_ratio
{
    static constexpr std::size_t value =
            (sizeof (T*) / sizeof (T)) * (sizeof (T*) / 2);
};

// ====================================================

template <typename T,
          typename Allocator = Memory::Allocator<T>,
          bool     Atomic    = false
          >
class CircularQueue : private Allocator
{
public:
    typedef T                                     value_type            ;
    typedef T*                                    pointer               ;
    typedef T const*                              const_pointer         ;
    typedef T&                                    reference             ;
    typedef T const&                              const_reference       ;
    typedef Memory::AllocatorType<Allocator>      allocator_type        ;
    typedef typename Allocator::size_type         size_type             ;
    typedef typename Allocator::size_type const   const_size            ;
    typedef typename Allocator::difference_type   difference_type       ;
    typedef CircularQueue<T, Allocator, Atomic>   self_type             ;
    typedef CircularIterator<self_type>           iterator              ;
    typedef CircularIterator<self_type const>     const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef std::pair<pointer, size_type>         array_range           ;
    typedef std::pair<const_pointer, size_type>   const_array_range     ;

    enum { default_size = 10 };
    CircularQueue& operator = (CircularQueue const&);
    CircularQueue (CircularQueue&&) noexcept = default;
    void resize   (size_type new_capacity);
    void erase    (const_iterator&);

    constexpr const_pointer    data   () const noexcept { return m_pArray; }
    inline    iterator         begin  () noexcept { return iterator (*this, 0); }
    inline    iterator         end    () noexcept { return iterator (*this, size ()); }
    constexpr const_iterator   begin  () const noexcept { return const_iterator (*this, 0); }
    constexpr const_iterator   end    () const noexcept { return const_iterator (*this, size ()); }
    constexpr const_iterator   cbegin () const noexcept { return const_iterator (*this, 0); }
    constexpr const_iterator   cend   () const noexcept { return const_iterator (*this, size ()); }
    inline    reverse_iterator rbegin () noexcept { return reverse_iterator (end ()); }
    inline    reverse_iterator rend   () noexcept { return reverse_iterator (begin ()); }
    constexpr const_reference  front  () const  { return m_pArray[m_uBeginPos]; }
    constexpr const_reference  back   () const  { return m_pArray[m_uEndPos  ]; }
    inline    reference        front  ()        { return m_pArray[m_uBeginPos]; }
    inline    reference        back   ()        { return m_pArray[m_uEndPos  ]; }
    constexpr allocator_type   get_allocator () const noexcept { return *this;  }
    inline    void             pop_front     () { if (!empty ()) _pop_front (); }
    inline    void             pop_back      () { if (!empty ()) _pop_back  (); }

    CircularQueue& operator = (CircularQueue&& gObj)
    {
        if (this == &gObj) return *this;
        dispose  ();
        swap (gObj);
        return *this;
    }

    ~CircularQueue () noexcept
    {
        if  (!m_pArray) return;
        for (value_type& elem : *this) allocator_type::destroy (&elem);
        allocator_type::deallocate (m_pArray, m_uCapacity);
    }

    explicit
    CircularQueue (size_type         uCapacity = default_size,
                   allocator_type const& gAtor = allocator_type ())
    : allocator_type (gAtor),
      m_pArray       (allocator_type::allocate (uCapacity)),
      m_uBeginPos    (),
      m_uEndPos      (),
      m_uCapacity    (m_pArray ? uCapacity : size_type ())
    { if (!uCapacity) throw std::bad_array_new_length  (); }

    CircularQueue (allocator_type const& gAtor)
    : allocator_type (gAtor),
      m_pArray       (),
      m_uBeginPos    (),
      m_uEndPos      (),
      m_uCapacity    ()
    { }

    CircularQueue (CircularQueue const& gObj)
    : allocator_type (gObj),
      m_pArray       (!gObj.empty () ? allocator_type::allocate (gObj.size ()) : pointer ()),
      m_uBeginPos    (),
      m_uEndPos      (m_pArray ? gObj.size () - 1 : m_uBeginPos),
      m_uCapacity    (m_pArray ? m_uEndPos    + 1 : m_uBeginPos)
    {
        if (!gObj.empty () and !m_pArray) throw std::bad_array_new_length ();
        std::copy (gObj.cbegin (), gObj.cend (), begin ());
    }

    template <typename Iterator>
    CircularQueue (IteratorType<Iterator> gBegin, IteratorType<Iterator> gEnd,
                   allocator_type const&  gAtor = allocator_type ())
    : allocator_type (gAtor),
      m_pArray       (allocator_type::allocate (diff (gEnd, gBegin) + 1)),
      m_uBeginPos    (),
      m_uEndPos      (m_pArray ? diff (gEnd, gBegin) : 0),
      m_uCapacity    (m_pArray ? m_uEndPos + 1 : 0)
    { m_pArray ? std::copy (gBegin, gEnd, begin ()) : throw std::bad_array_new_length (); }

    constexpr const_reverse_iterator crbegin () const
    { return const_reverse_iterator (end ()); }

    constexpr const_reverse_iterator crend () const
    { return const_reverse_iterator (begin ()); }

    reference operator [] (size_type n)
    { return m_pArray[index_to_subscript (n)];  }

    constexpr const_reference operator [] (size_type n) const
    { return m_pArray[index_to_subscript (n)];  }

    constexpr size_type capacity () const noexcept
    { return m_uCapacity; }

    constexpr size_type max_size () const noexcept
    { return std::numeric_limits<size_type>::max (); }

    array_range array_one () noexcept
    {
        return array_range (m_pArray + m_uBeginPos, is_linearized () ?
                                size () : capacity () - m_uBeginPos);
    }

    constexpr const_array_range array_one () const noexcept
    {
        return const_array_range (m_pArray + m_uBeginPos, is_linearized () ?
                                      size () : capacity () - m_uBeginPos);
    }

    array_range array_two () noexcept
    {
        return !is_linearized () ? const_array_range (m_pArray + (m_uEndPos - 1), m_uEndPos) :
                                   const_array_range ();
    }

    const_array_range array_two () const noexcept
    {
        return !is_linearized () ? const_array_range (m_pArray + (m_uEndPos - 1), m_uEndPos) :
                                   const_array_range ();
    }

    size_type size () const noexcept
    {
        //return !is_linearized () ? m_uEndPos + (capacity () - m_uBeginPos)
                                 //: m_uEndPos - m_uBeginPos;
        return !is_linearized () ? capacity () - (m_uBeginPos - m_uEndPos)
                                 : m_uEndPos   - m_uBeginPos;
    }

    constexpr bool is_linearized () const noexcept
    { return m_uBeginPos <= m_uEndPos; }

    constexpr bool empty () const noexcept
    { return m_uBeginPos == m_uEndPos; }

    constexpr bool full () const noexcept
    { return m_uBeginPos == normalize (m_uEndPos + 1); }

    void reserve (size_type uNewCapacity)
    { if (uNewCapacity > capacity ()) resize (uNewCapacity); }

    void shrink_to_fit ()
    { if (capacity () != size ()) swap (self_type (*this)); }

    template <typename... Args>
    void emplace_back (Args&&... args)
    { push_back (std::forward<Args> (args)...); }

    template <typename... Args>
    void emplace_front (Args&&... args)
    { push_front (std::forward<Args> (args)...); }

    const_reference at (size_type n) const
    { return size () > n ? (*this)[n] : throw std::out_of_range ("index is out of range"); }

    reference at (size_type n)
    { return size () > n ? (*this)[n] : throw std::out_of_range ("index is out of range"); }

    template <typename... Args>
    void push_front (Args&&... args)
    {
        // ++front--
        size_type uNewBegin = m_uBeginPos ? m_uBeginPos - 1 : capacity () - 1;

        if (uNewBegin == m_uEndPos) // full
        {
            resize (capacity () + expand_size ());
            uNewBegin = capacity () - 1;
        }

        // add element
        allocator_type::construct (&m_pArray[uNewBegin], std::forward<Args> (args)...);
        m_uBeginPos = uNewBegin;
    }

    template <typename... Args>
    void push_back (Args&&... args)
    {
        // --back++
        size_type uNewEnd = normalize (m_uEndPos + 1);

        if (uNewEnd == m_uBeginPos) // full
        {
            resize (capacity () + expand_size ());
            uNewEnd = normalize (m_uEndPos + 1);
        }

        // add element
        allocator_type::construct (&m_pArray[m_uEndPos], std::forward<Args> (args)...);
        m_uEndPos = uNewEnd;
    }

    void swap (self_type& gObj) noexcept
    {
        std::swap (static_cast<allocator_type&> (*this),
                   static_cast<allocator_type&> (gObj));

        std::swap (m_pArray,    gObj.m_pArray   );
        std::swap (m_uBeginPos, gObj.m_uBeginPos);
        std::swap (m_uEndPos,   gObj.m_uEndPos  );
        std::swap (m_uCapacity, gObj.m_uCapacity);
    }

    void clear ()
    {
        for (value_type& elem : *this) allocator_type::destroy (&elem);
        m_uBeginPos = m_uEndPos = size_type ();
    }

private:
    void _pop_front ()
    {
        // ++front--
        allocator_type::destroy (&m_pArray[m_uBeginPos]);
        m_uBeginPos = normalize (++m_uBeginPos);
    }

    void _pop_back ()
    {
        // --back++
        allocator_type::destroy (&m_pArray[m_uEndPos = normalize (--m_uEndPos)]);
    }

    size_type normalize (size_type uIdx) const noexcept
    { return uIdx % capacity (); }

    size_type index_to_subscript (size_type uIdx) const noexcept
    { return (m_uBeginPos + uIdx) % capacity (); }

    constexpr size_type expand_size () const noexcept
    { return expand_ratio<value_type>::value + 1; }

    template <typename Iterator>
    constexpr static size_type diff (Iterator i1, Iterator i2) noexcept
    { return i1 >= i2 ? size_type (i1 - i2) : size_type (i2 - i1); }

    void dispose ();

private:
    pointer   m_pArray;
    size_type m_uBeginPos, m_uEndPos, m_uCapacity;
};

// ====================================================

template <typename T, typename Allocator, bool Atomic>
CircularQueue<T, Allocator, Atomic>&
CircularQueue<T, Allocator, Atomic>::operator = (CircularQueue const& gObj)
{
    if (this == &gObj) return *this;

    allocator_type::operator = (gObj);
    size_type new_size = gObj.size ();

    if (capacity () < new_size)
    {
        dispose  ();
        m_pArray     = allocator_type::allocate (new_size);
        m_uCapacity  = new_size;
    }
    else m_uBeginPos = size_type ();

    m_uEndPos = index_to_subscript (--new_size);
    std::copy (gObj.cbegin (), gObj.cend (), begin ());
    return *this;
}

template <typename T, typename Allocator, bool Atomic>
void CircularQueue<T, Allocator, Atomic>::resize (size_type uNewCapacity)
{
    self_type gObj (uNewCapacity, *this);
    size_type uNewSize = size () > uNewCapacity ? uNewCapacity : size ();

    std::copy (cbegin (), const_iterator (*this, uNewSize), gObj.begin ());
    gObj.m_uEndPos = uNewSize - 1;
    swap (gObj);
}

template <typename T, typename Allocator, bool Atomic>
void CircularQueue<T, Allocator, Atomic>::erase (const_iterator& gIt)
{
    if (empty () or gIt < cbegin () or cend () <= gIt)
        throw std::out_of_range ("iterator is out of range");

    auto pos = index_to_subscript (gIt.pos ());

    if (pos == m_uBeginPos) _pop_front ();
    else if (pos == index_to_subscript (size () - 1)) _pop_back ();
    else allocator_type::destroy (&(*gIt));
}

template <typename T, typename Allocator, bool Atomic>
void CircularQueue<T, Allocator, Atomic>::dispose ()
{
    clear ();
    allocator_type::deallocate (m_pArray, capacity ());
    m_uCapacity = size_type ();
    m_pArray    = pointer   ();
}

// ====================================================

// lock-free circular queue (1 producer / 1 consumer)
// ex. sufficient for event handling
template <typename T, std::size_t N>
class UniformQueue : NonCopyable
{
public:
    typedef T                  value_type;
    typedef T*                 pointer;
    typedef T const*           const_pointer;
    typedef T&                 reference;
    typedef T const&           const_reference;
    typedef std::size_t        size_type;
    typedef std::size_t const  const_size;
    typedef std::ptrdiff_t     difference_type;
    typedef UniformQueue<T, N> self_type;

    inline size_type size () const noexcept
    {
        const_size uBeginPos = m_uReadPos.load  (std::memory_order_relaxed);
        const_size uEndPos   = m_uWritePos.load (std::memory_order_relaxed);

        return uBeginPos > uEndPos ? uEndPos + (capacity () - uBeginPos)
                                   : uEndPos - uBeginPos;
    }

    constexpr bool is_linearized () const noexcept
    {
        return m_uReadPos .load (std::memory_order_relaxed) <=
               m_uWritePos.load (std::memory_order_relaxed);
    }

    constexpr static size_type capacity () noexcept
    { return N + 1; }

    bool is_lock_free () const noexcept
    { return m_uWritePos.is_lock_free () and m_uReadPos.is_lock_free (); }

    bool empty () const noexcept
    { return m_uReadPos.load () == m_uWritePos.load (); }

    bool full () const noexcept
    { return ((m_uWritePos.load () + 1) % capacity ()) == m_uReadPos.load (); }

    void push_back (value_type const& value)
    { push_back (const_cast<value_type&> (value)); }

    void emplace_back (value_type const& value)
    { push_back (const_cast<value_type&> (value)); }

    void emplace_back (value_type&& value)
    { push_back (std::forward<value_type> (value)); }

    template <typename... Args>
    void emplace_back (Args&&... args)
    { push_back (std::forward<Args> (args)...); }

    constexpr UniformQueue () noexcept
    : m_uReadPos (), m_uWritePos ()
    { }

    template <typename Iterator>
    UniformQueue  (InputIterator<Iterator> gBegin, InputIterator<Iterator> gEnd) noexcept
    : m_uReadPos  (),
      m_uWritePos (size_type (gEnd - gBegin) > N ? N : size_type (gEnd - gBegin))
    { std::copy (gBegin, gBegin + m_uWritePos, m_Array); }

    void push_back (value_type&& value)
    {
        const_size uCurWrite  = m_uWritePos.load (std::memory_order_relaxed);
        const_size uNextWrite = next (uCurWrite);

        if (uNextWrite != m_uReadPos.load (std::memory_order_acq_rel))
        {
            new (&m_Array[uCurWrite]) T (std::forward<value_type> (value));
            m_uWritePos = uNextWrite;
        }
        else m_Array[uCurWrite] = std::forward<value_type> (value);
    }

    template <typename... Args>
    void push_back (Args&&... args)
    {
        const_size uCurWrite  = m_uWritePos.load (std::memory_order_relaxed);
        const_size uNextWrite = next (uCurWrite);

        if (uNextWrite != m_uReadPos.load (std::memory_order_acq_rel))
        {
            new (&m_Array[uCurWrite]) T (std::forward<Args> (args)...);
            m_uWritePos = uNextWrite;
        }
        else
        {
            m_Array[uCurWrite].~value_type ();
            new (&m_Array[uCurWrite]) T (std::forward<Args> (args)...);
        }
    }

    bool pop_front (T& elem)
    {
        const_size uCurRead = m_uReadPos.load (std::memory_order_relaxed);

        if (uCurRead == m_uWritePos.load (std::memory_order_acq_rel))
            return false; // empty

        elem = m_Array[uCurRead];
        m_Array[uCurRead].~value_type ();

        m_uReadPos = next (uCurRead);
        return true;
    }

private:
    static size_type next (size_type uIdx) noexcept
    { return ++uIdx % N; }

private:
    atomic_size_t m_uReadPos, m_uWritePos;
    T             m_Array[capacity ()];
};

// ====================================================

// feature complete lock-free multi-producer/multi-consumer bidirectional queue
// ex. game messaging queue
template <typename T, class Allocator>
class CircularQueue <T, Allocator, true> : Allocator, NonCopyable
{
public:
    typedef T                                   value_type;
    typedef T*                                  pointer;
    typedef T const*                            const_pointer;
    typedef T&                                  reference;
    typedef T const&                            const_reference;
    typedef atomic_size_t                       atomic_size;
    typedef Memory::AllocatorType<Allocator>    allocator_type;
    typedef typename Allocator::size_type       size_type;
    typedef typename Allocator::size_type const const_size;
    typedef typename Allocator::difference_type difference_type;
    typedef CircularQueue<T, Allocator, true>   self_type;

    CircularQueue () noexcept;

    void reserve (size_type);
    void reserve_unsafe (size_type);
    bool push_back (const_reference);
    bool bounded_push_back (const_reference);
    bool unsynchronized_push_back (const_reference);
    bool pop_front (reference);
    bool unsynchronized_pop_front (reference);

    bool is_lock_free () const noexcept
    { return m_uBeginPos.is_lock_free () and m_uEndPos.is_lock_free (); }

    bool empty () const noexcept
    {
        return m_uBeginPos.load (std::memory_order_relaxed) =
               m_uEndPos  .load (std::memory_order_relaxed);
    }

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
    pointer     m_pArray;
    atomic_size m_uBeginPos, m_uEndPos;
    size_type   m_uCapacity;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_CIRCULAR_QUEUE_H_
