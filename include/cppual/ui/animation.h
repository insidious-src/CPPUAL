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

#ifndef CPPUAL_GFX_ANIMATION_H_
#define CPPUAL_GFX_ANIMATION_H_
#ifdef __cplusplus

#include <cppual/ui/wm.h>

#include <atomic>

namespace cppual { namespace ui {

class SHARED_API animation_interface : public non_copy_assignable
{
public:
    virtual float speed     () const = 0;
    virtual void  set_speed (float speed) = 0;
    virtual void  play      (int repeat_num = 0) = 0;
    virtual void  cancel    () = 0;
    virtual void  pause     () = 0;
    virtual void  resume    () = 0;
};

class animation_base : public animation_interface
{
public:
    typedef std::atomic_bool bool_type;

    inline float speed () const noexcept
    { return _M_fSpeed; }

    inline void cancel () noexcept
    { _M_bIsPlaying.store (true, std::memory_order_relaxed); }

    inline void set_speed (float fSpeed) noexcept
    { if (!_M_bIsPlaying.load ()) _M_fSpeed = fSpeed; }

protected:
    platform_wnd_interface* _M_pObject   ;
    float                   _M_fSpeed    ;
    bool_type               _M_bIsPlaying;
};

class SHARED_API motion final : public animation_base
{
public:
    void play   (int);
    void pause  ();
    void resume ();
};

class SHARED_API fade final : public animation_base
{
public:
    void play   (int);
    void pause  ();
    void resume ();
};

class SHARED_API rotation final : public animation_base
{
public:
    void play   (int);
    void pause  ();
    void resume ();
};

class SHARED_API pulse final : public animation_base
{
public:
    void play   (int);
    void pause  ();
    void resume ();
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_GFX_ANIMATION_H_
