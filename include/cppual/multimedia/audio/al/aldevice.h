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

#ifndef CPPUAL_AUDIO_DEVICE_H_
#define CPPUAL_AUDIO_DEVICE_H_
#ifdef __cplusplus

#include <vector>
#include <string>
#include <cppual/noncopyable.h>
#include <cppual/multimedia/audio/sound.h>
#include <cppual/multimedia/audio/spatial.h>
#include <cppual/multimedia/audio/al/alobject.h>

using std::vector;
using std::string;

namespace cppual { namespace Audio { namespace AL {

enum class DeviceType : unsigned char
{
	Null,
	Playback,
	Capture
};

enum class StringQuery : unsigned char
{
	Version,
	Renderer,
	Vendor
};

// ====================================================

class AudioDevice : public NonCopyable
{
public:
	AudioDevice () = delete;
	static bool isExtensionSupported (string const& name) noexcept;
	bool		isExtensionPresent   (string const& name) noexcept;

	inline DeviceType type () const noexcept { return m_eType; }
	inline string     name () const noexcept { return m_gDeviceName; }
	inline void*      object () noexcept { return m_pDevObj; }
	inline bool       isValid () const noexcept { return m_pDevObj; }

protected:
	string     m_gDeviceName;
	void*      m_pDevObj;
	DeviceType m_eType;

	inline
	AudioDevice (string const& gName, void* pObj, DeviceType eType) noexcept
	: m_gDeviceName (std::move (gName)),
	  m_pDevObj (pObj),
	  m_eType (eType)
	{ }
};

class PlaybackDevice final : public AudioDevice
{
public:
	PlaybackDevice () noexcept;
	PlaybackDevice (string const& name) noexcept;
	~PlaybackDevice () noexcept;
};

class CaptureDevice final : public AudioDevice
{
public:
	CaptureDevice () noexcept;
	~CaptureDevice () noexcept;

	CaptureDevice (string const& name,
				   uint          freq    = 22050,
				   OutputFormat  format  = OutputFormat::Stereo,
				   SoundQuality  quality = SoundQuality::Low) noexcept;

	SoundQuality quality () const noexcept { return m_eQuality; }
	OutputFormat format () const noexcept { return m_eFormat; }

private:
	SoundQuality m_eQuality;
	OutputFormat m_eFormat;
};

// ====================================================

class Instance final : public NonCopyable
{
public:
	typedef vector<string> ext_list;
	Instance () = delete;
	Instance (PlaybackDevice& device, bool current = false) noexcept;
	~Instance () noexcept;

	bool use () noexcept;
	void invalidate () noexcept;
	bool setCapability (int cap, bool enable) noexcept;
	bool hasCapability (int cap) noexcept;
	void process () noexcept;
	void suspend () noexcept;

	static DistanceModel distanceModel () noexcept;
	static void			 setDistanceModel (DistanceModel model) noexcept;
	static Instance*	 current () noexcept;
	static string		 label (StringQuery query) noexcept;
	static ext_list		 extensions () noexcept;
	static void			 setDopplerFactor (float factor) noexcept;
	static float		 dopplerFactor () noexcept;
	static void			 setSpeedOfSound (float speed) noexcept;
	static float		 speedOfSound () noexcept;

	inline PlaybackDevice& device  () const noexcept { return m_gDevice; }
	inline bool	           isValid () const noexcept { return m_pDevContext; }

private:
	PlaybackDevice& m_gDevice;
	void*           m_pDevContext;
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_DEVICE_H_
