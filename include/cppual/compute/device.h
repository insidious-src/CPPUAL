/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <cppual/mstring.h>
#include <cppual/flags.h>
#include <cppual/noncopyable.h>
#include <cppual/compute/clobject.h>

using std::string;

namespace cppual { namespace Compute {

class device_exception    : public std::bad_exception { };
class bad_device          : public device_exception   { };
class not_available       : public device_exception   { };
class bad_partition_count : public device_exception   { };

// =========================================================

class Device
{
public:
	enum Type
	{
		CPU         = 1 << 1,
		GPU         = 1 << 2,
		Accelerator = 1 << 3,
		Custom      = 1 << 4,
	};

	enum class DevType
	{
		CPU, // serial processing
		GPU, // shader and parallel processing
		PPU, // dedicated parallel processing
		DSP, // digital signal processing (audio)
		BSD, // bit stream decoder (video)
		VCE, // codec encoder (video)
		SPU, // multiple specialized processing cores
		Custom
	};

	enum
	{
		Null    = 0,
		Default = 1 << 0,
		All     = Default | CPU | GPU | Accelerator | Custom
	};

	enum class Info : unsigned char
	{
		Name,
		Board,
		Vendor,
		Profile,
		Version
	};

	class Partition : public Object <Partition>
	{
	public:
		Partition () = delete;
		Partition (Partition&&);
		Partition (Partition const&);
		Partition& operator = (Partition&&);
		Partition& operator = (Partition const&);

	private:
		Partition (Device&);
		friend class Device;
	};

	typedef typename std::underlying_type<Type>::type type_size;
	typedef Object<Device>::int_type                  int_type;
	typedef Object<Device>::uint_type                 uint_type;
	typedef std::size_t                               size_type;

	static uint_type count (type_size type, u16 platform = 0);
	bool   available       (cchar* feature);

	Device (Device const&) noexcept;
	Device (Type type = count (GPU) ? GPU : CPU, u16 platform = 0, uint_type id = 0);

	Partition part (uint_type);
	string    info (Info);
	uint_type dimensions   () const;
	size_type groupSize    () const;
	uint_type items        () const;
	size_type cache        () const;
	uint_type cacheLine    () const;
	size_type localMemory  () const;
	size_type constMemory  () const;
	size_type globalMemory () const;

	inline uint_type platform () const noexcept { return m_uPlatformId; }
	inline uint_type units    () const noexcept { return m_uNumUnits; }
	inline Type      type     () const noexcept { return m_eType; }
	inline bool      isValid  () const noexcept { return m_uNumUnits; }

private:
	u16       m_uPlatformId;
	Type      m_eType;
	uint_type m_uId, m_uNumUnits;
};

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>&
operator << (std::basic_ostream<CharT, Traits>& os, Device::Type u)
{
	switch (u)
	{
	case Device::CPU:
		return os << "CPU";
	case Device::GPU:
		return os << "GPU";
	case Device::Accelerator:
		return os << "Accelerator";
	case Device::Custom:
		return os << "Custom";
	default:
		return os;
	}
}

}

// =========================================================

int32 reference (Disposable<Compute::Device::Partition>*) noexcept;

template <>
struct Disposable <Compute::Device::Partition>
{
	static void* operator new (std::size_t) noexcept;
	static void  operator delete (void*) noexcept;
};

} // Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_DEVICE_H_
