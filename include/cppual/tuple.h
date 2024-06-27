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
class tuple
{
public:
    typedef std::size_t size_type;

    tuple();

    explicit tuple(Ts const&...);

    template <typename... U>
    explicit tuple(U&&...);

    tuple(tuple const&);

    tuple(tuple&&);

    template <typename... U>
    tuple(tuple<U...> const&);

    template <typename... U>
    tuple(tuple<U...>&&);

    tuple& operator=(const tuple&);

    tuple& operator=(tuple&&);

    template <typename... U>
    tuple& operator=(const tuple<U...>&);

    template <typename... U>
    tuple& operator=(tuple<U...>&&);

    void swap(tuple&);

    static constexpr size_type size () { return sizeof...(Ts); }

private:
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_TUPLE_H_

