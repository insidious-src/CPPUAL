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

#ifndef CPPUAL_AUDIO_BUFFER_H_
#define CPPUAL_AUDIO_BUFFER_H_
#ifdef __cplusplus

#include <vector>
#include <cppual/common.h>
#include <cppual/multimedia/audio/sound.h>
#include <cppual/multimedia/audio/al/aldevice.h>

using std::vector;
using std::string;

namespace cppual { namespace Audio { namespace AL {

class SoundSource;

// ====================================================

class SoundBuffer : public AudioObject, public SoundData
{
public:
	typedef vector<SoundSource*>        vector_type;
	typedef vector_type::const_iterator const_iterator;
	typedef std::size_t                 size_type;
	friend  class                       SoundSource;

	SoundBuffer () noexcept;
	SoundBuffer (SoundBuffer&&) noexcept;
	SoundBuffer (SoundBuffer const&) noexcept;
	SoundBuffer& operator = (SoundBuffer&&) noexcept;
	SoundBuffer& operator = (SoundBuffer const&) noexcept;
	~SoundBuffer () noexcept;

	int getFrequency () const noexcept;
	int getSize () const noexcept;
	int getBits () const noexcept;
	int getDuration () const noexcept;

	inline Instance* getContext () const noexcept
	{ return m_pContext; }

private:
	vector_type m_gSources;
	Instance*    m_pContext;

	int onOpen () noexcept;
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_BUFFER_H_
