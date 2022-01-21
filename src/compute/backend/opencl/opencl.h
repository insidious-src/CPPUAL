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
#include <cppual/noncopyable.h>
#include <cppual/compute/object.h>
#include <cppual/compute/device.h>
#include <cppual/compute/backend_iface.h>
#include <cppual/containers.h>

#define CL_TARGET_OPENCL_VERSION 200

#include <CL/cl.h>
#include <CL/cl_ext.h>

#include <type_traits>
#include <exception>

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

typedef string                  string_type         ;
typedef ::cl_device_info        device_info_type    ;
typedef ::cl_device_type        device_category_type;
typedef ::cl_mem_info           memory_info_type    ;
typedef ::cl_context_info       context_info_type   ;
typedef ::cl_kernel_info        kernel_info_type    ;
typedef ::cl_program_info       program_info_type   ;
typedef ::cl_event_info         event_info_type     ;
typedef ::cl_command_queue_info cmd_queue_info_type ;
typedef ::cl_sampler_info       sampler_info_type   ;
typedef ::cl_image_info         image_info_type     ;

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

enum class SupportedILs : ::cl_device_exec_capabilities
{
    OpenCL = CL_EXEC_KERNEL,
    Native = CL_EXEC_NATIVE_KERNEL
};

typedef BitSet<SupportedILs> supported_ils_type;

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

template <ResourceType> struct conv_type            { typedef platform_type type; };
template <> struct conv_type<ResourceType::Device>  { typedef device_type   type; };
template <> struct conv_type<ResourceType::Event>   { typedef event_type    type; };
template <> struct conv_type<ResourceType::Shader>  { typedef kernel_type   type; };
template <> struct conv_type<ResourceType::Program> { typedef program_type  type; };
template <> struct conv_type<ResourceType::Context> { typedef context_type  type; };
template <> struct conv_type<ResourceType::Buffer>  { typedef memory_type   type; };
template <> struct conv_type<ResourceType::Image>   { typedef memory_type   type; };
template <> struct conv_type<ResourceType::Queue>   { typedef queue_type    type; };

// =========================================================

class opencl_error : public std::exception
{
public:
    /// Creates a new opencl_error exception object for \p error.
    explicit opencl_error (i32 error) noexcept
    : m_error(error),
      m_error_string(to_string(error))
    {
    }

    /// Destroys the opencl_error object.
    ~opencl_error() noexcept
    {
    }

    /// Returns the numeric error code.
    constexpr i32 error_code() const noexcept
    {
        return m_error;
    }

    /// Returns a string description of the error.
    string_type error_string() const noexcept
    {
        return m_error_string;
    }

    /// Returns a C-string description of the error.
    cchar* what() const noexcept
    {
        return m_error_string.c_str();
    }

    /// Static function which converts the numeric OpenCL error code \p error
    /// to a human-readable string.
    ///
    /// For example:
    /// \code
    /// std::cout << opencl_error::to_string(CL_INVALID_KERNEL_ARGS) << std::endl;
    /// \endcode
    ///
    /// Will print "Invalid Kernel Arguments".
    ///
    /// If the error code is unknown (e.g. not a valid OpenCL error), a string
    /// containing "Unknown OpenCL Error" along with the error number will be
    /// returned.
    static string_type to_string(i32 error);

private:
    i32       m_error;
    string_type m_error_string;
};

// =========================================================

struct no_platform_found : public std::exception
{
    /// Returns a string containing a human-readable error message.
    cchar* what() const noexcept
    {
        return "No OpenCL platform found";
    }
};

struct no_device_found : public std::exception
{
    /// Returns a string containing a human-readable error message.
    cchar* what() const noexcept
    {
        return "No OpenCL device found";
    }
};

// =========================================================

template <class Function, class Object, class AuxInfo>
struct bound_info_function
{
    bound_info_function(Function function, Object object, AuxInfo aux_info)
    : m_function(function),
      m_object(object),
      m_aux_info(aux_info)
    {
    }

    template <class Info>
    i32 operator()(Info info, size_t input_size, const void *input,
                   size_t size, void *value, size_t *size_ret) const
    {
        return m_function(
            m_object, m_aux_info, info,
            input_size, input, size, value, size_ret
        );
    }

    template <class Info>
    i32 operator()(Info info, size_t size, void *value, size_t* size_ret) const
    {
        return m_function(m_object, m_aux_info, info, size, value, size_ret);
    }

    Function m_function;
    Object   m_object;
    AuxInfo  m_aux_info;
};

template <class Function, class Object>
struct bound_info_function<Function, Object, void>
{
    bound_info_function(Function function, Object object)
    : m_function(function),
      m_object(object)
    {
    }

    template<class Info>
    i32 operator()(Info info, size_t size, void *value, size_t* size_ret) const
    {
        return m_function(m_object, info, size, value, size_ret);
    }

    Function m_function;
    Object   m_object;
};

template <class Function>
struct bound_info_function<Function, platform_type*, void>
{
    bound_info_function(Function function, platform_type* object)
    : m_function(function),
      m_object(object)
    {
    }

