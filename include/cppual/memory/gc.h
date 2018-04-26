/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#ifndef CPPUAL_MEMORY_MODEL_H_
#define CPPUAL_MEMORY_MODEL_H_
#ifdef __cplusplus

#include <cstddef>
#include <cppual/types.h>

namespace cppual { namespace Memory {

static_assert (sizeof (std::size_t) == sizeof (u8*),
               "size_t is not equal to the size of a pointer!");

static_assert (alignof (std::size_t) == alignof (u8*),
               "The alignment is wrong!");

// =========================================================

std::size_t size        (); // system memory size
std::size_t maxSize     (); // largest available system memory block
std::size_t workingSize (); // process' current memory usage

// =========================================================



} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_MODEL_H_
