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
#include <cppual/containers.h>
#include <cppual/noncopyable.h>
#include <cppual/compute/object.h>
#include <cppual/process/plugin.h>

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

enum class Backend : byte
{
    Native,
    OpenCL,
    Vulkan,
    D3D12 ,
    Metal
};

enum class IL : u16
{
    None   = 0,
    Native = 1 << 0,
    OpenCL = 1 << 1,
    GLSL   = 1 << 2,
    SPIR   = 1 << 3,
    SPIRV  = 1 << 4,
    HLIL   = 1 << 5
};

typedef BitSet<DeviceType> device_types;
typedef BitSet<IL>         device_ils  ;

// =========================================================

struct Factory;
class  IDevice;
class  IBuffer;
class  ICommandSequence;
class  IImage;
class  IPipeline;
class  IRenderPass;
class  IShader;
class  IDescriptorPool;
class  IEvent;
class  IState;
class  IQueue;
class  ISampler;

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
typedef std::shared_ptr<ISampler>         shared_sampler;

// =========================================================

struct SHARED_API Factory : public NonCopyableVirtual
{
public:
    typedef std::size_t                     size_type    ;
    typedef vector<shared_device>           device_vector;
    typedef Process::PluginManager<Factory> manager_type ;

    virtual device_vector          getDevices (device_types type = DeviceType::Any) = 0;
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
    virtual shared_sampler         createSampler () = 0;
    virtual size_type              deviceCount (device_types type = DeviceType::Any) = 0;

public:
    static shared_factory instance();
    static bool           hasValidInstance() noexcept;
};

// =========================================================

class SHARED_API IDevice : public Object<ResourceType::Device>
{
public:
    enum class Profile : byte
    {
        Full,
        Embedded
    };

    virtual string_type name              () const = 0;
    virtual string_type vendor            () const = 0;
    virtual Profile     profile           () const = 0;
    virtual Backend     backend           () const = 0;
    virtual device_ils  supportedILs      () const = 0;
    virtual DeviceType  devType           () const = 0;
    virtual Version     version           () const = 0;
    virtual size_type   cacheSize         () const = 0;
    virtual size_type   cacheLineSize     () const = 0;
    virtual size_type   localMemorySize   () const = 0;
    virtual size_type   constMemorySize   () const = 0;
    virtual size_type   globalMemorySize  () const = 0;
    virtual u32         computeUnits      () const = 0;
    virtual u64         maxMemoryAllocSize() const = 0;

protected:
    using Object<ResourceType::Device>::Object;
};

// =========================================================

class SHARED_API IBuffer : public Object<ResourceType::Buffer>
{
public:
    enum class AddressSpace : byte
    {
        GlobalMemory ,
        LocalMemory  ,
        PrivateMemory,
        ConstantMemory
    };

    enum class MemoryFlag : u16
    {
        ReadWrite     = 1 << 0,
        ReadOnly      = 1 << 1,
        WriteOnly     = 1 << 2,
        UseHostPtr    = 1 << 3,
        AllocHostPtr  = 1 << 4,
        CopyHostPtr   = 1 << 5,
        HostWriteOnly = 1 << 6,
        HostReadOnly  = 1 << 7,
        HostNoAccess  = 1 << 8
    };

    typedef BitSet<MemoryFlag> MemoryFlags;

protected:
    using Object<ResourceType::Buffer>::Object;
};

// =========================================================

class SHARED_API ICommandSequence : public Object<ResourceType::Program>
{
protected:
    using Object<ResourceType::Program>::Object;
};

// =========================================================

class SHARED_API IImage : public Object<ResourceType::Image>
{
protected:
    using Object<ResourceType::Image>::Object;
};

// =========================================================

class SHARED_API IPipeline : public Object<ResourceType::Context>
{
protected:
    using Object<ResourceType::Context>::Object;
};

// =========================================================

class SHARED_API IRenderPass : public Object<ResourceType::RenderPass>
{
protected:
    using Object<ResourceType::RenderPass>::Object;
};

// =========================================================

class SHARED_API IShader : public Object<ResourceType::Shader>
{
protected:
    using Object<ResourceType::Shader>::Object;
};

// =========================================================

class SHARED_API IDescriptorPool : public Object<ResourceType::DescriptorPool>
{
protected:
    using Object<ResourceType::DescriptorPool>::Object;
};

// =========================================================

class SHARED_API IEvent : public Object<ResourceType::Event>
{
protected:
    using Object<ResourceType::Event>::Object;
};

// =========================================================

class SHARED_API IState : public Object<ResourceType::State>
{
protected:
    using Object<ResourceType::State>::Object;
};

// =========================================================

class SHARED_API IQueue : public Object<ResourceType::Queue>
{
protected:
    using Object<ResourceType::Queue>::Object;
};

// =========================================================

class SHARED_API ISampler : public Object<ResourceType::Sampler>
{
protected:
    using Object<ResourceType::Sampler>::Object;
};

// =========================================================

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
