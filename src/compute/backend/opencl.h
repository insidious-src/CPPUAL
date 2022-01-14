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
 *memory
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_COMPUTE_BACKEND_OPENCL_H_
#define CPPUAL_COMPUTE_BACKEND_OPENCL_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/string.h>
#include <cppual/resource.h>
#include <cppual/noncopyable.h>
#include <cppual/compute/object.h>
#include <cppual/compute/device.h>
#include <cppual/compute/backend_iface.h>

#define CL_TARGET_OPENCL_VERSION 200

#include <CL/cl.h>

#include <vector>
#include <type_traits>

/*
 * API Typesafe Encapsulation Guide Lines
 *
 * API calls should always start with root namespace "::";
 * define all API typedefs in a consistent non-conflicting way;
 * completely cross-platform implementation using the ISO C++17 standard;
 * for integer bit fields use enums;
 * for object encapsulation use template specialization,
 * inline & constexpr functions as much as possible;
 * if the api requires some initialization then use STL containers + cppual polymorphic allocators;
 * if possible prefer static allocators (cpu stack memory);
 * define all class typedefs according to STL (ex. typedef T value_type);
 * maintain strict typesafety;
 *
 */

namespace cppual { namespace Compute { namespace CL {

// =========================================================

typedef string string_type;

// =========================================================

typedef std::remove_pointer<cl_platform_id  >::type platform_type;
typedef std::remove_pointer<cl_device_id    >::type device_type  ;
typedef std::remove_pointer<cl_event        >::type event_type   ;
typedef std::remove_pointer<cl_kernel       >::type kernel_type  ;
typedef std::remove_pointer<cl_program      >::type program_type ;
typedef std::remove_pointer<cl_context      >::type context_type ;
typedef std::remove_pointer<cl_mem          >::type memory_type  ;
typedef std::remove_pointer<cl_command_queue>::type queue_type   ;
typedef std::remove_pointer<cl_sampler      >::type sampler_type ;

// =========================================================

enum
{
    Success            = CL_SUCCESS                         ,
    True               = CL_TRUE                            ,
    False              = CL_FALSE                           ,

    MaxUnits           = CL_DEVICE_MAX_COMPUTE_UNITS        ,
    MaxDimensions      = CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS ,
    MaxGroupSize       = CL_DEVICE_MAX_WORK_GROUP_SIZE      ,
    MaxItemCount       = CL_DEVICE_MAX_WORK_ITEM_SIZES      ,
    GlobalMemSize      = CL_DEVICE_GLOBAL_MEM_SIZE          ,
    CacheSize          = CL_DEVICE_GLOBAL_MEM_CACHE_SIZE    ,
    CacheLineSize      = CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
    ConstMemSize       = CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE ,
    LocalMemSize       = CL_DEVICE_LOCAL_MEM_SIZE           ,

