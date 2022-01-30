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

#ifndef CPPUAL_SYS_INFO_H_
#define CPPUAL_SYS_INFO_H_
#ifdef __cplusplus

#include <string>
#include <cppual/string.h>

namespace cppual { namespace system {

enum class QueryCategory : byte
{
    Motherboard,
    CPU,
    GPU,
    PPU,
    Voltages,
    Sensors,
    Services,
    Kernel,
    Environment
};

struct Motherboard final
{
    enum
    {
        FullName = 1,
        Vendor,
        Model,
        Revision,
        FormFactor,
        BIOSName,
        BIOSVersion,
        RAMDAC,
        RAMQuantity
    };
};

struct CPU final
{
    enum
    {
        FullName = 1,
        Vendor,
        Model,
        CodeName,
        Architecture,
        PhysicalCoresCount,
        LogicalCoresCount,
        SSE,
        SIMD,
        AVX
    };
};

struct GPU final
{
    enum
    {
        FullName = 1,
        Vendor,
        Model,
        CodeName,
        Architecture,
        ComputeCoresCount,
        ShaderModel,
        VRAMQuantity
    };
};

struct InfoQuery final
{
    static string label (QueryCategory category, uint query_id);
    static int    value (QueryCategory category, uint query_id);
};

} } // namespace System

#endif // __cplusplus
#endif // CPPUAL_SYS_INFO_H_
