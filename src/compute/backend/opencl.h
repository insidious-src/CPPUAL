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
#include <cppual/resource.h>
#include <cppual/memory/stack.h>
#include <cppual/compute/device.h>

#include <CL/cl.h>

#include <vector>
#include <type_traits>

/*
 * API Typesafe Encapsulation Guide Lines
 *
 * API calls should always start with root namespace "::";
 * define all API typedefs in a consistent non-conflicting way;
 * completely cross-platform implementation using the ISO C++14 standard;
 * for integer bit fields use enums;
 * for object encapsulation use template specialization,
 * inline & constexpr functions as much as possible;
 * if the api requires some initialization then use STL containers + cppual polymorphic allocators;
 * if possible prefer static allocators (cpu stack memory);
 * define all class typedefs according to STL (ex. typedef T value_type);
 * maintain strict typesafety;
 */

namespace cppual { namespace Compute { namespace CL {

typedef std::remove_pointer<cl_platform_id  >::type platform_type;
typedef std::remove_pointer<cl_device_id    >::type device_type  ;
typedef std::remove_pointer<cl_event        >::type event_type   ;
typedef std::remove_pointer<cl_kernel       >::type kernel_type  ;
typedef std::remove_pointer<cl_program      >::type program_type ;
typedef std::remove_pointer<cl_context      >::type context_type ;
typedef std::remove_pointer<cl_mem          >::type memory_type  ;
typedef std::remove_pointer<cl_command_queue>::type queue_type   ;

enum
{
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

template <typename> struct is_cl_object_helper       : public std::false_type { };
template <> struct is_cl_object_helper<device_type > : public std::true_type  { };
template <> struct is_cl_object_helper<event_type  > : public std::true_type  { };
template <> struct is_cl_object_helper<kernel_type > : public std::true_type  { };
template <> struct is_cl_object_helper<program_type> : public std::true_type  { };
template <> struct is_cl_object_helper<context_type> : public std::true_type  { };
template <> struct is_cl_object_helper<memory_type > : public std::true_type  { };
template <> struct is_cl_object_helper<queue_type  > : public std::true_type  { };

template <typename T>
struct is_cl_object : std::integral_constant<bool, (is_cl_object_helper<typename
                                                    std::remove_cv<T>::type>::value)>
{ static_assert (is_cl_object<T>::value, "invalid OpenCL object type!"); };

template <typename T>
using CLObject = typename std::enable_if<is_cl_object<T>::value, T>::type;

// =========================================================
// simplified shared_ptr reimplementation for managing OpenCL handles
// =========================================================

template <typename T>
class Object
{
public:
    typedef CLObject<T>* pointer    ;
    typedef std::size_t  size_type  ;
    typedef Handle       handle_type;

    Object (Object &&        ) = default ;
    Object (Object const& rhs)   noexcept;

    Object& operator = (Object &&        ) = default ;
    Object& operator = (Object const& rhs)   noexcept;
    ~Object () noexcept;

    constexpr Object ()               noexcept : m_object ()       { }
    constexpr Object (pointer handle) noexcept : m_object (handle) { }