    PlatformExtensions = CL_PLATFORM_EXTENSIONS             ,
    DeviceExtensions   = CL_DEVICE_EXTENSIONS
};

// =========================================================

class Platform    ;
class Device      ;
class Program     ;
class Kernel      ;
class Event       ;
class Context     ;
class MemoryChunk ;
class CommandQueue;

// =========================================================

class platform_exception : public std::bad_exception { };
class no_plaforms_found  : public platform_exception { };
class invalid_platform   : public platform_exception { };
class device_exception   : public std::bad_exception { };
class no_devices_found   : public device_exception   { };
class invalid_device     : public device_exception   { };

// =========================================================
// OpenCL Object Concept
// =========================================================

template <typename> struct is_cl_object_helper        : public std::false_type { };
template <> struct is_cl_object_helper<platform_type> : public std::true_type  { };
template <> struct is_cl_object_helper<device_type  > : public std::true_type  { };
template <> struct is_cl_object_helper<event_type   > : public std::true_type  { };
template <> struct is_cl_object_helper<kernel_type  > : public std::true_type  { };
template <> struct is_cl_object_helper<program_type > : public std::true_type  { };
template <> struct is_cl_object_helper<context_type > : public std::true_type  { };
template <> struct is_cl_object_helper<memory_type  > : public std::true_type  { };
template <> struct is_cl_object_helper<queue_type   > : public std::true_type  { };
template <> struct is_cl_object_helper<sampler_type > : public std::true_type  { };

template <typename T>
struct is_cl_object : std::integral_constant<bool, (is_cl_object_helper<typename
                                                    std::remove_cv<T>::type>::value)>
{ static_assert (is_cl_object<T>::value, "invalid OpenCL object type!"); };

template <typename T>
using CLObject = typename std::enable_if<is_cl_object<T>::value, T>::type;

// =========================================================

template <typename>
struct HandleType
{
    constexpr static auto value = ObjectType::Base;
};

template <>
struct HandleType<device_type>
{
    constexpr static auto value = ObjectType::Device;
};

template <>
struct HandleType<event_type>
{
    constexpr static auto value = ObjectType::Event;
};

template <>
struct HandleType<kernel_type>
{
    constexpr static auto value = ObjectType::Shader;
};

template <>
struct HandleType<program_type>
{
    constexpr static auto value = ObjectType::CommandSequence;
};

template <>
struct HandleType<context_type>
{
    constexpr static auto value = ObjectType::Pipeline;
};

template <>
struct HandleType<memory_type>
{
    constexpr static auto value = ObjectType::Buffer;
};

template <>
struct HandleType<queue_type>
{
    constexpr static auto value = ObjectType::Queue;
};

// =========================================================
// simplified implementation for managing OpenCL handles
// =========================================================

//! Program Handle Class
template <typename T>
class ObjectHandle : public Object<HandleType<T>::value>
{
public:
    typedef Object<HandleType<T>::value> base_type  ;
    typedef CLObject<T>*                 pointer    ;
    typedef std::size_t                  size_type  ;
    typedef Handle                       handle_type;

    ObjectHandle  () = delete ;
    ~ObjectHandle () = default;

