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

#ifndef CPPUAL_STRING_META_H_
#define CPPUAL_STRING_META_H_
#ifdef __cplusplus


#include <cppual/types>
#include <cppual/string.h>
#include <cppual/concepts>
#include <cppual/containers>

#include <string_view>
#include <string>

namespace cppual {

// ====================================================

template <char_t T = char>
constexpr std::size_t str_size (T const* str)
{
    return std::char_traits<T>::length (str);
}

// ====================================================

//! char string constexpr performance hash (ex. usage as a map hash)
template <char_t T = char>
constexpr std::size_t char_fast_hash (T const* str) noexcept
{
    typedef std::size_t size_type;

    auto const last_pos = str_size (str) - 1;
    auto const size     = str_size (str);

    return size >= 3 ? ((size ^
                          static_cast<size_type> (str[0]                 ) ^
                         (static_cast<size_type> (str[1]           ) << 1) ^
                         (static_cast<size_type> (str[last_pos - 1]) << 1) ^
                          static_cast<size_type> (str[last_pos]    ) << 1) >> 1) :
               5381;
}

// ====================================================

constexpr std::size_t char_fast_hash (std::string_view const& str) noexcept
{
    return char_fast_hash (str.data ());
}

// ====================================================

//! char string consteval performance hash (ex. usage as a map hash)
template <auto const* In> requires (c_const_tstr<decltype (In)>)
consteval std::size_t char_fast_hash () noexcept
{
    typedef std::size_t size_type;

    auto const size     = str_size (In);
    auto const last_pos = size - 1;

    return size >= 3 ? ((size ^
                         static_cast<size_type> (In[0]                 ) ^
                        (static_cast<size_type> (In[1]           ) << 1) ^
                        (static_cast<size_type> (In[last_pos - 1]) << 1) ^
                         static_cast<size_type> (In[last_pos]    ) << 1) >> 1) :
                5381;
}

// ====================================================

//! char string constexpr complete hash (ex. usage in switch cases or as a hash)
template <char_t T = char>
constexpr std::size_t char_hash (T const* input) noexcept
{
    return *input ? static_cast<std::size_t> (*input) + 33 * char_hash (input + 1) : 5381;
}

constexpr std::size_t char_hash (std::string_view const& input) noexcept
{
    return char_hash (input.data ());
}

//! char string consteval complete hash (ex. usage in switch cases or as a hash)
template <auto const* In> requires (c_const_tstr<decltype (In)>)
consteval std::size_t char_hash () noexcept
{
    return *In ? static_cast<std::size_t> (*In) + 33 * char_hash<In + 1> () : 5381;
}

// ====================================================

struct char_hash
{
    typedef std::size_t size_type;

    template <c_const_tstr Char>
    constexpr size_type operator () (Char key) const noexcept
    {
        return char_fast_hash (key);
    }

    constexpr size_type operator () (std::string_view key) const noexcept
    {
        return char_fast_hash (key);
    }
};

// ====================================================

struct fnv1a_hash
{
    typedef std::size_t size_type;

    template <c_const_tstr Char>
    constexpr size_type operator () (Char str) const noexcept
    {
        size_type hash = 2166136261;

        while (*str)
        {
            hash ^= static_cast<size_type> (*str++);
            hash *= 16777619;
        }

        return hash;
    }

    constexpr size_type operator () (std::string_view const& sv) const noexcept
    {
        return (*this)(sv.data ());
    }

    constexpr size_type operator () (fstring const& sv) const noexcept
    {
        return (*this)(sv.data ());
    }

    constexpr size_type operator () (string const& sv) const noexcept
    {
        return (*this)(sv.data ());
    }
};

// ====================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_STRING_META_H_
