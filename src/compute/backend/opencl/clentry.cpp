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

#include <cppual/ui/manager.h>
#include <cppual/memory/stacked.h>

#include "cldevice.h"
#include "clcontext.h"
#include "clkernel.h"
#include "clprogram.h"
#include "clbuffer.h"
#include "climage.h"
#include "clevent.h"
#include "clcmdqueue.h"
#include "clqueue.h"
#include "clsampler.h"

namespace cppual { namespace compute { namespace cl {

class cl_factory final : factory
{
public:
    typedef device_vector& device_const_reference;

    constexpr cl_factory(memory::memory_resource& rc) noexcept
    : _M_rc(&rc)
    { }

    device_vector          get_devices(device_categories = device_category::any);
    shared_buffer          create_buffer();
    shared_cmd_sequence    create_cmd_sequence();
    shared_image           create_image();
    shared_pipeline        create_pipeline();
    shared_render_pass     create_render_pass();
    shared_shader          create_shader();
    shared_descriptor_pool create_descriptor_pool();
    shared_event           create_event();
    shared_state           create_state();
    shared_queue           create_queue();
    shared_sampler         create_sampler();
    size_type              device_count(device_categories = device_category::any);

private:
    device_const_reference all_devices() const;

private:
    memory::memory_resource* _M_rc;
};

// =========================================================

cl_factory::device_const_reference cl_factory::all_devices() const
{
    static auto devs = device::get_devices(*_M_rc);
    return devs;
}

// =========================================================

factory::device_vector cl_factory::get_devices(device_categories types)
{
    device_const_reference devs = all_devices();

    if (types != device_category::any)
    {
        device_vector ret_devs;
        auto const    dev_count = device_count(types);

        if (dev_count) ret_devs.reserve(dev_count);

        for (auto i = 0U; i < devs.size(); ++i)
        {
            if (types.test(devs[i]->dev_type())) ret_devs.push_back(devs[i]);
        }

        return ret_devs;
    }

    return devs;
}

factory::size_type cl_factory::device_count(device_categories types)
{
    device_const_reference devs = all_devices();

    if (types != device_category::any)
    {
        auto dev_count = size_type ();

        for (auto i = 0U; i < devs.size(); ++i)
        {
            if (types.test(devs[i]->dev_type())) ++dev_count;
        }

        return dev_count;
    }

    return devs.size();
}

} } } // namespace CL

// =========================================================

using cppual::compute::cl::cl_factory ;
using cppual::process::Plugin         ;
using cppual::memory::memory_resource ;
using cppual::memory::allocator       ;
using cppual::memory::stacked_resource;
using cppual::memory::allocate_shared ;

// =========================================================

extern "C" Plugin* plugin_main (memory_resource* rc)
{
    static char buffer[sizeof (cl_factory) + memory_resource::max_align * 2];
    static stacked_resource static_resource (buffer, sizeof (buffer));
    static Plugin plugin;

    plugin.name     = "cl_factory"      ;
    plugin.desc     = "OpenCL Factory"  ;
    plugin.provides = "Compute::Factory";
    plugin.verMajor = 1                 ;
    plugin.verMinor = 0                 ;

    plugin.iface    = allocate_shared<cl_factory, void>(&static_resource, *rc);

    return &plugin;
}