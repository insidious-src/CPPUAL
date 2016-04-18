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

#ifndef CPPUAL_AUDIO_EMITTER_H_
#define CPPUAL_AUDIO_EMITTER_H_
#ifdef __cplusplus

#include <cppual/gfx/coord.h>
#include <cppual/multimedia/audio/al/alsource.h>

namespace cppual { namespace Audio { namespace AL {

class SoundEmitter final : public SoundSource
{
public:
    using    SoundSource::SoundSource;
    explicit SoundEmitter (SoundEmitter const&) noexcept;
    SoundEmitter& operator = (SoundEmitter&&) noexcept = default;
    SoundEmitter& operator = (SoundEmitter const&) noexcept;

    EmitType getType () const noexcept;

    void    setType (Audio::EmitType type) noexcept;
    void    setRelativeToListener (bool value) noexcept;
    bool    isRelativeToListener () const noexcept;

    void    setMinVolume (float value) noexcept;
    float    getMinVolume () const noexcept;
    void    setMaxVolume (float value) noexcept;
    float    getMaxVolume () const noexcept;
    void    setConeOuterVolume (float value) noexcept;
    float    getConeOuterVolume () const noexcept;

    void    setReferenceDistance (float value) noexcept;
    float    getReferenceDistance () const noexcept;
    void    setCancelation (float value) noexcept;
    float    getCancelation () const noexcept;
    void    setMaxDistance (float value) noexcept;
    float    getMaxDistance () const noexcept;
    void    setPosition (point3f const& pos) noexcept;
    point3f    getPosition () const noexcept;
    void    setVelocity (point3f const& pos) noexcept;
    point3f    getVelocity () const noexcept;
    void    setDirection (point3f const& pos) noexcept;
    point3f    getDirection () const noexcept;

    void    setConeInnerAngle (anglef angle) noexcept;
    anglef    getConeInnerAngle () const noexcept;
    void    setConeOuterAngle (anglef angle) noexcept;
    anglef    getConeOuterAngle () const noexcept;

private:
    void copyEmitterState (SoundEmitter const&) noexcept;
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_EMITTER_H_
