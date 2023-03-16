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

#ifndef CPPUAL_AUDIO_EMITTER_H_
#define CPPUAL_AUDIO_EMITTER_H_
#ifdef __cplusplus

#include <cppual/gfx/coord.h>
#include <cppual/multimedia/audio/al/alsource.h>

namespace cppual { namespace audio { namespace al {

class SHARED_API sound_emitter : public sound_source
{
public:
    using sound_source::sound_source;

    explicit sound_emitter (sound_emitter const&) noexcept;
    sound_emitter& operator = (sound_emitter&&) noexcept = default;
    sound_emitter& operator = (sound_emitter const&) noexcept;

    emit_type type () const noexcept;
    void      set_type (audio::emit_type type) noexcept;
    void      set_relative_to_listener (bool value) noexcept;
    bool      is_relative_to_listener () const noexcept;

    void      set_min_volume (float value) noexcept;
    float     min_volume () const noexcept;
    void      set_max_volume (float value) noexcept;
    float     max_volume () const noexcept;
    void      set_cone_outer_volume (float value) noexcept;
    float     cone_outer_volume () const noexcept;

    void      set_reference_distance (float value) noexcept;
    float     reference_distance () const noexcept;
    void      set_cancelation (float value) noexcept;
    float     cancelation () const noexcept;
    void      set_max_distance (float value) noexcept;
    float     max_distance () const noexcept;
    void      set_position (point3f const& pos) noexcept;
    point3f   position () const noexcept;
    void      set_velocity (point3f const& pos) noexcept;
    point3f   velocity () const noexcept;
    void      set_direction (point3f const& pos) noexcept;
    point3f   direction () const noexcept;

    void      set_cone_inner_angle (anglef angle) noexcept;
    anglef    cone_inner_angle () const noexcept;
    void      set_cone_outer_angle (anglef angle) noexcept;
    anglef    cone_outer_angle () const noexcept;

private:
    void copy_emitter_state (sound_emitter const&) noexcept;
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_EMITTER_H_
