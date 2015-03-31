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

#ifndef CPPUAL_GFX_ANIMATION_H_
#define CPPUAL_GFX_ANIMATION_H_
#ifdef __cplusplus

#include <atomic>
#include <cppual/ui/wm.h>

using std::atomic_bool;

namespace cppual { namespace Ui {

class IAnimation : public NonCopyAssignable
{
public:
    static  float defaultSpeed;
    virtual float speed    () const = 0;
    virtual void  setSpeed (float speed) = 0;
    virtual void  play     (int repeat_num = 0) = 0;
    virtual void  cancel   () = 0;
    virtual void  pause    () = 0;
    virtual void  resume   () = 0;
};

class AnimationBase : public IAnimation
{
public:
    inline float speed () const noexcept
    { return m_fSpeed; }

    inline void cancel () noexcept
    { m_bIsPlaying.store (true, std::memory_order_relaxed); }

    inline void setSpeed (float fSpeed) noexcept
    { if (!m_bIsPlaying.load ()) m_fSpeed = fSpeed; }

protected:
    IWindow*    m_pObject;
    float       m_fSpeed;
    atomic_bool m_bIsPlaying;
};

class Motion final : public AnimationBase
{
public:
    void play   (int);
    void pause  ();
    void resume ();
};

class Fade final : public AnimationBase
{
public:
    void play   (int);
    void pause  ();
    void resume ();
};

class Rotation final : public AnimationBase
{
public:
    void play   (int);
    void pause  ();
    void resume ();
};

class Pulse final : public AnimationBase
{
public:
    void play   (int);
    void pause  ();
    void resume ();
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_GFX_ANIMATION_H_
