/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_COMPUTE_DEVICE_H_
#define CPPUAL_COMPUTE_DEVICE_H_
#ifdef __cplusplus

#include <string>
#include <cppual/flags.h>
#include <cppual/noncopyable.h>
#include <cppual/compute/object.h>

using std::string;

namespace cppual { namespace Compute {

class device_exception    : public std::bad_exception { };
class bad_device          : public device_exception   { };
class not_available       : public device_exception   { };
class already_created     : public device_exception   { };
class bad_partition_count : public device_exception   { };

// =========================================================

class Device
{
public:
    enum Type
    {
        CPU    = 1 << 0, // serial processing
        IGPU   = 1 << 1, // shader, texture and parallel processing
        DGPU   = 1 << 2, // shader, texture and parallel processing
        PPU    = 1 << 3, // dedicated parallel processing (generic processing)
        DSP    = 1 << 4, // digital signal processing (audio)
        BSD    = 1 << 5, // bit stream decoder (video)
        VCE    = 1 << 6, // codec engine (video)
        FPGA   = 1 << 7, // field-programmable gate array
        MPU    = 1 << 8, // multi-specialized processing cores
        Custom = 1 << 9,

        GPU         = IGPU | DGPU,
        Accelerator =  PPU |  DSP | BSD  | FPGA | VCE,
        Any         =  CPU |  GPU | Accelerator | MPU | Custom
    };

    enum Attribute
    {
        Specialized = 1 << 0, // specialized processing
        Native      = 1 << 1  // supoorts execution of native functions
    };

    enum class Backend : unsigned char
    {
        Native,
        OpenCL,
        Vulkan,
        CUDA,
        D3D12,
        Metal
    };

    enum class IL : unsigned char
    {
        None,
        SPIRV,
        CUDA,
        HLIL
    };

    enum class Info : unsigned char
    {
        Name,
        Board,
        Vendor,
        Profile,
        Version
    };

    typedef BitSet<Type>      Types;
    typedef BitSet<Attribute> Attributes;
    typedef Handle            pointer;
    typedef std::size_t       size_type;

    static size_type count () noexcept;
    static Device&   host  () noexcept;

    bool      available    (cchar* feature);
    string    info         (Info);
    size_type cache        () const;
    size_type cacheLine    () const;
    size_type localMemory  () const;
    size_type constMemory  () const;
    size_type globalMemory () const;

    size_type physical () const noexcept { return m_uId;      }
    Type      type     () const noexcept { return m_eType;    }
    bool      valid    () const noexcept { return m_pHandle;  }
    Backend   backend  () const noexcept { return m_eBackend; }

    template <typename T = pointer::pointer>
    T handle () const noexcept { return m_pHandle.get<T> (); }

private:
    pointer   m_pHandle;
    size_type m_uId;
    Type      m_eType;
    Backend   m_eBackend;
};

} } // Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_DEVICE_H_
