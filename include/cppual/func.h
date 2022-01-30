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

#ifndef CPPUAL_FUNC_H_
#define CPPUAL_FUNC_H_
#ifdef __cplusplus

#include <cppual/decl.h>

namespace cppual
{

template <typename Type>
inline uint getArrayLen (Type type) noexcept
{
    return (sizeof (type) / sizeof (Type));
}

inline byte getNumDigits (int nNum) noexcept
{
    byte nDigitCount = 0;

    for (int n = (nNum < 0 ? -nNum : nNum); n > 0; n /= 10) ++nDigitCount;
    return nDigitCount;
}

inline bool isPowerOfTwo (int nNum) noexcept
{
    if (nNum < 1) return false;
    return (nNum & (nNum - 1)) == 0;
}

} // cppual

#endif // __cplusplus
#endif // CPPUAL_FUNC_H_
