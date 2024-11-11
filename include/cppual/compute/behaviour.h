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

#ifndef CPPUAL_BEHAVIOR_H_
#define CPPUAL_BEHAVIOR_H_
#ifdef __cplusplus

// =========================================================

namespace cppual { namespace compute {

// =========================================================

class device_group { };
class vblank_offload : public device_group { };
class parallel_offload : public device_group { };
class cpu : public device_group { };
class gpu : public device_group { };
class gpgpu : public device_group { };
class all : public device_group { };

// =========================================================

class behaviour
{
public:
    behaviour (device_group&);
    ~behaviour ();

    behaviour (behaviour&&) = default;
    behaviour& operator = (behaviour&&) = default;

    bool set (device_group&);

    device_group& devices () const noexcept
    { return *_M_gDevGroup; }

private:
    device_group* _M_gDevGroup;
};

// =========================================================

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_BEHAVIOR_H_
