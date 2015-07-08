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

#ifndef CPPUAL_CL_CONTEXT_H_
#define CPPUAL_CL_CONTEXT_H_
#ifdef __cplusplus

#include <cppual/compute/clobject.h>

namespace cppual { namespace Compute {

class DeviceGroup { };

class VBlankOffload : public DeviceGroup
{ };

class ParallelOffload : public DeviceGroup
{ };

class CPU : public DeviceGroup
{ };

class GPU : public DeviceGroup
{ };

class All : public DeviceGroup
{ };

class Behaviour : public Object <Behaviour>
{
public:
    Behaviour (DeviceGroup&);
    Behaviour (Behaviour&&) = default;
    Behaviour& operator = (Behaviour&&) = default;
    ~Behaviour ();

    bool set (DeviceGroup&);

    DeviceGroup& devices () const noexcept
    { return *m_gDevice; }

private:
    DeviceGroup* m_gDevice;
};

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_CL_CONTEXT_H_
