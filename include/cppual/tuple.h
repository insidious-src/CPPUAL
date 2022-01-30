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

#ifndef CPPUAL_TUPLE_H_
#define CPPUAL_TUPLE_H_
#ifdef __cplusplus

#include <cppual/decl.h>

#include <cstddef>

namespace cppual {

template <typename... Ts>
class Tuple
{
public:
    typedef std::size_t size_type;

    Tuple();

    explicit Tuple(Ts const&...);

    template <typename... U>
    explicit Tuple(U&&...);

    Tuple(Tuple const&);

    Tuple(Tuple&&);

    template <typename... U>
    Tuple(Tuple<U...> const&);

    template <typename... U>
    Tuple(Tuple<U...>&&);

    Tuple& operator=(const Tuple&);

    Tuple& operator=(Tuple&&);

    template <typename... U>
    Tuple& operator=(const Tuple<U...>&);

    template <typename... U>
    Tuple& operator=(Tuple<U...>&&);

    void swap(Tuple&);

    static constexpr size_type size () { return sizeof...(Ts); }

private:
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_TUPLE_H_

