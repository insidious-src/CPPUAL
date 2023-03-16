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

#ifndef CPPUAL_AUDIO_BASE_H_
#define CPPUAL_AUDIO_BASE_H_
#ifdef __cplusplus

#include <cppual/multimedia/audio/spatial.h>

#include <atomic>

namespace cppual { namespace audio { namespace al {

class SHARED_API object
{
public:
    object (object_type type) noexcept;
    object (object&&) noexcept;
    object (object const&) noexcept;
    object& operator = (object&&) noexcept;
    object& operator = (object const&) noexcept;

    constexpr object () noexcept : _M_uObjId (), _M_eObjType () { }
    virtual  ~object () noexcept { reset (); }

    inline object_type type  () const noexcept { return _M_eObjType; }
    inline uint        id    () const noexcept { return _M_uObjId;   }
    inline bool        valid () const noexcept { return _M_uObjId;   }

    inline static uint count () noexcept
    { return sm_uALObjCount.load (std::memory_order_consume); }

private:
    static std::atomic_uint sm_uALObjCount;
    uint                     _M_uObjId    ;
    object_type              _M_eObjType  ;

    void reset () noexcept;
};

} } } // namespace al

#endif // __cplusplus
#endif // CPPUAL_AUDIO_BASE_H_