    template <typename U = pointer>
    constexpr U handle () const noexcept { return m_object.get<U> (); }

private:
    handle_type m_object;
};

// =========================================================

template <>
inline Object<device_type>::~Object () noexcept
{
    if (m_object) ::clReleaseDevice (handle ());
}

template <>
inline Object<context_type>::~Object () noexcept
{
    if (m_object) ::clReleaseContext (handle ());
}

template <>
inline Object<kernel_type>::~Object () noexcept
{
    if (m_object) ::clReleaseKernel (handle ());
}

template <>
inline Object<program_type>::~Object () noexcept
{
    if (m_object) ::clReleaseProgram (handle ());
}

template <>
inline Object<queue_type>::~Object () noexcept
{
    if (m_object) ::clReleaseCommandQueue (handle ());
}

template <>
inline Object<memory_type>::~Object () noexcept
{
    if (m_object) ::clReleaseMemObject (handle ());
}

template <>
inline Object<event_type>::~Object () noexcept
{
    if (m_object) ::clReleaseEvent (handle ());
}

template <>
inline Object<device_type>::Object (Object const& rhs) noexcept
: m_object (rhs.m_object)
{
    if (m_object) ::clRetainDevice (handle ());
}

template <>
inline Object<context_type>::Object (Object const& rhs) noexcept
: m_object (rhs.m_object)
{
    if (m_object) ::clRetainContext (handle ());
}

template <>
inline Object<kernel_type>::Object (Object const& rhs) noexcept
: m_object (rhs.m_object)
{
    if (m_object) ::clRetainKernel (handle ());
}

template <>
inline Object<program_type>::Object (Object const& rhs) noexcept
: m_object (rhs.m_object)
{
    if (m_object) ::clRetainProgram (handle ());
}

template <>
inline Object<queue_type>::Object (Object const& rhs) noexcept
: m_object (rhs.m_object)
{
    if (m_object) ::clRetainCommandQueue (handle ());
}

template <>
inline Object<event_type>::Object (Object const& rhs) noexcept
: m_object (rhs.m_object)
{
    if (m_object) ::clRetainEvent (handle ());
}

template <>
inline Object<memory_type>::Object (Object const& rhs) noexcept
: m_object (rhs.m_object)
{
    if (m_object) ::clRetainMemObject (handle ());
}

template <>
inline
Object<device_type>&
Object<device_type>::operator = (Object const& rhs) noexcept
{
    if (this == &rhs) return *this;
    if (m_object) ::clReleaseDevice (handle ());
    m_object = rhs.m_object;
    if (m_object) ::clRetainDevice  (handle ());
    return *this;
}

template <>
inline
Object<context_type>&
Object<context_type>::operator = (Object const& rhs) noexcept
{
    if (this == &rhs) return *this;
    if (m_object) ::clReleaseContext (handle ());
    m_object = rhs.m_object;
    if (m_object) ::clRetainContext  (handle ());
    return *this;
}

template <>
inline
Object<kernel_type>&
Object<kernel_type>::operator = (Object const& rhs) noexcept
{
    if (this == &rhs) return *this;
    if (m_object) ::clReleaseKernel (handle ());
    m_object = rhs.m_object;
    if (m_object) ::clRetainKernel  (handle ());
    return *this;
}

template <>
inline
Object<program_type>&
Object<program_type>::operator = (Object const& rhs) noexcept
{
    if (this == &rhs) return *this;
    if (m_object) ::clReleaseProgram (handle ());
    m_object = rhs.m_object;
    if (m_object) ::clRetainProgram  (handle ());
    return *this;
}

template <>
inline
Object<queue_type>&
Object<queue_type>::operator = (Object const& rhs) noexcept
{
    if (this == &rhs) return *this;
    if (m_object) ::clReleaseCommandQueue (handle ());
    m_object = rhs.m_object;
    if (m_object) ::clRetainCommandQueue  (handle ());
    return *this;
}

template <>
inline
Object<event_type>&
Object<event_type>::operator = (Object const& rhs) noexcept
{
    if (this == &rhs) return *this;
    if (m_object) ::clReleaseEvent (handle ());
    m_object = rhs.m_object;
    if (m_object) ::clRetainEvent  (handle ());
    return *this;
}

template <>
inline
Object<memory_type>&
Object<memory_type>::operator = (Object const& rhs) noexcept
{
    if (this == &rhs) return *this;
    if (m_object) ::clReleaseMemObject (handle ());
    m_object = rhs.m_object;
    if (m_object) ::clRetainMemObject  (handle ());
    return *this;
}

// =========================================================

class Device : public Object<device_type>
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
    constexpr
    Device (pointer handle) noexcept : Object (handle) { }

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

    string info          (Info  );
    bool        has_extension (cchar*);

    static size_type count () noexcept
    { return list ().size  ();             }

    inline pointer handle  () const noexcept
    { return m_attributes.handle;          }

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

    // helper functions
    static cl_uint get_num_platforms () noexcept
    {
        cl_uint num;
        ::clGetPlatformIDs (0, nullptr, &num);
        return num;
    }

    static cl_uint get_num_devices (platform_type* platform)
    {
        cl_uint n;
        ::clGetDeviceIDs (platform, Device::Any, 0, nullptr, &n);
        return n;
    }
};

// =========================================================

} } } // namespace CL

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_BACKEND_OPENCL_H_
