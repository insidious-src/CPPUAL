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

#include <cppual/concepts>

#include <string>

namespace cppual {

// ====================================================

template <char_t T = char>
constexpr std::size_t constexpr_str_size (T const* str)
{
    return std::char_traits<T>::length (str);
}

// ====================================================

//! char string constexpr performance hash (ex. usage as a map hash)
template <char_t T = char>
constexpr std::size_t constexpr_char_fast_hash (T const* str) noexcept
{
    typedef std::size_t size_type;

    auto const last_pos = constexpr_str_size (str) - 1;
    auto const size     = constexpr_str_size (str);

    return size >= 3 ? ((size ^
                          static_cast<size_type> (str[0]                 ) ^
                         (static_cast<size_type> (str[1]           ) << 1) ^
                         (static_cast<size_type> (str[last_pos - 1]) << 1) ^
                          static_cast<size_type> (str[last_pos]    ) << 1) >> 1) :
               5381;
}

//! char string consteval performance hash (ex. usage as a map hash)
template <char_t T = char, T const* In>
consteval std::size_t consteval_char_fast_hash () noexcept
{
    typedef std::size_t size_type;

    auto const last_pos = constexpr_str_size (In) - 1;
    auto const size     = constexpr_str_size (In);

    return size >= 3 ? ((size ^
                         static_cast<size_type> (In[0]                 ) ^
                        (static_cast<size_type> (In[1]           ) << 1) ^
                        (static_cast<size_type> (In[last_pos - 1]) << 1) ^
                         static_cast<size_type> (In[last_pos]    ) << 1) >> 1) :
                5381;
}

// ====================================================

//! char string constexpr complete hash (ex. usage in switch cases or as a hash)
constexpr uint constexpr_char_hash (cchar* input) noexcept
{
    return *input ? static_cast<uint> (*input) + 33 * constexpr_char_hash (input + 1) : 5381;
}

//! char string consteval complete hash (ex. usage in switch cases or as a hash)
template <cchar* In>
consteval uint consteval_char_hash () noexcept
{
    return *In ? static_cast<uint> (*In) + 33 * consteval_char_hash<In + 1> () : 5381;
}

// ====================================================

struct char_hash
{
    constexpr std::size_t operator () (cchar* key) const noexcept
    {
        return constexpr_char_fast_hash (key);
    }
};

// ====================================================

struct fnv1a_hash
{
    constexpr std::size_t operator () (cchar* str) const noexcept
    {
        std::size_t hash = 2166136261;

        while (*str)
        {
            hash ^= static_cast<std::size_t> (*str++);
            hash *= 16777619;
        }

        return hash;
    }
};

// ====================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_STRING_META_H_
