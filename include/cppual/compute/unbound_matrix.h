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

#ifndef CPPUAL_UNBOUND_MATRIX_H_
#define CPPUAL_UNBOUND_MATRIX_H_
#ifdef __cplusplus

#include <cppual/types>
#include <cppual/signal>
#include <cppual/reactive>
#include <cppual/containers>
#include <cppual/functional>
#include <cppual/circular_queue>
#include <cppual/memory_allocator>
#include <cppual/concepts>

//#include <memory>
//#include <cstdint>
//#include <cstddef>
//#include <variant>
//#include <type_traits>

namespace cppual {

// =========================================================

using memory::allocator_like;

// =========================================================


/**
  * @brief The unbound_matrix class
  * XAXAXaxAxAXAxaxa няма да ти кажа...
  */
template <non_void T = uchar, allocator_like A = memory::allocator<T>>
class unbound_matrix
{
public:
    typedef unbound_matrix<T, A>                       self_type      ;
    typedef std::allocator_traits<A>                   traits_type    ;
    typedef traits_type::allocator_type                allocator_type ;
    typedef remove_cvrefptr_t<T>                       value_type     ;
    typedef value_type &                               reference      ;
    typedef value_type const&                          const_reference;
    typedef value_type *                               pointer        ;
    typedef value_type const*                          const_pointer  ;
    typedef std::vector<value_type, allocator_type>    vector_type    ;
    typedef std::deque <value_type, allocator_type>    array_type     ;
    typedef signal<void()>                             signal_type    ;
    typedef function<void(reference)>                  fn_type        ;
    typedef dyn_array<fn_type>                         fn_vector      ;
    typedef circular_queue<value_type, allocator_type> queue_type     ;
    typedef allocator_type::size_type                  size_type      ;

    enum class operation : u8
    {
        none = 0,
        add,
        subtract,
        multiply,
        divide,
        modulo,
        power,
        log,
        root,
        floor,
        round,
        cin,
        cos,
        tan,
        asin,
        acos,
        atan,
        atan2,
        sinh,
        cosh,
        tanh
    };

    /// Process sequentially each function using matrix'
    /// queue operation functions and calculate each result
    /// in parallel. Return all results.
    vector_type process (fn_vector);

    constexpr size_type    cols      () const noexcept { return _M_uCols;           }
    constexpr size_type    rows      () const noexcept { return _M_uRows;           }
    constexpr size_type    size      () const noexcept { return rows () * cols ();  }
    constexpr size_type    max_row   ()       noexcept { return rows () - 1;        }
    constexpr size_type    max_col   ()       noexcept { return cols () - 1;        }
    constexpr signal_type& processed ()       noexcept { return _M_processedSignal; }

    constexpr const_pointer data () const noexcept
    { return _M_matrix.data (); }

    constexpr unbound_matrix () noexcept
    : _M_matrix (),
      _M_uCols  (),
      _M_uRows  ()
    {
        _M_matrix.reserve (size ());
    }

    constexpr unbound_matrix (unbound_matrix&& gObj) noexcept
    : _M_matrix (std::move (gObj._M_matrix)),
      _M_instructionQueue (std::move (gObj._M_instructionQueue)),
      _M_uCols  (std::move (gObj._M_uCols)),
      _M_uRows  (std::move (gObj._M_uRows))
    { }

    constexpr unbound_matrix (unbound_matrix const& gObj) noexcept
    : _M_matrix (gObj._M_matrix),
      _M_uCols  (gObj._M_uCols ),
      _M_uRows  (gObj._M_uRows )
    { }

    template <typename U>
    reactive<U>& operator * (reactive<U> const&); //! multiply

    template <typename U>
    reactive<U>& operator *= (reactive<U> const&); //! multiply

    template <typename U>
    reactive<U>& operator / (reactive<U> const&); //! divide

    template <typename U>
    reactive<U>& operator /= (reactive<U> const&); //! divide

    template <typename U>
    reactive<U>& operator + (reactive<U> const&); //! add

    template <typename U>
    reactive<U>& operator += (reactive<U> const&); //! add

    template <typename U>
    reactive<U>& operator - (reactive<U> const&); //! subtract

    template <typename U>
    reactive<U>& operator -= (reactive<U> const&); //! subtract

    template <typename U>
    reactive<U>& operator % (reactive<U> const&); //! modulo

    template <typename U>
    reactive<U>& operator %= (reactive<U> const&); //! modulo

    template <typename U>
    reactive<U>& root (reactive<U> const&);

    template <typename U>
    reactive<U>& log (reactive<U> const&);

    template <typename U>
    reactive<U>& power (reactive<U> const&);

    template <typename U>
    reactive<U>& floor (reactive<U> const&);

    template <typename U>
    reactive<U>& round (reactive<U> const&);

    template <typename U>
    reactive<U>& cin (reactive<U> const&);

    template <typename U>
    reactive<U>& cos (reactive<U> const&);

    template <typename U>
    reactive<U>& tan (reactive<U> const&);

    template <typename U>
    reactive<U>& asin (reactive<U> const&);

    template <typename U>
    reactive<U>& acos (reactive<U> const&);

    template <typename U>
    reactive<U>& atan (reactive<U> const&);

    template <typename U>
    reactive<U>& atan2 (reactive<U> const&);

    template <typename U>
    reactive<U>& sinh (reactive<U> const&);

    template <typename U>
    reactive<U>& cosh (reactive<U> const&);

    template <typename U>
    reactive<U>& tanh (reactive<U> const&);

private:
    vector_type _M_matrix          ;
    queue_type  _M_instructionQueue;
    signal_type _M_processedSignal ;
    size_type   _M_uCols, _M_uRows ;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_UNBOUND_MATRIX_H_
