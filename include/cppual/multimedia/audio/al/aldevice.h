/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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


#include <cppual/noncopyable>
#include <cppual/containers>
#include <cppual/resource>
#include <cppual/string>

#include <cppual/multimedia/audio/sound.h>
#include <cppual/multimedia/audio/spatial.h>
#include <cppual/multimedia/audio/al/alobject.h>

namespace cppual { namespace audio { namespace al {

// ====================================================

enum class device_type : u8
{
    null,
    playback,
    capture
};

enum class string_query : u8
{
    version,
    renderer,
    vendor
};

// ====================================================

class SHARED_API audio_device : public resource<void, void*>
{
public:
    typedef string string_type;

    audio_device () = delete;
    static bool is_extension_supported (string const& name) noexcept;
    bool        is_extension_present   (string const& name) noexcept;

    constexpr device_type type () const noexcept { return _M_eType      ; }
    inline    string_type name () const noexcept { return _M_gDeviceName; }

protected:
    string_type _M_gDeviceName;
    device_type _M_eType      ;

    inline
    audio_device (string_type const& gName, void* pObj, device_type eType) noexcept
    : resource       (pObj),
      _M_gDeviceName (std::move (gName)),
      _M_eType       (eType)
    { }
};

class SHARED_API playback_device : public audio_device
{
public:
    playback_device  () noexcept;
    playback_device  (string_type const& name) noexcept;
    ~playback_device () noexcept;
};

class SHARED_API capture_device : public audio_device
{
public:
    typedef string string_type;

    capture_device  () noexcept;
    ~capture_device () noexcept;

    capture_device (string_type const& name,
                    uint               freq    = 22050,
                    output_format      format  = output_format::stereo,
                    sound_quality      quality = sound_quality::low) noexcept;

    constexpr sound_quality quality () const noexcept { return _M_eQuality; }
    constexpr output_format format  () const noexcept { return _M_eFormat ; }

private:
    sound_quality _M_eQuality;
    output_format _M_eFormat ;
};

// ====================================================

class SHARED_API instance final : public non_copyable
{
public:
    typedef string                 string_type;
    typedef dyn_array<string_type> ext_list   ;
    typedef resource_handle        handle_type;

    instance () = delete;
    instance (playback_device& device, bool current = false) noexcept;
    ~instance () noexcept;

    bool use () noexcept;
    void invalidate () noexcept;
    bool set_capability (int cap, bool enable) noexcept;
    bool has_capability (int cap) noexcept;
    void process () noexcept;
    void suspend () noexcept;

    static distance_type distance_model () noexcept;
    static void          set_distance_model (distance_type model) noexcept;
    static instance*     current () noexcept;
    static string        label (string_query query) noexcept;
    static ext_list      extensions () noexcept;
    static void          set_doppler_factor (float factor) noexcept;
    static float         doppler_factor () noexcept;
    static void          set_speed_of_sound (float speed) noexcept;
    static float         speed_of_sound () noexcept;

    constexpr playback_device& get_device () const noexcept { return *_M_gDevice    ; }
    constexpr bool             valid      () const noexcept { return  _M_pDevContext; }

private:
    playback_device* _M_gDevice    ;
    handle_type      _M_pDevContext;
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_DEVICE_H_