    template<class Info>
    i32 operator()(Info info, u32 size, device_type** value, u32* size_ret) const
    {
        return m_function(m_object, info, size, value, size_ret);
    }

    Function       m_function;
    platform_type* m_object;
};

template <class Function>
struct bound_info_function<Function, void, void>
{
    bound_info_function(Function function)
    : m_function(function)
    {
    }

    i32 operator()(size_t size, void *value, size_t* size_ret) const
    {
        return m_function(size, value, size_ret);
    }

    i32 operator()(u32 size, platform_type** value, u32* size_ret) const
    {
        return m_function(size, value, size_ret);
    }

    Function m_function;
};

template <class Function>
inline bound_info_function<Function, void, void>
bind_info_function(Function f)
{
    return bound_info_function<Function, void, void>(f);
}

template <class Function, class Object>
inline bound_info_function<Function, Object, void>
bind_info_function(Function f, Object o)
{
    return bound_info_function<Function, Object, void>(f, o);
}

template <class Function, class Object, class AuxInfo>
inline bound_info_function<Function, Object, AuxInfo>
bind_info_function(Function f, Object o, AuxInfo j)
{
    return bound_info_function<Function, Object, AuxInfo>(f, o, j);
}

//! default implementation
template <class T>
struct get_object_info_impl
{
    template<class Function, class Info>
    T operator()(Function function, Info info) const
    {
        T value;

        i32 ret = function(info, sizeof(T), &value, 0);
        if(ret != Success) throw (opencl_error(ret));

        return value;
    }

    template<class Function, class Info>
    T operator()(Function function, Info info,
                 const size_t input_size, const void* input) const
    {
        T value;

        i32 ret = function(info, input_size, input, sizeof(T), &value, 0);
        if(ret != Success) throw (opencl_error(ret));

        return value;
    }
};

//! specialization for bool
template<>
struct get_object_info_impl<bool>
{
    template<class Function, class Info>
    bool operator()(Function function, Info info) const
    {
        cl_bool value;

        i32 ret = function(info, sizeof(cl_bool), &value, 0);
        if(ret != Success) throw (opencl_error(ret));

        return value == CL_TRUE;
    }
};

//! specialization for string_type
template<>
struct get_object_info_impl<string_type>
{
    template <class Function, class Info>
    string_type operator()(Function function, Info info) const
    {
        size_t size = 0;

        i32 ret = function(info, 0, 0, &size);
        if(ret != Success) throw (opencl_error(ret));

        if(size == 0) return string_type();

        string_type value(size - 1, 0);

        ret = function(info, size, &value[0], 0);
        if(ret != Success) throw (opencl_error(ret));

        return value;
    }
};

//! specialization for vector<T>
template <class T>
struct get_object_info_impl< vector<T> >
{
    template <class Function>
    vector<T> operator()(Function function) const
    {
        size_t size = 0;

        i32 ret = function(0, 0, &size);
        if(ret != Success) throw (opencl_error(ret));

        if(size == 0) return vector<T>();

        vector<T> vector(size / sizeof(T));

        ret = function(size, &vector[0], 0);
        if(ret != Success) throw (opencl_error(ret));

        return vector;
    }

    template <class Function, class Info>
    vector<T> operator()(Function function, Info info) const
    {
        size_t size = 0;

        i32 ret = function(info, 0, 0, &size);
        if(ret != Success) throw (opencl_error(ret));

        if(size == 0) return vector<T>();

        vector<T> vector(size / sizeof(T));

        ret = function(info, size, &vector[0], 0);
        if(ret != Success) throw (opencl_error(ret));

        return vector;
    }

    template <class Function, class Info>
    vector<T> operator()(Function function, Info info,
                         const size_t input_size, const void* input) const
    {
        size_t size = 0;

        i32 ret = function(info, input_size, input, 0, 0, &size);
        if(ret != Success) throw (opencl_error(ret));

        vector<T> vector(size / sizeof(T));

        ret = function(info, input_size, input, size, &vector[0], 0);
        if(ret != Success) throw (opencl_error(ret));

        return vector;
    }
};

template <>
struct get_object_info_impl< vector<platform_type*> >
{
    template <class Function>
    vector<platform_type*> operator()(Function function) const
    {
        u32 size = 0;

        i32 ret = function(0, 0, &size);
        if(ret != Success) throw (opencl_error(ret));

        if(size == 0) return vector<platform_type*>();

        vector<platform_type*> vector(size / sizeof(platform_type*));

        ret = function(size, &vector[0], 0);
        if(ret != Success) throw (opencl_error(ret));

        return vector;
    }
};

template <>
struct get_object_info_impl< vector<device_type*> >
{
    template <class Function, class Info>
    vector<device_type*> operator()(Function function, Info info) const
    {
        u32 size = 0;

        i32 ret = function(info, 0, 0, &size);
        if(ret != Success) throw (opencl_error(ret));

        if(size == 0) return vector<device_type*>();

        vector<device_type*> vector(size / sizeof(device_type*));

        ret = function(info, size, &vector[0], 0);
        if(ret != Success) throw (opencl_error(ret));

        return vector;
    }
};

