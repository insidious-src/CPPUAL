/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
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

#ifndef CPPUAL_AUDIO_DEVICE_H_
#define CPPUAL_AUDIO_DEVICE_H_
#ifdef __cplusplus

#include <cppual/noncopyable.h>
#include <cppual/compute/context.h>
#include <cppual/multimedia/audio/sound.h>
#include <cppual/multimedia/audio/spatial.h>

namespace cppual { namespace Audio {

enum class StringQuery : unsigned char
{
    Version,
    Renderer,
    Vendor
};

// ====================================================

class Device : public NonCopyable
{
public:
    typedef std::size_t size_type;
    typedef string string_type;

    enum Type
    {
        Null,
        Playback,
        Capture
    };

    Device(string_type const &, Type);

    static bool isExtensionSupported(string_type const &name) noexcept;
    bool        isExtensionPresent  (string_type const &name) noexcept;

    inline Type        type () const noexcept { return m_eType                ; }
    inline string_type name () const noexcept { return m_gDeviceName          ; }
    inline bool     isValid () const noexcept { return m_eType != Device::Null; }

protected:
    string_type m_gDeviceName;
    size_type   m_uId;
    Type        m_eType;

};

class PlaybackDevice : public virtual Device
{
public:
    PlaybackDevice  () noexcept;
    PlaybackDevice  (string_type const &name) noexcept;
    ~PlaybackDevice () noexcept;
};

class CaptureDevice : public virtual Device
{
public:
    CaptureDevice  () noexcept;
    ~CaptureDevice () noexcept;
    bool busy      () noexcept;

    CaptureDevice (string_type const&,
                   uint = 22050,
                   SoundQuality = SoundQuality::Low,
                   OutputFormat = OutputFormat::Stereo) noexcept;

    SoundQuality quality () const noexcept { return m_eQuality; }
    OutputFormat format  () const noexcept { return m_eFormat;  }

private:
    SoundQuality m_eQuality;
    OutputFormat m_eFormat;
};

// ====================================================

class Context
{
public:
    typedef string string_type;

    Context () = delete;
    Context (PlaybackDevice& device, bool make_current = false) noexcept;
    ~Context () noexcept;

    using Scenario = Compute::DeviceGroup;

    void use ();
    void invalidate () noexcept;
    bool setCapability (int cap, bool enable) noexcept;
    bool hasCapability (int cap) noexcept;
    void process () noexcept;
    void suspend () noexcept;

    static DistanceModel distanceModel () noexcept;
    static void          setDistanceModel (DistanceModel model) noexcept;
    static Context*      current () noexcept;
    static string_type   label(StringQuery query) noexcept;
    static void          setDopplerFactor (float factor) noexcept;
    static float         dopplerFactor () noexcept;
    static void          setSpeedOfSound (float speed) noexcept;
    static float         speedOfSound () noexcept;
    static Scenario      scenario () noexcept;
    static bool          setScenario (Scenario&) noexcept;

    PlaybackDevice& device () const noexcept
    { return *m_gDevice; }

private:
    PlaybackDevice* m_gDevice;
};

} } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_DEVICE_H_
