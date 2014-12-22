/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#ifndef CPPUAL_NETWORK_PACKET_H_
#define CPPUAL_NETWORK_PACKET_H_
#ifdef __cplusplus

#include <string>
#include <vector>
#include <cppual/types.h>
#include <cppual/noncopyable.h>

using std::vector;
using std::string;
using std::wstring;
using std::u16string;
using std::u32string;

namespace cppual { namespace Network {

class Packet
{
public:
	typedef std::size_t size_type;
	typedef bool (Packet::* BoolType)(size_type);

	bool operator == (Packet const&) const = delete;
	bool operator != (Packet const&) const = delete;
	void append (cvoid* data, size_type size_in_bytes) noexcept;

	Packet& operator >> (bool&) noexcept;
	Packet& operator >> (int8&) noexcept;
	Packet& operator >> (u8&) noexcept;
	Packet& operator >> (int16&) noexcept;
	Packet& operator >> (u16&) noexcept;
	Packet& operator >> (int32&) noexcept;
	Packet& operator >> (u32&) noexcept;
	Packet& operator >> (float&) noexcept;
	Packet& operator >> (double&) noexcept;
	Packet& operator >> (ldouble&) noexcept;
	Packet& operator >> (char*) noexcept;
	Packet& operator >> (string&) noexcept;
	Packet& operator >> (wchar*) noexcept;
	Packet& operator >> (wstring&) noexcept;
	Packet& operator >> (char16*) noexcept;
	Packet& operator >> (u16string&) noexcept;
	Packet& operator >> (char32*) noexcept;
	Packet& operator >> (u32string&) noexcept;

	Packet& operator << (bool) noexcept;
	Packet& operator << (int8) noexcept;
	Packet& operator << (u8) noexcept;
	Packet& operator << (int16) noexcept;
	Packet& operator << (u16) noexcept;
	Packet& operator << (int32) noexcept;
	Packet& operator << (u32) noexcept;
	Packet& operator << (float) noexcept;
	Packet& operator << (double) noexcept;
	Packet& operator << (ldouble) noexcept;
	Packet& operator << (cchar*) noexcept;
	Packet& operator << (string const&) noexcept;
	Packet& operator << (cwchar*) noexcept;
	Packet& operator << (wstring const&) noexcept;
	Packet& operator << (cchar16*) noexcept;
	Packet& operator << (u16string const&) noexcept;
	Packet& operator << (cchar32*) noexcept;
	Packet& operator << (u32string const&) noexcept;

	inline size_type getSize () const noexcept
	{ return m_gData.size (); }

	inline cvoid* getData () const noexcept
	{ return !m_gData.empty () ? &m_gData[0] : nullptr; }

	inline bool isEndOfPacket () const noexcept
	{ return m_uPos >= m_gData.size (); }

	inline operator BoolType () const noexcept
	{ return m_bIsValid ? &Packet::canExchange : nullptr; }

	inline void flush () noexcept
	{
		m_gData.clear ();
		m_uPos = 0;
	}

protected:
	virtual cvoid* onSend (size_type& size);
	virtual void   onReceive (cvoid* data, size_type size);

private:
	vector<char> m_gData;
	size_type    m_uPos;
	bool         m_bIsValid;

	bool canExchange (size_type) noexcept;
};

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_PACKET_H_
