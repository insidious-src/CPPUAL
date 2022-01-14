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

#ifndef CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
#define CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/flags.h>
#include <cppual/noncopyable.h>
#include <cppual/compute/object.h>

#include <memory>
#include <type_traits>

namespace cppual { namespace Compute {

// =========================================================

enum DeviceType
{
    None        =      0,
    CPU         = 1 << 0,
    GPU         = 1 << 1,
    GPGPU       = 1 << 2,
    Specialized = 1 << 3,
    Any         = CPU | GPU | GPGPU | Specialized
};

typedef BitSet<DeviceType> device_types;

// =========================================================

class IDevice;
class IBuffer;
class ICommandSequence;
class IImage;
class IPipeline;
class IRenderPass;
class IShader;
class IDescriptorPool;
class IEvent;
class IState;
class IQueue;

// =========================================================

typedef std::shared_ptr<Factory>          shared_factory;
typedef std::shared_ptr<IDevice>          shared_device;
typedef std::shared_ptr<IBuffer>          shared_buffer;
typedef std::shared_ptr<ICommandSequence> shared_cmd_sequence;
typedef std::shared_ptr<IImage>           shared_image;
typedef std::shared_ptr<IPipeline>        shared_pipeline;
typedef std::shared_ptr<IRenderPass>      shared_render_pass;
typedef std::shared_ptr<IShader>          shared_shader;
typedef std::shared_ptr<IDescriptorPool>  shared_descriptor_pool;
typedef std::shared_ptr<IEvent>           shared_event;
typedef std::shared_ptr<IState>           shared_state;
typedef std::shared_ptr<IQueue>           shared_queue;

// =========================================================

struct Factory : public NonCopyableVirtual
{
public:
    typedef std::size_t size_type;

    virtual shared_device          createDevice () = 0;
    virtual shared_buffer          createBuffer () = 0;
    virtual shared_cmd_sequence    createCmdSequence () = 0;
    virtual shared_image           createImage () = 0;
    virtual shared_pipeline        createPipeline () = 0;
    virtual shared_render_pass     createRenderPass () = 0;
    virtual shared_shader          createShader () = 0;
    virtual shared_descriptor_pool createDescriptorPool () = 0;
    virtual shared_event           createEvent () = 0;
    virtual shared_state           createState () = 0;
    virtual shared_queue           createQueue () = 0;
    virtual size_type              deviceCount () = 0;
    virtual device_types           deviceTypes () = 0;

public:
    static shared_factory instance();
    static bool           hasValidInstance() noexcept;
};

// =========================================================

class IDevice : public Object<ObjectType::Device>
{
public:
    using Object<ObjectType::Device>::Object;
};

// =========================================================

class IBuffer : public Object<ObjectType::Buffer>
{
public:
    using Object<ObjectType::Buffer>::Object;
};

// =========================================================

class ICommandSequence : public Object<ObjectType::CommandSequence>
{
public:
    using Object<ObjectType::CommandSequence>::Object;
};

// =========================================================

class IImage : public Object<ObjectType::Image>
{
public:
    using Object<ObjectType::Image>::Object;
};

// =========================================================

class IPipeline : public Object<ObjectType::Pipeline>
{
public:
    using Object<ObjectType::Pipeline>::Object;
};

// =========================================================

class IRenderPass : public Object<ObjectType::RenderPass>
{
public:
    using Object<ObjectType::RenderPass>::Object;
};

// =========================================================

class IShader : public Object<ObjectType::Shader>
{
public:
    using Object<ObjectType::Shader>::Object;
};

// =========================================================

class IDescriptorPool : public Object<ObjectType::DescriptorPool>
{
public:
    using Object<ObjectType::DescriptorPool>::Object;
};

// =========================================================

class IEvent : public Object<ObjectType::Event>
{
public:
    using Object<ObjectType::Event>::Object;
};

// =========================================================

class IState : public Object<ObjectType::State>
{
public:
    using Object<ObjectType::State>::Object;
};

// =========================================================

class IQueue : public Object<ObjectType::Queue>
{
public:
    using Object<ObjectType::Queue>::Object;
};

// =========================================================

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
