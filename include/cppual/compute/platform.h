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

#ifndef CPPUAL_COMPUTE_PLATFORM_H_
#define CPPUAL_COMPUTE_PLATFORM_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/compute/device.h>

namespace cppual { namespace Compute {

class platform_exception : public std::bad_exception { };
class bad_platform       : public platform_exception { };

class Instance : public NonCopyable
{
public:
    size_t deviceCount ();
    size_t gpuCount    ();
};

namespace Platform {

enum class Info : unsigned char
{
    Name,
    Vendor,
    Profile,
    Version
};

u16    count () noexcept;
bool   available (cchar* feature, u16 id = 0);
float  version (u16 id = 0);
string info (Info, u16 id = 0);

} } } // namespace Platform

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_PLATFORM_H_