    ObjectHandle (ObjectHandle&&) = default;
    ObjectHandle& operator = (ObjectHandle&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr ObjectHandle (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class ObjectHandle<device_type> : public IDevice
{
public:
    typedef IDevice                base_type  ;
    typedef CLObject<device_type>* pointer    ;
    typedef std::size_t            size_type  ;
    typedef Handle                 handle_type;

    ~ObjectHandle () noexcept;

    ObjectHandle (ObjectHandle&&) = default;
    ObjectHandle& operator = (ObjectHandle &&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr ObjectHandle (pointer handle) noexcept : base_type (handle) { }

    ObjectHandle ();
};

// =========================================================

template <>
class ObjectHandle<memory_type> : public IBuffer
{
public:
    typedef IBuffer                base_type  ;
    typedef CLObject<memory_type>* pointer    ;
    typedef std::size_t            size_type  ;
    typedef Handle                 handle_type;

    ~ObjectHandle () noexcept;

    ObjectHandle (ObjectHandle&&) = default;
    ObjectHandle& operator = (ObjectHandle&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr ObjectHandle (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class ObjectHandle<context_type> : public IPipeline
{
public:
    typedef IPipeline               base_type  ;
    typedef CLObject<context_type>* pointer    ;
    typedef std::size_t             size_type  ;
    typedef Handle                  handle_type;

    ~ObjectHandle () noexcept;

    ObjectHandle (ObjectHandle&&) = default;
    ObjectHandle& operator = (ObjectHandle&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr ObjectHandle (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class ObjectHandle<kernel_type> : public IShader
{
public:
    typedef IShader                base_type  ;
    typedef CLObject<kernel_type>* pointer    ;
    typedef std::size_t            size_type  ;
    typedef Handle                 handle_type;

    ~ObjectHandle () noexcept;

    ObjectHandle (ObjectHandle&&) = default;
    ObjectHandle& operator = (ObjectHandle&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr ObjectHandle (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class ObjectHandle<program_type> : public ICommandSequence
{
public:
    typedef ICommandSequence        base_type  ;
    typedef CLObject<program_type>* pointer    ;
    typedef std::size_t             size_type  ;
    typedef Handle                  handle_type;

    ~ObjectHandle () noexcept;

    ObjectHandle (ObjectHandle&&) = default;
    ObjectHandle& operator = (ObjectHandle&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr ObjectHandle (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class ObjectHandle<queue_type> : public IQueue
{
public:
    typedef IQueue                base_type  ;
    typedef CLObject<queue_type>* pointer    ;
    typedef std::size_t           size_type  ;
    typedef Handle                handle_type;

    ~ObjectHandle () noexcept;

    ObjectHandle (ObjectHandle&&) = default;
    ObjectHandle& operator = (ObjectHandle&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr ObjectHandle (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class ObjectHandle<event_type> : public IEvent
{
public:
    typedef IEvent                base_type  ;
    typedef CLObject<event_type>* pointer    ;
    typedef std::size_t           size_type  ;
    typedef Handle                handle_type;

    ~ObjectHandle () noexcept;

    ObjectHandle (ObjectHandle&&) = default;
    ObjectHandle& operator = (ObjectHandle&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr ObjectHandle (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

class Device : public ObjectHandle<device_type>
{
public:
    enum Type
    {
        Default     = CL_DEVICE_TYPE_DEFAULT    ,
        CPU         = CL_DEVICE_TYPE_CPU        ,
        GPU         = CL_DEVICE_TYPE_GPU        ,
        Accelerator = CL_DEVICE_TYPE_ACCELERATOR,
        Custom      = CL_DEVICE_TYPE_CUSTOM     ,
        Any         = CL_DEVICE_TYPE_ALL
    };

    size_type cache        () const noexcept;
    size_type cacheLine    () const noexcept;
    size_type localMemory  () const noexcept;
    size_type constMemory  () const noexcept;
    size_type globalMemory () const noexcept;

    constexpr static Compute::Device::Type convert (Type type) noexcept
    {
        return type == CPU         ? Compute::Device::CPU         :
               type == GPU         ? Compute::Device::GPU         :
               type == Accelerator ? Compute::Device::Accelerator :
               type == Custom      ? Compute::Device::Custom      :
                                     Compute::Device::Any         ;
    }

    constexpr static Type convert (Compute::Device::Type type) noexcept
    {
        return type == Compute::Device::CPU         ? CPU         :
               type == Compute::Device::GPU         ? GPU         :
               type == Compute::Device::Accelerator ? Accelerator :
               type == Compute::Device::Custom      ? Custom      :
                                                      Any         ;
    }

private:
    Device (pointer handle) noexcept : ObjectHandle (handle) { }

    friend class Platform;
};

// =========================================================

class Platform : public NonCopyable
{
public:
    typedef std::size_t    size_type;
    typedef platform_type* pointer  ;
    struct  attr_type { pointer handle; std::vector<Device::pointer>& devices; };

    enum Info
    {
        Name    = CL_PLATFORM_NAME   ,
        Vendor  = CL_PLATFORM_VENDOR ,
        Profile = CL_PLATFORM_PROFILE,
        Version = CL_PLATFORM_VERSION
    };

    Platform () = delete;

    string info          (Info);
    bool   has_extension (string_type const&);

    static size_type count () noexcept
    { return list ().size  (); }

    inline pointer handle  () const noexcept
    { return m_attributes.handle; }

    inline size_type device_count () const noexcept
    { return m_attributes.devices.size (); }

    static std::vector<Platform>& list ()
    {
        static std::vector<Platform> inst (init_platforms ());
        return inst;
    }

    static Platform& get (size_type id)
    {
        if (id >= count ()) throw invalid_platform ();
        return list ()[id];
    }

    Device device (size_type id) const
    {
        if (id >= device_count ()) throw invalid_device ();
        return m_attributes.devices[id];
    }

private:
    attr_type m_attributes;

    static std::vector<Platform> init_platforms ();

    Platform (platform_type* handle, std::vector<Device::pointer>& devices) noexcept
    : m_attributes { handle, devices } { }

    static u32 get_num_platforms () noexcept
    {
        u32 num;
        ::clGetPlatformIDs (0, nullptr, &num);
        return num;
    }

    static u32 get_num_devices (platform_type* platform)
    {
        u32 n;
        ::clGetDeviceIDs (platform, Device::Any, 0, nullptr, &n);
        return n;
    }
};

// =========================================================

} } } // namespace CL

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_BACKEND_OPENCL_H_
