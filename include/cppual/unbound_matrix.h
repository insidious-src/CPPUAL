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

#ifndef CPPUAL_UNBOUND_MATRIX_H_
#define CPPUAL_UNBOUND_MATRIX_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/signal.h>
#include <cppual/concepts.h>
#include <cppual/functional.h>
#include <cppual/circular_queue.h>
#include <cppual/memory/allocator.h>
#include <cppual/reactive.h>

#include <memory>
#include <cstdint>
#include <cstddef>
#include <deque>
#include <vector>
#include <variant>
#include <type_traits>

namespace cppual {

template <typename T, typename Allocator = memory::allocator<T> >
class unbound_matrix
{
public:
    typedef T                                    value_type    ;
    typedef Allocator                            allocator_type;
    typedef T*                                   pointer       ;
    typedef T const*                             const_pointer ;
    typedef std::vector      <T, allocator_type> vector_type   ;
    typedef std::deque       <T, allocator_type> array_type    ;
    typedef signal        <void, allocator_type> signal_type   ;
    typedef function         <void(value_type&)> fn_type       ;
    typedef std::vector<fn_type, allocator_type> fn_vector_type;
    typedef circular_queue   <T, allocator_type> queue_type    ;
    typedef typename Allocator::size_type        size_type     ;

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

    /// Process sequentially each function using matrix'
    /// queue operation functions and calculate each result
    /// in parallel. Return all results.
    vector_type process (fn_vector_type);

    constexpr size_type    size      () const noexcept { return rows () * cols (); }
    constexpr size_type    cols      () const noexcept { return _M_uCols;          }
    constexpr size_type    rows      () const noexcept { return _M_uRows;          }
    constexpr size_type    max_row   ()       noexcept { return rows () - 1;       }
    constexpr size_type    max_col   ()       noexcept { return cols () - 1;       }
    constexpr signal_type& processed ()       noexcept { return _M_signal;         }

    constexpr const_pointer data () const noexcept
    { return _M_matrix.data (); }

    constexpr unbound_matrix () noexcept
    : _M_matrix (),
      _M_uCols  (),
      _M_uRows  ()
    { }

    unbound_matrix (unbound_matrix&& gObj) noexcept
    : _M_matrix (std::move (gObj._M_matrix)),
      _M_uCols  (gObj._M_uCols),
      _M_uRows  (gObj._M_uRows)
    { }

    unbound_matrix (unbound_matrix const& gObj) noexcept
    : _M_matrix (gObj._M_matrix),
      _M_uCols  (gObj._M_uCols ),
      _M_uRows  (gObj._M_uRows )
    { }

    template <typename U>
    reactive<U> operator * (reactive<U>); // multiply

    template <typename U>
    reactive<U> operator *= (reactive<U>); // multiply

    template <typename U>
    reactive<U> operator / (reactive<U>); // divide

    template <typename U>
    reactive<U> operator /= (reactive<U>); // divide

    template <typename U>
    reactive<U> operator + (reactive<U>); // add

    template <typename U>
    reactive<U> operator += (reactive<U>); // add

    template <typename U>
    reactive<U> operator - (reactive<U>); // subtract

    template <typename U>
    reactive<U> operator -= (reactive<U>); // subtract

    template <typename U>
    reactive<U> operator % (reactive<U>); // modulo

    template <typename U>
    reactive<U> operator %= (reactive<U>); // modulo

    template <typename U>
    reactive<U> root (reactive<U>, size_type);

    template <typename U>
    reactive<U> log (reactive<U>);

    template <typename U>
    reactive<U> power (reactive<U>, size_type);

private:
    vector_type _M_matrix          ;
    queue_type  _M_instructionQueue;
    signal_type _M_signal          ;
    size_type   _M_uCols, _M_uRows ;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_UNBOUND_MATRIX_H_
