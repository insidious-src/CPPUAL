/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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
#include <cppual/memory/static.h>

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

namespace cppual { namespace Compute { namespace CL {

class CLFactory final : Factory
{
public:
    typedef device_vector& device_const_reference;

    constexpr CLFactory(Memory::MemoryResource& rc) noexcept
    : _M_rc(&rc)
    { }

    device_vector          getDevices(device_types = DeviceType::Any);
    shared_buffer          createBuffer();
    shared_cmd_sequence    createCmdSequence();
    shared_image           createImage();
    shared_pipeline        createPipeline();
    shared_render_pass     createRenderPass();
    shared_shader          createShader();
    shared_descriptor_pool createDescriptorPool();
    shared_event           createEvent();
    shared_state           createState();
    shared_queue           createQueue();
    shared_sampler         createSampler();
    size_type              deviceCount(device_types = DeviceType::Any);

private:
    device_const_reference allDevices() const;

private:
    Memory::MemoryResource* _M_rc;
};

// =========================================================

CLFactory::device_const_reference CLFactory::allDevices() const
{
    static auto devs = Device::get_devices(*_M_rc);
    return devs;
}

// =========================================================

Factory::device_vector CLFactory::getDevices(device_types types)
{
    device_const_reference devs = allDevices();

    if (types != DeviceType::Any)
    {
        device_vector ret_devs;
        auto const    dev_count = deviceCount(types);

        if (dev_count) ret_devs.reserve(dev_count);

        for (auto i = 0U; i < devs.size(); ++i)
        {
            if (types.test(devs[i]->devType())) ret_devs.push_back(devs[i]);
        }

        return ret_devs;
    }

    return devs;
}

Factory::size_type CLFactory::deviceCount(device_types types)
{
    device_const_reference devs = allDevices();

    if (types != DeviceType::Any)
    {
        auto dev_count = size_type ();

        for (auto i = 0U; i < devs.size(); ++i)
        {
            if (types.test(devs[i]->devType())) ++dev_count;
        }

        return dev_count;
    }

    return devs.size();
}

} } } // namespace CL

// =========================================================

using cppual::Compute::CL::CLFactory ;
using cppual::Process::Plugin        ;
using cppual::Memory::MemoryResource ;
using cppual::Memory::Allocator      ;
using cppual::Memory::StaticResource ;
using cppual::Memory::allocate_shared;

// =========================================================

extern "C" Plugin* plugin_main (MemoryResource* rc)
{
    static StaticResource<sizeof (CLFactory)> static_resource;
    static Plugin plugin;

    plugin.name     = "CLFactory"       ;
    plugin.desc     = "OpenCL Factory"  ;
    plugin.provides = "Compute::Factory";
    plugin.verMajor = 1                 ;
    plugin.verMinor = 0                 ;

    plugin.iface    = allocate_shared<CLFactory, void>(&static_resource, *rc);

    return &plugin;
}