//! returns the value (of type T) from the given clGet*Info() function call.
template <class T, class Function>
inline T get_object_info(Function f)
{
    return get_object_info_impl<T>()(bind_info_function(f));
}

template <class T, class Function, class Object, class Info>
inline T get_object_info(Function f, Object o, Info i)
{
    return get_object_info_impl<T>()(bind_info_function(f, o), i);
}


template <class T, class Function, class Object, class Info, class AuxInfo>
inline T get_object_info(Function f, Object o, Info i, AuxInfo j)
{
    return get_object_info_impl<T>()(bind_info_function(f, o, j), i);
}

template <class T, class Function, class Object, class Info, class AuxInfo>
inline T get_object_info(Function f, Object o, Info i, AuxInfo j, const size_t k, const void * l)
{
    return get_object_info_impl<T>()(bind_info_function(f, o, j), i, k, l);
}

// =========================================================
// simplified implementation for managing OpenCL handles
// =========================================================

//! Handle Class
template <ResourceType T>
class Interface : public Object<T>
{
public:
    typedef Object<T>                              base_type  ;
    typedef CLObject<typename conv_type<T>::type>* pointer    ;
    typedef std::size_t                            size_type  ;
    typedef Handle                                 handle_type;

    Interface  () = delete ;
    ~Interface () = default;

    Interface (Interface&&) = default;
    Interface& operator = (Interface&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr explicit Interface (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class Interface<ResourceType::Device> : public IDevice
{
public:
    typedef IDevice                base_type  ;
    typedef CLObject<device_type>* pointer    ;
    typedef std::size_t            size_type  ;
    typedef Handle                 handle_type;

    ~Interface () noexcept;

    Interface (Interface&&) = default;
    Interface& operator = (Interface &&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr explicit Interface (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class Interface<ResourceType::Buffer> : public IBuffer
{
public:
    typedef IBuffer                base_type  ;
    typedef CLObject<memory_type>* pointer    ;
    typedef std::size_t            size_type  ;
    typedef Handle                 handle_type;

    ~Interface () noexcept;

    Interface (Interface&&) = default;
    Interface& operator = (Interface&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr explicit Interface (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class Interface<ResourceType::Image> : public IImage
{
public:
    typedef IImage                 base_type  ;
    typedef CLObject<memory_type>* pointer    ;
    typedef std::size_t            size_type  ;
    typedef Handle                 handle_type;

    ~Interface () noexcept;

    Interface (Interface&&) = default;
    Interface& operator = (Interface&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr explicit Interface (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class Interface<ResourceType::Context> : public IPipeline
{
public:
    typedef IPipeline               base_type  ;
    typedef CLObject<context_type>* pointer    ;
    typedef std::size_t             size_type  ;
    typedef Handle                  handle_type;

    ~Interface () noexcept;

    Interface (Interface&&) = default;
    Interface& operator = (Interface&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr explicit Interface (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class Interface<ResourceType::Shader> : public IShader
{
public:
    typedef IShader                base_type  ;
    typedef CLObject<kernel_type>* pointer    ;
    typedef std::size_t            size_type  ;
    typedef Handle                 handle_type;

    ~Interface () noexcept;

    Interface (Interface&&) = default;
    Interface& operator = (Interface&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr explicit Interface (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class Interface<ResourceType::Program> : public ICommandSequence
{
public:
    typedef ICommandSequence        base_type  ;
    typedef CLObject<program_type>* pointer    ;
    typedef std::size_t             size_type  ;
    typedef Handle                  handle_type;

    ~Interface () noexcept;

    Interface (Interface&&) = default;
    Interface& operator = (Interface&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr explicit Interface (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class Interface<ResourceType::Queue> : public IQueue
{
public:
    typedef IQueue                base_type  ;
    typedef CLObject<queue_type>* pointer    ;
    typedef std::size_t           size_type  ;
    typedef Handle                handle_type;

    ~Interface () noexcept;

    Interface (Interface&&) = default;
    Interface& operator = (Interface&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr explicit Interface (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class Interface<ResourceType::Event> : public IEvent
{
public:
    typedef IEvent                base_type  ;
    typedef CLObject<event_type>* pointer    ;
    typedef std::size_t           size_type  ;
    typedef Handle                handle_type;

    ~Interface () noexcept;

    Interface (Interface&&) = default;
    Interface& operator = (Interface&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr explicit Interface (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

template <>
class Interface<ResourceType::Sampler> : public ISampler
{
public:
    typedef ISampler                base_type  ;
    typedef CLObject<sampler_type>* pointer    ;
    typedef std::size_t             size_type  ;

    ~Interface () noexcept;

    Interface (Interface&&) = default;
    Interface& operator = (Interface&&) = default;

    constexpr operator pointer () const noexcept
    {
        return handle ();
    }

    constexpr pointer handle () const noexcept
    {
        return base_type::template handle<pointer>();
    }

protected:
    constexpr explicit Interface (pointer handle) noexcept : base_type (handle) { }
};

// =========================================================

} } } // namespace CL

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_BACKEND_OPENCL_H_
