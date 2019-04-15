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

#ifndef CPPUAL_UNBOUND_MATRIX_H_
#define CPPUAL_UNBOUND_MATRIX_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/signal.h>
#include <cppual/concepts.h>
#include <cppual/functional.h>
#include <cppual/circular_queue.h>
#include <cppual/reactive.h>

#include <memory>
#include <cstdint>
#include <cstddef>
#include <deque>
#include <vector>
#include <variant>
#include <type_traits>

namespace cppual {

template <typename T, typename Allocator = std::allocator<T> >
class UnboundMatrix
{
public:
    typedef T                                value_type    ;
    typedef Allocator                        allocator_type;
    typedef T*                               pointer       ;
    typedef T const*                         const_pointer ;
    typedef std::vector  <T, allocator_type> vector_type   ;
    typedef std::deque   <T, allocator_type> array_type    ;
    typedef Signal    <void, allocator_type> signal_type   ;
    typedef CircularQueue<T, allocator_type> queue_type    ;
    typedef typename Allocator::size_type    size_type     ;

    enum class Instructions
    {
        Add,
        Subtract,
        Multiply,
        Divide,
        Modulo,
        Power,
        Log,
        Root
    };

    void process ();

    constexpr size_type    size      () const noexcept { return rows () * cols (); }
    constexpr size_type    cols      () const noexcept { return m_uCols;           }
    constexpr size_type    rows      () const noexcept { return m_uRows;           }
    constexpr size_type    maxRow    ()       noexcept { return rows () - 1;       }
    constexpr size_type    maxCol    ()       noexcept { return cols () - 1;       }
    constexpr signal_type& processed ()       noexcept { return m_signal;          }

    constexpr const_pointer data () const noexcept
    { return m_matrix.data (); }

    constexpr UnboundMatrix () noexcept
    : m_matrix (),
      m_uCols  (),
      m_uRows  ()
    { }

    UnboundMatrix (UnboundMatrix&& gObj) noexcept
    : m_matrix (std::move (gObj.m_matrix)),
      m_uCols  (gObj.m_uCols),
      m_uRows  (gObj.m_uRows)
    { }

    UnboundMatrix (UnboundMatrix const& gObj) noexcept
    : m_matrix (gObj.m_matrix),
      m_uCols  (gObj.m_uCols ),
      m_uRows  (gObj.m_uRows )
    { }

    template <typename U>
    Reactive<U> operator * (Reactive<U>); // multiply

    template <typename U>
    Reactive<U> operator *= (Reactive<U>); // multiply

    template <typename U>
    Reactive<U> operator / (Reactive<U>); // divide

    template <typename U>
    Reactive<U> operator /= (Reactive<U>); // divide

    template <typename U>
    Reactive<U> operator + (Reactive<U>); // add

    template <typename U>
    Reactive<U> operator += (Reactive<U>); // add

    template <typename U>
    Reactive<U> operator - (Reactive<U>); // subtract

    template <typename U>
    Reactive<U> operator -= (Reactive<U>); // subtract

    template <typename U>
    Reactive<U> operator % (Reactive<U>); // modulo

    template <typename U>
    Reactive<U> operator %= (Reactive<U>); // modulo

    template <typename U>
    Reactive<U> root (Reactive<U>, size_type);

    template <typename U>
    Reactive<U> log (Reactive<U>);

    template <typename U>
    Reactive<U> power (Reactive<U>, size_type);

private:
    vector_type m_matrix          ;
    queue_type  m_instructionQueue;
    signal_type m_signal          ;
    size_type   m_uCols, m_uRows  ;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_UNBOUND_MATRIX_H_
