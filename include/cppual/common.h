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

#ifndef CPPUAL_COMMON_H_
#define CPPUAL_COMMON_H_
#ifdef __cplusplus

#include <cppual/decl.h>

namespace cppual {

enum class access_mode : byte
{
    read,
    write,
    read_write
};

enum class load_from : byte
{
    file,
    memory,
    binary
};

enum class orientation : byte
{
    horizontal,
    vertical
};

// ====================================================

template <typename T>
inline char* binaryToString (T value, char* str) noexcept
{
    auto p = str;

    for(int i = (static_cast<int>(sizeof(T)) - 1); i >= 0; --i)
        *p++ = *(reinterpret_cast<char*>(&value) + i);

    return str;
}

template <typename T>
inline T stringToBinary (cchar* str) noexcept
{
    T value = T ();

    for(int i = static_cast<int>(sizeof (T) - 1); i >= 0; --i)
        *(reinterpret_cast<char*>(&value) + i) = *str++;

    return value;
}

template <typename T>
T ByteArrayToBaseType(cchar* byteArray, int littleEndian) {
    int i;
    T x;
    if(!littleEndian) {
        for(i = (static_cast<int>(sizeof(T)) - 1); i >= 0; -- i) {
            *(reinterpret_cast<char*>(&x) + i) = *byteArray ++;
        }
    }
    else {
        for(i = 0; i < static_cast<int>(sizeof(T)); ++ i) {
            *(reinterpret_cast<char*>(&x) + i) = *byteArray ++;
        }
    }
    return (x);
}

} // cppual

#endif // __cplusplus
#endif // CPPUAL_COMMON_H_
