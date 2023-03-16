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

#ifndef CPPUAL_AUDIO_DEVICE_H_
#define CPPUAL_AUDIO_DEVICE_H_
#ifdef __cplusplus

#include <cppual/noncopyable.h>
#include <cppual/compute/behaviour.h>
#include <cppual/multimedia/audio/sound.h>
#include <cppual/multimedia/audio/spatial.h>

namespace cppual { namespace audio {

enum class string_query : byte
{
    version,
    renderer,
    vendor
};

// ====================================================

class SHARED_API device : public non_copyable
{
public:
    typedef std::size_t size_type  ;
    typedef string      string_type;

    enum device_type
    {
        null,
        playback,
        capture
    };

    device(string_type const&, device_type);

    static bool is_extension_supported(string_type const& name) noexcept;
    bool        is_extension_present  (string_type const& name) noexcept;

    inline device_type type () const noexcept { return _M_eType                ; }
    inline string_type name () const noexcept { return _M_gDeviceName          ; }
    inline bool       valid () const noexcept { return _M_eType != device::null; }

protected:
    string_type _M_gDeviceName;
    size_type   _M_uId        ;
    device_type _M_eType      ;

};

class SHARED_API playback_device : public virtual device
{
public:
    playback_device  () noexcept;
    playback_device  (string_type const &name) noexcept;
    ~playback_device () noexcept;
};

class capture_device : public virtual device
{
public:
    capture_device  () noexcept;
    ~capture_device () noexcept;
    bool busy       () noexcept;

    capture_device (string_type const&,
                    uint = 22050,
                    sound_quality = sound_quality::low,
                    output_format = output_format::stereo) noexcept;

    sound_quality quality () const noexcept { return _M_eQuality; }
    output_format format  () const noexcept { return _M_eFormat;  }

private:
    sound_quality _M_eQuality;
    output_format _M_eFormat;
};

// ====================================================

class SHARED_API context
{
public:
    typedef string string_type;

    context () = delete;
    context (playback_device& device, bool make_current = false) noexcept;
    ~context () noexcept;

    using scenario_type = compute::device_group;

    void use ();
    void invalidate () noexcept;
    bool set_capability (int cap, bool enable) noexcept;
    bool has_capability (int cap) noexcept;
    void process () noexcept;
    void suspend () noexcept;

    static distance_type distance_model () noexcept;
    static void          set_distance_model (distance_type model) noexcept;
    static context*      current () noexcept;
    static string_type   label(string_query query) noexcept;
    static void          set_doppler_factor (float factor) noexcept;
    static float         doppler_factor () noexcept;
    static void          set_speed_of_sound (float speed) noexcept;
    static float         speed_of_sound () noexcept;
    static scenario_type scenario () noexcept;
    static bool          set_scenario (scenario_type&) noexcept;

    playback_device& get_device () const noexcept
    { return *_M_gDevice; }

private:
    playback_device* _M_gDevice;
};

} } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_DEVICE_H_
