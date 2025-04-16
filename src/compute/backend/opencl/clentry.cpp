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

#include <cppual/compute/backend_iface.h>
#include <cppual/memory/stacked.h>

#include "cldevice.h"
#include "clcontext.h"
#include "clkernel.h"
#include "clprogram.h"
#include "clbuffer.h"
#include "climage.h"
#include "clevent.h"
#include "clcmdqueue.h"
#include "clsampler.h"

namespace cppual::compute::cl {

class cl_factory final : factory
{
public:
    typedef device_vector& devices_const_reference;

    constexpr cl_factory (memory::memory_resource& rc) noexcept
    : _M_rc(&rc)
    { }

    device_vector          get_devices(device_types = device_type::any);
    shared_context         create_context (device_vector const&);
    shared_memory          allocate_memory(shared_context const&, size_type, memory_access, memory_cat);
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
    size_type              device_count(device_types);

private:
    devices_const_reference all_devices() const;

private:
    memory::memory_resource* _M_rc;
};

// =========================================================

cl_factory::devices_const_reference cl_factory::all_devices() const
{
    static auto devs = device::get_devices(*_M_rc);
    return devs;
}

// =========================================================

factory::device_vector cl_factory::get_devices(device_types types)
{
    devices_const_reference devs = all_devices ();

    if (types != device_type::any)
    {
        device_vector ret_devs;
        auto const    dev_count = device_count (types);

        if (dev_count) ret_devs.reserve (dev_count);

        for (auto i = 0U; i < devs.size (); ++i)
        {
            if (types.test(devs[i]->dev_type ())) ret_devs.push_back (devs[i]);
        }

        return ret_devs;
    }

    return devs;
}

factory::size_type cl_factory::device_count(device_types types)
{
    devices_const_reference devs = all_devices ();

    if (types != device_type::any)
    {
        auto dev_count = size_type ();

        for (auto i = 0U; i < devs.size (); ++i)
        {
            if (types.test(devs[i]->dev_type ())) ++dev_count;
        }

        return dev_count;
    }

    return devs.size ();
}

shared_context cl_factory::create_context (device_vector const& devs)
{
    return shared_context (new context (devs, nullptr));
}

shared_memory cl_factory::allocate_memory (shared_context const& cntxt,
                                           size_type size,
                                           memory_access access,
                                           memory_cat cat)
{
    return shared_memory (new memory_chunk (cntxt, size, access, cat));
}

shared_image cl_factory::create_image ()
{
    return shared_image (new image (nullptr));
}

shared_cmd_sequence cl_factory::create_cmd_sequence ()
{
    return shared_cmd_sequence (new program (nullptr));
}

shared_pipeline cl_factory::create_pipeline ()
{
    return shared_pipeline ();
}

shared_render_pass cl_factory::create_render_pass ()
{
    return shared_render_pass ();
}

shared_shader cl_factory::create_shader ()
{
    return shared_shader (new cl::kernel (nullptr));
}

shared_descriptor_pool cl_factory::create_descriptor_pool ()
{
    return shared_descriptor_pool ();
}

shared_event cl_factory::create_event ()
{
    return shared_event (new event (nullptr));
}

shared_state cl_factory::create_state ()
{
    return shared_state ();
}

shared_queue cl_factory::create_queue ()
{
    return shared_queue (new command_queue (nullptr));
}

shared_sampler cl_factory::create_sampler ()
{
    return shared_sampler (new sampler (nullptr));
}


} // namespace CL

// =========================================================

using cppual::compute::cl::cl_factory ;
using cppual::process::plugin_vars    ;
using cppual::memory::memory_resource ;
using cppual::memory::allocator       ;
using cppual::memory::stacked_resource;
using cppual::memory::allocate_shared ;

// =========================================================

extern "C" plugin_vars* plugin_main (memory_resource* rc)
{
    static unsigned char buffer[sizeof (cl_factory) + memory_resource::max_adjust];
    static stacked_resource static_resource (buffer, sizeof (buffer));
    static plugin_vars plugin;

    plugin.name     = "cl_factory"      ;
    plugin.desc     = "OpenCL Factory"  ;
    plugin.provides = "compute::factory";
    plugin.verMajor = 1                 ;
    plugin.verMinor = 0                 ;
    plugin.iface    = allocate_shared<void, cl_factory> (&static_resource, *rc);

    return &plugin;
}