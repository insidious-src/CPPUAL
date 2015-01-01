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

#include <cppual/multimedia/audio/sound.h>
#include <cppual/types.h>
#include <sndfile.h>
#include <string>
#include <locale>

using std::string;

namespace cppual { namespace Audio {

SoundData::SoundData (SoundData&& gObj) noexcept
: m_pSndDesc (gObj.m_pSndDesc),
  m_nSampleCount (gObj.m_nSampleCount),
  m_nSampleRate (gObj.m_nSampleRate),
  m_nChannelCount (gObj.m_nChannelCount),
  m_gFlags (gObj.m_gFlags)
{
	gObj.m_nSampleCount = gObj.m_nChannelCount = gObj.m_nSampleRate = 0;
	gObj.m_pSndDesc     = nullptr;
	gObj.m_gFlags       = 0;
}

SoundData& SoundData::operator = (SoundData&& gObj) noexcept
{
	if (this == &gObj) return *this;
	return *this;
}

SoundData::~SoundData () noexcept
{
	if (m_pSndDesc) sf_close (static_cast<SNDFILE*> (m_pSndDesc));
}

int SoundData::openReadOnly (string const& gFile) noexcept
{
	close ();

	SF_INFO gInfo { 0, 0, 0, 0, 0, 0 };
	m_pSndDesc = sf_open (gFile.c_str (), 0, &gInfo);

	if (m_pSndDesc)
	{
		if (gInfo.channels > MaxChannels)
		{
			sf_close (static_cast<SNDFILE*> (m_pSndDesc));
			return false;
		}

		m_nChannelCount = gInfo.channels;
		m_nSampleCount  = gInfo.frames;
		m_nSampleRate   = gInfo.samplerate;

		if (gInfo.seekable) m_gFlags += SoundData::Seekable;
		m_gFlags += SoundData::Read;

		// using format to save an int copy
		if (!(gInfo.format = onOpen ()))
		{
			close ();
			return gInfo.format;
		}

		return true;
	}

	return false;
}

int SoundData::openReadOnly (cvoid*, size_type) noexcept
{
	close ();
	m_gFlags = SoundData::Read;
	return false;
}

int SoundData::openWritable (string const&, int, int) noexcept
{
	close ();
	m_gFlags = SoundData::Read | SoundData::Write;

	onOpen ();
	return false;
}

bool SoundData::save (string const& gFileName) noexcept
{
	if (m_pSndDesc)
	{
		onSave (gFileName);
		return true;
	}

	return false;
}

void SoundData::close () noexcept
{
	if (m_pSndDesc)
	{
		onClose ();
		sf_close (static_cast<SNDFILE*> (m_pSndDesc));

		m_nSampleCount = m_nSampleRate = m_nChannelCount = 0;
		m_gFlags.clear ();
	}
}

void SoundData::seek (seconds) noexcept
{
	if (m_gFlags.hasBit (SoundData::Seekable))
	{
	}
}

string SoundData::attribute (SoundAttrib::Value eType) const noexcept
{
	return sf_get_string (static_cast<SNDFILE*> (m_pSndDesc), eType);
}



} } // namespace Audio
